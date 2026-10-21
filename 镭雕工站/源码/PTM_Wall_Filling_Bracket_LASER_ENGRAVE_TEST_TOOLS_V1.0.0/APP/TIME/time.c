/*
 * @Author: “HP “1357243695@qq.com”
 * @Date: 2023-08-04 09:51:34
 * @LastEditors: “HP “1357243695@qq.com”
 * @LastEditTime: 2025-02-25 14:00:20
 * @FilePath: \project\APP\TIME\time.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include "time.h"
#include "user.h"
static u8  fac_us=0;							//us延时倍乘数			   
static u16 fac_ms=0;	

void vTimeDelayInit(uint8_t SYSCLK)
{
	uint32_t reload;
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); 
	fac_us=SYSCLK;							//不论是否使用OS,fac_us都需要使用
	reload=SYSCLK;							//每秒钟的计数次数 单位为M	   
	reload*=1000;		            //根据configTICK_RATE_HZ设定溢出时间
											//reload为24位寄存器,最大值:16777216,在168M下,约合0.0998s左右	
	fac_ms=1;			//代表OS可以延时的最少单位	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//开启SYSTICK中断
	SysTick->LOAD=reload; 					//每1/configTICK_RATE_HZ断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //开启SYSTICK     
}			



void vTimeDelayUs(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks=nus*fac_us; 						//需要的节拍数 
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};										    
}  


void vTimeDelayMs(u32 nms)
{	
	u32 i;
	for(i=0;i<nms;i++) 
	{
		vTimeDelayUs(1000);
	}
}


void vTimeJustDelayMs(u32 nms)
{	
	u32 i;
	for(i=0;(i<nms) && (system_run_ram.wait_chip_work_mode == 2);i++) 
	{
		vTimeDelayUs(1000);
	}
}

