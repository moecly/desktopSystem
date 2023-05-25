#include "asm-generic/fcntl.h"
#include "asm-generic/gpio.h"
#include "asm/delay.h"
#include "asm/gpio.h"
#include "asm/uaccess.h"
#include "linux/delay.h"
#include "linux/err.h"
#include "linux/export.h"
#include "linux/gpio.h"
#include "linux/interrupt.h"
#include "linux/irq.h"
#include "linux/irqreturn.h"
#include "linux/jiffies.h"
#include "linux/of_gpio.h"
#include "linux/pinctrl/consumer.h"
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

typedef struct hc_sr501_data {
    unsigned int gpio;
    struct gpio_desc *gpiod;
    unsigned int irq;
    unsigned int value;
} hc_sr501_data;

static hc_sr501_data hc_sr501;
static int major;
static struct class *hc_sr501_class;
static DECLARE_WAIT_QUEUE_HEAD(hc_sr501_wait);

static ssize_t hc_sr501_read(struct file *file, char __user *buf, size_t size, loff_t *ppos) {
    int ret = 0;
    if (hc_sr501.value == 0 && file->f_flags & O_NONBLOCK) {
        return 0;
    }
    
    wait_event_interruptible(hc_sr501_wait, hc_sr501.value);
    ret = copy_to_user(buf, &hc_sr501.value, 4);
    hc_sr501.value = 0;
    return 4;
}

static ssize_t hc_sr501_write(struct file *file, const char __user *buf, size_t size, loff_t *ppos) {
    return 0;
}

static int hc_sr501_open(struct inode *node, struct file *file) {
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    gpiod_direction_input(hc_sr501.gpiod);
    return 0;
}

static int hc_sr501_release(struct inode *node, struct file *file) {
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

static int hc_sr501_fasync(int fd, struct file *file, int on) {
    return 0;
}

static struct file_operations hc_sr501_fops = {
    .owner = THIS_MODULE,
    .open = hc_sr501_open,
    .release = hc_sr501_release,
    .write = hc_sr501_write,
    .read = hc_sr501_read,
    .fasync = hc_sr501_fasync,
    .llseek = no_llseek,
};

static irqreturn_t hc_sr501_thread_irq_handle(int irq, void *data) {
    int value = 0;
    hc_sr501.value = hc_sr501.gpio << 16;
    wake_up_interruptible(&hc_sr501_wait);
    value = gpiod_get_value(hc_sr501.gpiod);
    hc_sr501.value |= value;
    return IRQ_HANDLED;
}

static irqreturn_t hc_sr501_irq_handle(int irq, void *data) {
    return IRQ_WAKE_THREAD;
}

static int hc_sr501_drv_probe(struct platform_device *pdev) {
    int ret = 0;
    struct device_node *node = pdev->dev.of_node;
    
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

    hc_sr501.gpio = of_get_named_gpio(node, "hc_sr501-gpios", 0);
    if (!gpio_is_valid(hc_sr501.gpio))
        goto err_of_get_named_gpio;
    
    ret = devm_gpio_request(&pdev->dev, hc_sr501.gpio, "hc_sr501-gpio");
    if (ret)
        goto err_devm_gpio_request;

    hc_sr501.gpiod = gpio_to_desc(hc_sr501.gpio);
    if (IS_ERR(hc_sr501.gpiod))
        goto err_gpio_to_desc;
    
    hc_sr501.irq = gpiod_to_irq(hc_sr501.gpiod);
    ret = devm_request_threaded_irq(&pdev->dev, hc_sr501.irq, hc_sr501_irq_handle,\
        hc_sr501_thread_irq_handle, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,\
        "hc_sr501", (void*) &hc_sr501);
    
    major = register_chrdev(0, "hc_sr501", &hc_sr501_fops);
    if (major < 0) 
        goto err_register_chrdev;
    
    hc_sr501_class = class_create(THIS_MODULE, "hc_sr501");
    if (IS_ERR(hc_sr501_class))
        goto err_class_create;
        
    device_create(hc_sr501_class, NULL, MKDEV(major, 0), NULL, "hc_sr501");

    return 0;

err_class_create:
    unregister_chrdev(major, "hc_sr501");
err_register_chrdev:
err_devm_gpio_request:
err_gpio_to_desc:
err_of_get_named_gpio:
    return -1;
}

static int hc_sr501_drv_remove(struct platform_device *pdev) {
    device_destroy(hc_sr501_class, MKDEV(major, 0));
    class_destroy(hc_sr501_class);
    unregister_chrdev(major, "hc_sr501");
    return 0;
}

static struct of_device_id hc_sr501_device_ids[] = {
	{.compatible = "imx,hc_sr501"},
	{},
};

static struct platform_driver hc_sr501_drv = {
    .driver = {
        .name = "hc_sr501",
        .of_match_table = hc_sr501_device_ids,
    },
    .probe = hc_sr501_drv_probe,
    .remove = hc_sr501_drv_remove,
};

module_platform_driver(hc_sr501_drv);
MODULE_LICENSE("GPL");

