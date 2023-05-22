#include "asm/uaccess.h"
#include "linux/err.h"
#include "linux/export.h"
#include "linux/leds.h"
#include "linux/printk.h"
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

typedef struct led_object {
    int major;
    struct class *class;
    struct gpio_desc *gdesc;
} led_object;

led_object led;

ssize_t led_read(struct file *file, char __user *buf, size_t size, loff_t *ppos) {
    int value;
    int ret;
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    value = gpiod_get_value(led.gdesc);
    printk("read value = %d\n", value);
    ret = copy_to_user(buf, &value, 4);
    return 0;
}

ssize_t led_write(struct file *file, const char __user *buf, size_t size, loff_t *ppos) {
    int ret;
    char status;
    
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    // printk("write value = %c\n", *buf);
    ret = copy_from_user(&status, buf, 1);
    gpiod_set_value(led.gdesc, status);
    
    return 1;
}

int led_open(struct inode *node, struct file *file) {
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

int led_release(struct inode *node, struct file *file) {
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .release = led_release,
    .write = led_write,
    .read = led_read,
    .llseek = no_llseek,
};

int led_drv_probe(struct platform_device *pdev) {
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    led.gdesc = devm_gpiod_get_index(&pdev->dev, "leds", 0, GPIOD_OUT_LOW);
    if (IS_ERR(led.gdesc)) 
        goto err_devm_gpiod_get_index; 
    
    led.major = register_chrdev(0, "led", &led_fops);
    if (led.major < 0)
        goto err_register_chrdev;
    
    led.class = class_create(THIS_MODULE, "led");
    if (IS_ERR(led.class))
        goto err_class_create;
    
    device_create(led.class, NULL, MKDEV(led.major, 0), NULL, "led"); 
    gpiod_direction_output(led.gdesc, 0);
    
    return 0;

err_class_create:
err_register_chrdev:
err_devm_gpiod_get_index:
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    return -1;
}

int led_drv_remove(struct platform_device *pdev) {
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    device_destroy(led.class, MKDEV(led.major, 0));
    class_destroy(led.class);
    unregister_chrdev(led.major, "led");
    return 0;
}

static struct of_device_id led_device_ids[] = {
	{.compatible = "imx-gpio-leds"},
	{},
};

struct platform_driver led_drv = {
    .driver = {
        .name = "led",
        .of_match_table = led_device_ids,
    },
    .probe = led_drv_probe,
    .remove = led_drv_remove,
};

module_platform_driver(led_drv);
MODULE_LICENSE("GPL");

