#ifndef __ADC_H
#define __ADC_H

#include "user.h"

void Adc_Init(); 				//ADC通道初始化
void Get_Adc() ; 
void Get_Adc_Average(u8 times, u8 offset);//得到某个通道给定次数采样的平均值  
void get_DP_and_DM_voltage_event(float threshold, float redundancy);

#endif 















