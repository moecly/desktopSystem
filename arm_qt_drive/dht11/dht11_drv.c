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

typedef struct dht11_data {
    unsigned int gpio;
    struct gpio_desc *gpiod;
} dht11_data;

static dht11_data dht11;
static int major;
static struct class *dht11_class;

static int dht11_read_data(unsigned *read_data) {
    int i = 0, j = 0;
    int data[5];
    int cnt = 0;
    // int temp_tmp = 0, humidity_tmp = 0;
    unsigned int ret_data = 0;
    int value = 0, check = 0;
    
    /* 
     * 步骤二:
     *  微处理器的 I/O 设置为输出同时输出低电平，且低电平保持时间不能小于 18ms（最大不得超过 30ms），
     *  然后微处理器的 I/O 设置为输入状态，由于上拉电阻，微处理器的 I/O 即 DHT11 的 DATA 数据线也随之变
     *  高，等待 DHT11 作出回答信号，发送信号如图所示：
     */
    gpiod_direction_output(dht11.gpiod, 0);
    mdelay(20);
    gpiod_direction_output(dht11.gpiod, 1);
    udelay(30);
    gpiod_direction_input(dht11.gpiod);
    
    /* 
     * 步骤三:
     *  DHT11 的 DATA 引脚检测到外部信号有低电平时，等待外部信号低电平结束，延迟后 DHT11 的 DATA
     *  引脚处于输出状态，输出 83 微秒的低电平作为应答信号，紧接着输出 87 微秒的高电平通知外设准备接
     *  收数据，微处理器的 I/O 此时处于输入状态，检测到 I/O 有低电平（DHT11 回应信号）后，等待 87 微秒
     *  的高电平后的数据接收，发送信号如图所示：
     */
    while (!gpiod_get_value(dht11.gpiod) && cnt < 100) {
        cnt++;
        udelay(1);
    }
    
    if (cnt >= 100) {
        return -1;
    }
    
    /* 
     * 步骤四:
     *  由 DHT11 的 DATA 引脚输出 40 位数据，微处理器根据 I/O 电平的变化接收 40 位数据，位数据“0”
     *  的格式为： 54 微秒的低电平和 23-27 微秒的高电平，位数据“1”的格式为： 54 微秒的低电平加 68-74
     *  微秒的高电平。位数据“0”、“1”格式信号如图所示：
     */
    for (i = 0; i < 5; i++) {
        data[i] = 0;
        for (j = 0; j < 8; j++) {
            data[i] <<= 1;

            cnt = 0;
            while (gpiod_get_value(dht11.gpiod) && cnt < 100) {
                cnt++;
                udelay(1);
            }

            if (cnt >= 100) {
                return -1;
            }

            cnt = 0;
            while (!gpiod_get_value(dht11.gpiod) && cnt < 100) {
                cnt++;
                udelay(1);
            }

            if (cnt >= 100) {
                return -1;
            }

            udelay(40);
            data[i] |= gpiod_get_value(dht11.gpiod);
        }     
    }
    
    /* 
     * 结束信号：
     *  DHT11 的 DATA 引脚输出 40 位数据后，继续输出低电平 54 微秒后转为输入状态，由于上拉电阻随
     *  之变为高电平。但 DHT11 内部重测环境温湿度数据，并记录数据，等待外部信号的到来。
     */
     
    /* 
     * 检验数据：
     * 0011 0101 0000 0000 0001 1000 0000 0100 0101 0001
     *  湿度高 8 位 湿度低 8 位 温度高 8 位 温度低 8 位 校验位
     *  计算：
     *  0011 0101+0000 0000+0001 1000+0000 0100= 0101 0001
     */
    for (i = 0; i < 4; i++) {
        value += data[i];
        ret_data <<= 8;
        ret_data |= data[i];
    }

    check = data[4];
    if (value != check) {
        return -1;
    }
    
    *read_data = ret_data;
    // temp_tmp = (data[2] << 8) | data[3];
    // humidity_tmp = (data[0] << 8) | data[1];
    
    // *temp = temp_tmp;
    // *humidity = humidity_tmp;

    return 0;
}

static ssize_t dht11_read(struct file *file, char __user *buf, size_t size, loff_t *ppos) {
    int ret = 0;
    unsigned int value = 0;

    ret = dht11_read_data(&value);
    if (ret)
        return -1;
    
    ret = copy_to_user(buf, &value, 4);
    return 4;
}

static ssize_t dht11_write(struct file *file, const char __user *buf, size_t size, loff_t *ppos) {
    return 0;
}

static int dht11_open(struct inode *node, struct file *file) {
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    /* 
     * 步骤一:
     *  DHT11 上电后（DHT11 上电后要等待 1S 以越过不稳定状态在此期间不能发送任何指令），测试环境
     *  温湿度数据，并记录数据，同时 DHT11 的 DATA 数据线由上拉电阻拉高一直保持高电平；此时 DHT11 的
     *  DATA 引脚处于输入状态，时刻检测外部信号。
     */
    gpiod_direction_input(dht11.gpiod);
    mdelay(1000);
    return 0;
}

static int dht11_release(struct inode *node, struct file *file) {
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    gpiod_direction_output(dht11.gpiod, 1);
    return 0;
}

static int dht11_fasync(int fd, struct file *file, int on) {
    return 0;
}

static struct file_operations dht11_fops = {
    .owner = THIS_MODULE,
    .open = dht11_open,
    .release = dht11_release,
    .write = dht11_write,
    .read = dht11_read,
    .fasync = dht11_fasync,
    .llseek = no_llseek,
};

static int dht11_drv_probe(struct platform_device *pdev) {
    int ret = 0;
    struct device_node *node = pdev->dev.of_node;
    
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

    dht11.gpio = of_get_named_gpio(node, "dht11-gpios", 0);
    ret = devm_gpio_request(&pdev->dev, dht11.gpio, "dht11-gpio");
    dht11.gpiod = gpio_to_desc(dht11.gpio);
    if (IS_ERR(dht11.gpiod))
        goto err_gpio_to_desc;
    
    if (ret)
        goto err_devm_gpio_request;
    
    major = register_chrdev(0, "dht11", &dht11_fops);
    if (major < 0) 
        goto err_register_chrdev;
    
    dht11_class = class_create(THIS_MODULE, "dht11");
    if (IS_ERR(dht11_class))
        goto err_class_create;
        
    device_create(dht11_class, NULL, MKDEV(major, 0), NULL, "dht11");
    
    return 0;

err_class_create:
    unregister_chrdev(major, "dht11");
err_register_chrdev:
err_devm_gpio_request:
err_gpio_to_desc:
    return -1;
}

static int dht11_drv_remove(struct platform_device *pdev) {
    device_destroy(dht11_class, MKDEV(major, 0));
    class_destroy(dht11_class);
    unregister_chrdev(major, "dht11");
    return 0;
}

static struct of_device_id dht11_device_ids[] = {
	{.compatible = "imx,dht11"},
	{},
};

static struct platform_driver dht11_drv = {
    .driver = {
        .name = "dht11",
        .of_match_table = dht11_device_ids,
    },
    .probe = dht11_drv_probe,
    .remove = dht11_drv_remove,
};

module_platform_driver(dht11_drv);
MODULE_LICENSE("GPL");

