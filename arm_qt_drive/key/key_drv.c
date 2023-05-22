#include "asm-generic/fcntl.h"
#include "asm/gpio.h"
#include "asm/uaccess.h"
#include "linux/err.h"
#include "linux/export.h"
#include "linux/interrupt.h"
#include "linux/irq.h"
#include "linux/irqreturn.h"
#include "linux/jiffies.h"
#include "linux/of_gpio.h"
#include "linux/printk.h"
#include "linux/slab.h"
#include "linux/timer.h"
#include "linux/wait.h"
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/gfp.h>
#include <linux/gpio/consumer.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kmod.h>
#include <linux/major.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/tty.h>

#define BUFFER_MAX_LEN 200
#define NEXT_POS(a) ((a + 1) % BUFFER_MAX_LEN)

typedef struct key_object {
    int irq;
    int gpio;
    struct gpio_desc *gdesc;
    struct timer_list timer;
} key_object;


static DECLARE_WAIT_QUEUE_HEAD(key_wait);
static int push_idx = 0;
static int pop_idx = 0;
static int key_buf[BUFFER_MAX_LEN];
static int major;
static struct class *key_class;
static key_object *key;

static int buffer_is_null(void) {
    return push_idx == pop_idx;
}

static int pop_val(void) {
    int val;
    if (buffer_is_null())
        return -1;
    val = key_buf[pop_idx];
    pop_idx = NEXT_POS(pop_idx);
    return val;
}

static void push_val(int val) {
    key_buf[push_idx] = val;
    push_idx = NEXT_POS(push_idx);
}

static ssize_t key_read(struct file *file, char __user *buf, size_t size, loff_t *ppos) {
    int val;
    int ret;

    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    if (buffer_is_null() & file->f_flags & ~O_NONBLOCK) {
        return 0;
    }
    
    wait_event_interruptible(key_wait, !buffer_is_null());
    val = pop_val();
    ret = copy_to_user(buf, &val, 4);
    
    return 4;
}

static ssize_t key_write(struct file *file, const char __user *buf, size_t size, loff_t *ppos) {
    return 0;
}

static int key_open(struct inode *node, struct file *file) {
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

static int key_release(struct inode *node, struct file *file) {
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

static int key_fasync(int fd, struct file *file, int on) {
    return 0;
}

static struct file_operations key_fops = {
    .owner = THIS_MODULE,
    .open = key_open,
    .release = key_release,
    .write = key_write,
    .read = key_read,
    .fasync = key_fasync,
    .llseek = no_llseek,
};

static void key_timer_func(unsigned long data) {
    int val;
    int gpio;
    key_object *key_data = (key_object *)data;
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    val = gpio_get_value(key_data->gpio);
    gpio = (key_data->gpio << 8 | val);
    printk("gpio = %d\n", gpio);
    push_val(gpio);
    wake_up_interruptible(&key_wait);
}

static irqreturn_t key_irq_func(int irq, void *data) {
    key_object *key_data = (key_object *)data;
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    mod_timer(&key_data->timer, jiffies + HZ / 50);
    return IRQ_WAKE_THREAD;
}

static irqreturn_t key_irq_thread_func(int irq, void *data) {
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    return IRQ_HANDLED;
}

static int key_drv_probe(struct platform_device *pdev) {
    int count;
    int i = 0;
    int ret;
    struct device_node *node = pdev->dev.of_node;
    enum of_gpio_flags flags;
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    
    count = of_gpio_count(node);
    key = kzalloc(count * sizeof(key_object), GFP_KERNEL);
    for (; i < count; i++) {
        key[i].gpio = of_get_gpio_flags(node, i, &flags);
        key[i].irq = gpio_to_irq(key[i].gpio);
        key[i].gdesc = gpio_to_desc(key[i].gpio);
        setup_timer(&key[i].timer, key_timer_func, (unsigned long)&key[i]);
        key[i].timer.expires = ~0;
        add_timer(&key[i].timer);
        ret = request_threaded_irq(key[i].irq, key_irq_func, 
            key_irq_thread_func, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, 
            "key", (void *)&key[i]);
        if (ret)
            goto err_request_threaded_irq;
    }
    
    major = register_chrdev(0, "key", &key_fops);
    if (major < 0)
        goto err_register_chrdev;
    
    key_class = class_create(THIS_MODULE, "key");
    if (IS_ERR(key_class))
        goto err_class_create;
    
    device_create(key_class, NULL, MKDEV(major, 0), NULL, "key");
    
    return 0;
    
err_class_create:
    unregister_chrdev(major, "key");
err_register_chrdev:
    for (i = 0; i < count; i++) {
        free_irq(key[i].irq, (void *)&key[i]);
    }
err_request_threaded_irq:
    for (i = 0; i < count; i++) {
        del_timer(&key[i].timer);
    }
    return -1;
}

static int key_drv_remove(struct platform_device *pdev) {
    int i;
    struct device_node *node = pdev->dev.of_node;
    int count = of_gpio_count(node);
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

    device_destroy(key_class, MKDEV(major, 0));
    class_destroy(key_class);
    unregister_chrdev(major, "key");
    for (i = 0; i < count; i++) {
        free_irq(key[i].irq, (void *)&key[i]);
        del_timer(&key[i].timer);
    }

    return 0;
}

static struct of_device_id key_device_ids[] = {
	{.compatible = "imx,keys"},
	{},
};

static struct platform_driver key_drv = {
    .driver = {
        .name = "key",
        .of_match_table = key_device_ids,
    },
    .probe = key_drv_probe,
    .remove = key_drv_remove,
};

module_platform_driver(key_drv);
MODULE_LICENSE("GPL");

