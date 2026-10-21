/*!
 @file lib-gpio.h
 @brief 
 @author lull
 @date 2025-06
 @copyright Copyright (c) 2025  Shenzhen Nephotonics  Semiconductor Technology Co., Ltd.
 @license BSD 3-Clause License
          This file is part of the Nephotonics sensor SDK.
          It is licensed under the BSD 3-Clause License.
          A copy of the license can be found in the project root directory, in the file named LICENSE.
 */
#ifndef _LIB_GPIO_H_
#define _LIB_GPIO_H_

#include <stdint.h>

#define LOW             0x00
#define HIGH            0x01
#define INPUT           0x00
#define OUTPUT          0x10

typedef struct gpio_dev_t{
    int32_t pin;
    int32_t direction_fd;
    int32_t value_fd;
}gpio_dev_t;


int32_t gpio_dev_init(gpio_dev_t *gpio, int32_t pin);
int32_t gpio_dev_uninit(gpio_dev_t *gpio);

int32_t gpio_set_input(gpio_dev_t gpio);
int32_t gpio_set_output(gpio_dev_t gpio);

int32_t gpio_set_value(gpio_dev_t gpio, int32_t value);
int32_t gpio_get_value(gpio_dev_t gpio);

#endif
