/*
 * @Author: “HP “1357243695@qq.com”
 * @Date: 2023-08-07 15:13:11
 * @LastEditors: “HP “1357243695@qq.com”
 * @LastEditTime: 2023-08-09 14:53:03
 * @FilePath: \project\APP\TIM\timer.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _TIMER_H
#define _TIMER_H
#include "stm32f4xx.h"

extern volatile uint32_t  time_system_now_time;

void TIM3_Int_Init(u16 arr,u16 psc);
uint32_t vTimeGetSysTime(void);
#endif
