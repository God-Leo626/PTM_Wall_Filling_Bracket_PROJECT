/*
 * @Author: “HP “1357243695@qq.com”
 * @Date: 2023-08-10 10:57:35
 * @LastEditors: “HP “1357243695@qq.com”
 * @LastEditTime: 2023-08-11 17:05:12
 * @FilePath: \project\APP\usart\usart.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __USART_H__
#define __USART_H__

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include <stdio.h>


#define USAR  115200

void usart1_init(u32 bound);
void usart2_init(u32 bound);
void usart3_init(u32 bound);

void vUsar1tDisable(void);
void vUsar2tDisable(void);
void uart3_disable(void);

void uart_extern_burn_and_at_uart_tx_buf(uint8_t * p_tx_buf, uint16_t len );

#endif


