#ifndef ___TIME_H___
#define ___TIME_H___
#include <stdint.h>
#include "stm32f4xx.h"


void vTimeDelayInit(uint8_t SYSCLK);
void vTimeDelayUs(u32 nus);
void vTimeDelayMs(u32 nms);


#endif
