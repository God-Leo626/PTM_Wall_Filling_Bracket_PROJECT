/*
 * @Author: “HP “1357243695@qq.com”
 * @Date: 2025-02-08 10:39:29
 * @LastEditors: “HP “1357243695@qq.com”
 * @LastEditTime: 2025-04-09 10:54:30
 * @FilePath: \镭雕\APP\TIM\timer.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "user.h"

volatile uint32_t  time_system_now_time;

//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数5000次为500ms  
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void TIM3_IRQHandler(void)
{
	static uint32_t	time_1s;
	static uint32_t	time_100ms;
	static uint32_t	time_50ms;

	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		// 系统时钟自增加
		time_system_now_time++;

		if(	time_50ms++ ==  49	)
		{
			time_50ms = 0;
//			key_extern_monitor_handle(); 
		}

		// 按键事件
		key_extern_timer_1ms_handle();
		
		if(	time_100ms++ ==  99	)
		{
			// system 100ms EVENT
			time_100ms = 0;
		
			 sys_work_info |= SYSTEM_FLAG_100MS_EVENT;
			 sys_work_info |=SYSTEM_FLAG_100MS_EVENT_USB;
		}
		// if(	time_1s++ ==  999	)
		// {
		// 	// system 100ms EVENT
		// 	time_1s = 0;
		// 	TRACE_WP("RSSI = %d\r\n",system_run_ram.usb_seng_rssi);
		// }
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}




uint32_t vTimeGetSysTime(void)
{
	return time_system_now_time;
}