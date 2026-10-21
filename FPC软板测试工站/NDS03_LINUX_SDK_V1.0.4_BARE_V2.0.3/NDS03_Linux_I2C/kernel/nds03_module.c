/*!
 @file nds03_module.c
 @brief 
 @author lull
 @date 2025-06
 @copyright Copyright (c) 2025  Shenzhen Nephotonics  Semiconductor Technology Co., Ltd.
 @license BSD 3-Clause License
          This file is part of the Nephotonics sensor SDK.
          It is licensed under the BSD 3-Clause License.
          A copy of the license can be found in the project root directory, in the file named LICENSE.
 */
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/time.h>
#include <linux/platform_device.h>
#include <linux/version.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/atomic.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/timer.h>
#include <linux/delay.h>

#define DRIVER_VERSION		"1.0.4"

#define DEBUG 1
/* 缓冲区 */
#define NDS03_COMMS_CHUNK_SIZE  		1024
/* NDS03中断引脚，默认-1不使用中断 */
#define IRQ_GPIO -1
static int intr_gpio_num = IRQ_GPIO;

module_param(intr_gpio_num, int, 0000);
MODULE_PARM_DESC(intr_gpio_num, "select gpio number to use for nds03 interrupt");

#define NDS03_DRV_NAME		"tof_nds03"

#define NDS03_IOCTL_I2C_WRITE 		_IOWR('a',0x1, void*)
#define NDS03_IOCTL_I2C_READ			_IOWR('a',0x2, void*)
#define NDS03_IOCTL_WAIT_INTERRUPT	_IO('a',0x3)

struct nds03_i2c_transfer{
	uint16_t    addr;
	uint8_t   reg;
	uint8_t    *buf;
	uint16_t   len;
	
};
struct nds03_dev_t
{	struct device_node	*nd; /* 设备节点 */
};


static struct miscdevice nds03_miscdev;
static uint8_t * raw_data_buffer = NULL;

static wait_queue_head_t xshut;
/* 申请的中断号  */
static unsigned int nds03_irq_num;
/* 进入中断服务函数标志 */
static atomic_t irq_ready_flag;
static uint8_t is_i2c_driver_init = 0;
/* 字符设备注册标志*/
static uint8_t is_misc_registered = 0;
/* 中断注册标志 */
static uint8_t is_irq_registered = 0;
/* GPIO注册标志 */
uint16_t  is_gpio_taken = 0;
/* nds03设备信息 */
struct nds03_dev_t nds03_dev;



static struct i2c_client *nds03_i2c_client = NULL;


static const struct i2c_device_id nds03_i2c_id[] = {
	{NDS03_DRV_NAME, 0},
	{},
};

static const struct of_device_id nds03_of_match[] = {
	{
		.compatible = "nx,"NDS03_DRV_NAME,
		.data = NDS03_DRV_NAME,
	},
	{},
};

MODULE_DEVICE_TABLE(i2c, nds03_i2c_id);
MODULE_DEVICE_TABLE(of, nds03_of_match); 

static irqreturn_t nds03_intr_handler(int nds03_irq_num, void *dev_id)
{
	atomic_set(&irq_ready_flag, 1);
	wake_up_interruptible(&xshut);
	return IRQ_HANDLED;
}

static long nds03_ioctl(struct file *file,
		unsigned int cmd, unsigned long arg)
{
	struct i2c_msg i2c_message[2];
	int32_t ret = 0;
	struct nds03_i2c_transfer xfer = {0};
	switch (cmd) {
		case NDS03_IOCTL_WAIT_INTERRUPT:
			atomic_set(&irq_ready_flag, 0);
			ret = wait_event_interruptible(xshut, atomic_read(&irq_ready_flag) != 0);
			if (ret) {
				pr_err("wait_event_interruptible ret = %d", ret);
				atomic_set(&irq_ready_flag, 0);
				return -EINTR;
			}
			atomic_set(&irq_ready_flag, 0);
			break;
		case NDS03_IOCTL_I2C_WRITE:
			ret = copy_from_user(&xfer, (void __user *)arg, sizeof(struct nds03_i2c_transfer));
			if (ret) {
				pr_err("Error at %s(%d)\n", __func__, __LINE__);
				return -EINVAL;
			}
			raw_data_buffer[0] = xfer.reg;

			ret = copy_from_user(&raw_data_buffer[1], xfer.buf, xfer.len);
			if (ret) {
				pr_err("Error at %s(%d)\n", __func__, __LINE__);
				return -EINVAL;
			}
			i2c_message[0].addr = xfer.addr;  
			i2c_message[0].buf = raw_data_buffer;
			i2c_message[0].len = xfer.len + 1;
			i2c_message[0].flags = 0;
			ret = i2c_transfer(nds03_i2c_client->adapter, i2c_message, 1);
			if (ret != 1) {
				pr_err("Error %d at %s(%d)\n",ret,  __func__, __LINE__);
				return -EIO;
			}		
		break;
		case NDS03_IOCTL_I2C_READ:
			ret = copy_from_user(&xfer, (void __user *)arg, sizeof(struct nds03_i2c_transfer));
			if (ret) {
				pr_err("Error at %s(%d)\n", __func__, __LINE__);
				return -EINVAL;
			}
			// raw_data_buffer[0] = xfer.reg;
			i2c_message[0].addr = xfer.addr;  
			i2c_message[0].buf = &xfer.reg;
			i2c_message[0].len = 1;
			i2c_message[0].flags = 0;
	
			i2c_message[1].addr = xfer.addr;
			i2c_message[1].buf = raw_data_buffer;
			i2c_message[1].len = xfer.len;
			i2c_message[1].flags = 1;
			ret = i2c_transfer(nds03_i2c_client->adapter, i2c_message, 2);
			if (ret != 2) {
				pr_err("Error %d at %s(%d)\n",ret,  __func__, __LINE__);
				return -EIO;
			}		
			ret = copy_to_user(xfer.buf, raw_data_buffer, xfer.len);
			if (ret) {
				pr_err("Error at %s(%d)\n", __func__, __LINE__);
				return -EINVAL;
			}			
		break;
		default:
			return -EINVAL;

	}
	return 0;
}
/*!
 @brief 捕捉模组中断输入信号
 @param   gpio_number 选择的gpio口
 @param   name 命名
 @return int 
 */
static int gpio_input_init(int gpio_number, const char *name)
{
	int ret = 0;
	ret = gpio_request(gpio_number, name);
	if (ret) {
		pr_err("acquire gpio(%d)error", gpio_number);
		return ret;
	}
	ret = gpio_direction_input(gpio_number);
	if (ret) {
		gpio_free(gpio_number);
		pr_err("configure gpio_number(%d) as output error", gpio_number);
		return ret;
	}
	return ret;

}


static void gpio_releass(int gpio_number)
{
	gpio_free(gpio_number);
}

static int nds03_open(struct inode *inode, struct file *file)
{
	atomic_set(&irq_ready_flag, 0);  //清除中断标志
	return 0;
}

static int nds03_release(struct inode *inode, struct file *file)
{
	return 0;
}
static const struct file_operations nds03_ranging_fops = {
	.owner 			= THIS_MODULE,
	.unlocked_ioctl	= nds03_ioctl,
	.open 			= nds03_open,
	.release 		= nds03_release,
};


// static int nds03_probe(
// 			struct i2c_client *client, const struct i2c_device_id *id)
static int nds03_probe(struct i2c_client *client)
{
	int ret;
	u32	i2c_freq  = 0;
	nds03_i2c_client = client;
	raw_data_buffer = kzalloc(NDS03_COMMS_CHUNK_SIZE, GFP_DMA | GFP_KERNEL);
	if (raw_data_buffer == NULL)
		 return -ENOMEM;
	nds03_miscdev.minor = MISC_DYNAMIC_MINOR;
	nds03_miscdev.name = NDS03_DRV_NAME;
	nds03_miscdev.fops = &nds03_ranging_fops;
	nds03_miscdev.mode = 0444;

	ret = misc_register(&nds03_miscdev);
	if (ret) {
		pr_err("nds03 : misc device registering failed: %d\n", ret);
		return ret;
	}
	is_misc_registered = 1;
	pr_info("nds03:driver version %s\n", DRIVER_VERSION);
	if (intr_gpio_num == -1)
	{
		pr_err("nds03: not using interrupt pin\n");
		return 0;
	}

	of_property_read_u32(client->adapter->dev.of_node, "clock-frequency",
							&i2c_freq);
	pr_info("I2C bus number is %d, bus speed: %u Hz\n", client->adapter->nr,
			i2c_freq);

	ret = gpio_input_init(intr_gpio_num, "nds03_irq_gpio");

        if (ret != 0) {
		pr_err("acquire INTR GPIO(%d) error\n", intr_gpio_num);
	} else {
		is_gpio_taken = 1;

		nds03_irq_num = gpio_to_irq(intr_gpio_num);
		init_waitqueue_head(&xshut);
		ret = request_threaded_irq(nds03_irq_num,
					NULL,
					nds03_intr_handler,
					IRQF_TRIGGER_FALLING|IRQF_ONESHOT,
					"tof_sensor",
					NULL);

		if (ret) {
			pr_err("tof_nds03: Failed to Register IRQ handler,"
					" GPIO = %d, nds03_irq_num = %d\n",
					IRQ_GPIO, nds03_irq_num);
			kfree(raw_data_buffer);
			misc_deregister(&nds03_miscdev);
			is_misc_registered = 0;
			return -EPERM;
		} else {
			printk("tof_nds03: Registered IRQ handler, GPIO = %d, "
					"nds03_irq_num = %d\n",
					intr_gpio_num, nds03_irq_num);
			is_irq_registered = 1;
		}
	}
	
	return ret;
}

// int nds03_remove(struct i2c_client * client)
static void nds03_remove(struct i2c_client *client)
{

	if (raw_data_buffer)
		kfree(raw_data_buffer);
	// return 0;
}

static struct i2c_driver nds03_i2c_driver = {
	.driver = {
		.name = NDS03_DRV_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(nds03_of_match), 
	},
	.probe = nds03_probe,
	.remove = nds03_remove,
	.id_table = nds03_i2c_id,
};


static int __init nds03_module_init(void)
{
	int ret = 0;
	is_i2c_driver_init = 1;

	/* register as a i2c client device */
	ret = i2c_add_driver(&nds03_i2c_driver);

	if (ret) {
		i2c_del_driver(&nds03_i2c_driver);
		pr_err("tof_nds03: could not add i2c driver\n");
		return ret;
	}

	pr_debug("tof_nds03: module init\n");

	return ret;
}

static void __exit nds03_module_exit(void)
{

	if (is_misc_registered) {
		misc_deregister(&nds03_miscdev);
		is_misc_registered = 0;
	}

	if (is_i2c_driver_init) {
		i2c_del_driver(&nds03_i2c_driver);
		is_i2c_driver_init = 0;
	}

	if (is_irq_registered) {
		free_irq(nds03_irq_num, NULL);
	}

	if (is_gpio_taken == 1) {
		gpio_releass(intr_gpio_num);
		is_gpio_taken = 0;
	}

	pr_debug("tof_nds03 : exit\n");

}

module_init(nds03_module_init);
module_exit(nds03_module_exit);


MODULE_LICENSE("GPL");
MODULE_VERSION(DRIVER_VERSION);