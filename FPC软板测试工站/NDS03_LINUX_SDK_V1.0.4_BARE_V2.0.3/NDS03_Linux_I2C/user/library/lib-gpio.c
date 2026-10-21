/*!
 @file lib-gpio.c
 @brief 
 @author lull
 @date 2025-06
 @copyright Copyright (c) 2025  Shenzhen Nephotonics  Semiconductor Technology Co., Ltd.
 @license BSD 3-Clause License
          This file is part of the Nephotonics sensor SDK.
          It is licensed under the BSD 3-Clause License.
          A copy of the license can be found in the project root directory, in the file named LICENSE.
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "lib-gpio.h"

static int32_t gpio_dev_register(gpio_dev_t *gpio);
static int32_t gpio_dev_unregister(gpio_dev_t *gpio);

int32_t gpio_dev_init(gpio_dev_t *gpio, int32_t pin)
{
    int32_t ret;
    gpio->pin = pin;
    ret = gpio_dev_register(gpio);
    if(ret != 0)
    {   printf("gpio register fail\n");
        return -1;
    }
    printf("Export GPIO num: %d, dirfd: %d, valfd: %d\n", pin, gpio->direction_fd, gpio->value_fd);
    gpio_set_output(*gpio);
    gpio_set_value(*gpio, 0x01);
    return 0;
}

int32_t gpio_dev_uninit(gpio_dev_t *gpio)
{
    gpio_dev_unregister(gpio);
    return 0;
}

int32_t gpio_set_input(gpio_dev_t gpio)
{
    write(gpio.direction_fd, "in", 2);
    return 0;
}

int32_t gpio_get_value(gpio_dev_t gpio)
{
    char get_value = 0;
    lseek(gpio.value_fd, 0L, SEEK_SET);
    read( gpio.value_fd ,&get_value , 1);
    return (get_value - '0');
}

int32_t gpio_set_output(gpio_dev_t gpio)
{
    write(gpio.direction_fd, "out", 3);
    return 0;
}

int32_t gpio_set_value(gpio_dev_t gpio, int32_t value)
{
    char level = '0';
    if(value == HIGH)
        level = '1';
    write(gpio.value_fd, &level, 1);

    return 0;
}
static int32_t unexport_fd = -1;
static int32_t export_fd = -1;

static int32_t gpio_dev_register(gpio_dev_t *gpio)
{
    // uint32_t cnt =10;
    char path[256] = {0};
    char chpin[8] = {0};
    if(export_fd == -1)
    {
        export_fd = open("/sys/class/gpio/export", O_WRONLY|O_TRUNC);
        if (export_fd < 0)
        {
            printf("open /sys/class/gpio/export fail!\n");
            return -1;
        }
    }
    sprintf(path, "/sys/class/gpio/gpio%d", gpio->pin);
    if(access(path, F_OK) == -1)
    {
        sprintf(chpin, "%d", gpio->pin);
        if(write(export_fd, chpin, strlen(chpin)) < 0)
        {
            printf("write %s fail!\n", path);
            return -1;
        }
    }
    bzero(path, sizeof(path));
    sprintf(path, "/sys/class/gpio/gpio%d/value" , gpio->pin);
    gpio->value_fd = open(path, O_RDWR|O_TRUNC);
    if (gpio->value_fd < 0)
    {
        printf("open gpio%d.value fail!\n", gpio->pin);
        return -1;
    }
    bzero(path, sizeof(path));
    sprintf(path, "/sys/class/gpio/gpio%d/direction" , gpio->pin);
    gpio->direction_fd = open(path, O_RDWR|O_TRUNC);
    if (gpio->direction_fd < 0){
        printf("open gpio%d.direction fail!\n", gpio->pin);
        return -1;
    }
    return 0;

}

static int32_t gpio_dev_unregister(gpio_dev_t *gpio)
{ 
    char chpin[8] = {0};
    if (unexport_fd < 0)
        unexport_fd = open("/sys/class/gpio/unexport", O_WRONLY|O_TRUNC);
    if (unexport_fd < 0)
    {
        printf("open /sys/class/gpio/unexport fail!\n");
        return -1;
    }

    close(gpio->direction_fd);
    close(gpio->value_fd);
    sprintf(chpin, "%d", gpio->pin);                             
    if(write(unexport_fd, chpin, strlen(chpin)) == -1)
    {
        printf("write unexport fail of %d!\n", gpio->pin);
        return -1;
    }
  
    return 0;
}
