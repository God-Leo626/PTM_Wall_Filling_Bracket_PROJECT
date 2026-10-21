/*!
 @file nds03_platform.c
 @brief 
 @author lull
 @date 2025-06
 @copyright Copyright (c) 2025  Shenzhen Nephotonics  Semiconductor Technology Co., Ltd.
 @license BSD 3-Clause License
          This file is part of the Nephotonics sensor SDK.
          It is licensed under the BSD 3-Clause License.
          A copy of the license can be found in the project root directory, in the file named LICENSE.
 */
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <time.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include "nds03_platform.h"

#define NDS03_COMMS_CHUNK_SIZE  1024

#define NDS03_ERROR_COMMS		-1


#define NDS03_I2C_DEV_PATH 		"/dev/i2c-1"
#define NDS03_I2C_DEV_KERNEL 	"/dev/tof_nds03"

#define LOG 	printf


#define NDS03_IOCTL_I2C_WRITE 		_IOWR('a',0x1, void*)
#define NDS03_IOCTL_I2C_READ			_IOWR('a',0x2, void*)
#define NDS03_IOCTL_WAIT_INTERRUPT	_IO('a',0x3)

#ifdef TOF_NDS03_KERNEL

struct nds03_i2c_transfer{
    uint16_t    addr;
    uint8_t    reg;
    uint8_t    *buf;
    uint16_t   len;
};

#else

static uint8_t i2c_buffer[NDS03_COMMS_CHUNK_SIZE];
#endif

int8_t nds03_platform_init(NDS03_Platform_t *pDev, void *p_user_parameters)
{
    gpio_dev_init(&pDev->xshut, 20);

#ifdef TOF_NDS03_KERNEL
    pDev->fd = open(NDS03_I2C_DEV_KERNEL, O_RDONLY);
    if (pDev->fd == -1) {
        LOG("Failed to open %s\n",(char *)NDS03_I2C_DEV_KERNEL);
        return NDS03_ERROR_COMMS;
    }
#else
        pDev->fd = open(NDS03_I2C_DEV_PATH, O_RDONLY);
	if (pDev->fd == -1) {
		LOG("Failed to open %s\n",(char*)NDS03_I2C_DEV_PATH);
		return NDS03_ERROR_COMMS;
	}
	// if (ioctl(pDev->fd, I2C_SLAVE, pDev->i2c_dev_addr) < 0) {
	// 	LOG("Could not speak to the device on the i2c bus\n");
	// 	return NDS03_ERROR_COMMS;
	// }
#endif

    LOG("Opened	 TOF Dev = %d\n", pDev->fd);
    return 0;
}

int8_t nds03_platform_uninit(NDS03_Platform_t *pDev, void *p_user_parameters)
{
    close(pDev->fd);
	gpio_dev_uninit(&pDev->xshut);
    return 0;
}



int8_t nds03_i2c_read_nbytes(NDS03_Platform_t *pDev, uint8_t i2c_raddr, uint8_t *i2c_rdata, uint16_t len)
{

#ifdef TOF_NDS03_KERNEL
    struct nds03_i2c_transfer xfer;
    xfer.addr = pDev->i2c_dev_addr;
    xfer.reg = i2c_raddr;
    xfer.buf = i2c_rdata;
    xfer.len = len;
    if (ioctl(pDev->fd, NDS03_IOCTL_I2C_READ, &xfer) < 0)
        return NDS03_ERROR_COMMS;

#else

	struct i2c_msg messages[2];
	struct i2c_rdwr_ioctl_data packets;

	i2c_buffer[0] = i2c_raddr;


	messages[0].addr = pDev->i2c_dev_addr;
	messages[0].flags = 0;
	messages[0].len = 1;
	messages[0].buf = i2c_buffer;

	messages[1].addr = pDev->i2c_dev_addr;
	messages[1].flags = I2C_M_RD;
	messages[1].len = len;
	messages[1].buf = i2c_rdata;


	packets.msgs = &messages[0];
	packets.nmsgs = 2;

	if (ioctl(pDev->fd, I2C_RDWR, &packets) < 0)
	{
		LOG("Writing regaddr failed... : %04x\n", i2c_raddr);
		return NDS03_ERROR_COMMS;
	}
#endif

    return 0;
}


int8_t nds03_i2c_write_nbytes(NDS03_Platform_t *pDev, uint8_t i2c_waddr, uint8_t *i2c_wdata, uint16_t len)
{


#ifdef TOF_NDS03_KERNEL

    struct nds03_i2c_transfer xfer;

    xfer.addr = pDev->i2c_dev_addr;
    xfer.reg = i2c_waddr;
    xfer.buf = i2c_wdata;
    xfer.len = len;

    if (ioctl(pDev->fd, NDS03_IOCTL_I2C_WRITE, &xfer) < 0)
        return NDS03_ERROR_COMMS;
#else
	struct i2c_msg messages[2];
	struct i2c_rdwr_ioctl_data packets;

	memcpy(&i2c_buffer[1], i2c_wdata, len);

	i2c_buffer[0] = i2c_waddr;

	messages[0].addr = pDev->i2c_dev_addr;
	messages[0].flags = 0;
	messages[0].len = len + 1;
	messages[0].buf = i2c_buffer;

	packets.msgs = messages;
	packets.nmsgs = 1;
	if (ioctl(pDev->fd, I2C_RDWR, &packets) < 0)
		return NDS03_ERROR_COMMS;
#endif
    return 0;
}

#ifdef TOF_NDS03_KERNEL
int32_t WaitForNDS03_Interrupt(NDS03_Platform_t *pDev)
{
    int32_t ret;
    ret = ioctl(pDev->fd, NDS03_IOCTL_WAIT_INTERRUPT, 0);
    return ret;
}
#endif



int8_t nds03_delay_1ms(NDS03_Platform_t *pDev, uint32_t wait_ms)
{
    nds03_delay_10us(pDev, 100*wait_ms);
	return 0;
}

int8_t nds03_delay_10us(NDS03_Platform_t *pDev, uint32_t wait_10us)
{
	usleep(wait_10us * 10);
	return 0;
}

int8_t nds03_set_xshut_pin_level(NDS03_Platform_t *pDev, int8_t level)
{
    if(level) {
        gpio_set_value(pDev->xshut, 1);
	}
    else {
        gpio_set_value(pDev->xshut, 0);
	}

	return 0;
}

int8_t nds03_i2c_get_clock_frequency(NDS03_Platform_t *pDev, uint32_t *clock_frequency)
{
    return 0;
}

int8_t nds03_i2c_set_clock_frequency(NDS03_Platform_t *pDev, uint32_t clock_frequency)
{
    return 0;
}

int8_t nds03_get_system_clk_ms(NDS03_Platform_t *pDev, int32_t *time_ms)
{
    return 0;
}
