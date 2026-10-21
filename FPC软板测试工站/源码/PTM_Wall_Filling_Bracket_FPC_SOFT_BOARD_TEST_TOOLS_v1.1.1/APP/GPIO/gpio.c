/*
 * @Author: “HP “1357243695@qq.com”
 * @Date: 2023-08-07 17:07:04
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-11-28 14:46:56
 * @FilePath: \project\APP\GPIO\gpio.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/**********************************************************************************
*Copyright (c) 2019, Sonoff.ltd
*All rights reserved.
*
* File name			:	gpio
* Summary Content		:	
* Document			:	None
* Notes				:	

* Version				:	V1.0
*
* Modification date		Ver		       	Name		 		Modify 
*  ----------------------------------------------------------------------------
* 20190703		       V1.0 			 Alex		       	Establish
************************************************************************************/

#include "gpio.h"


/************************************************************************************************
* Functional 	:	System GPIO Initialization
* Input		:   	None
* Output		:	None
* Return 		: 	None
* Notes		:
* Modification	:   
* Modification date		Ver		       	Name		 		Modify 
*  ----------------------------------------------------------------------------
* 20190703		       V1.0 			 Alex		       	Establish
**************************************************************************************************/
void gpio_extern_system_driver_init(void)
{
	unsigned int i;
	GPIO_InitTypeDef GPIO_InitStructure;

	//GPIO SYSTEM CLOCK
	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |
							RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD |
							RCC_AHB1Periph_GPIOE , ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;   //PC12--->EXTERNAL 5V   PC13--->WIFI_POWER_CTRL
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);		//初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  //PD6--->EXTERNAL 3V3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);		//初始化

	//GPIOE6初始化设置      //uart_switch_ctrl
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//不拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);		//初始化

	GPIO_InitStructure.GPIO_Pin = CHENGKONG_KEY_PIN;  //PB1--->治具下压开关
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(CHENGKONG_KEY_GPIO, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  //非保持开关
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  //绿色LED灯
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  //DUT KEY
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  //DUT BULE LED
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  //DUT RED LED
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  //测距传感器XUHUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  //测距传感器INT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/************************************************************************************************
* Functional 	:	LED GPIO operation  Flash
* Input		:   	None
* Output		:	None
* Return 		: 	None
* Notes		:
* Modification	:   
* Modification date		Ver		       	Name		 		Modify 
*  ----------------------------------------------------------------------------
* 20190703		       V1.0 			 Alex		       	Establish
**************************************************************************************************/
void gpio_extern_led_system_flash(void)
{
	static uint8_t g_led_on;
	
	if (0 == g_led_on)
	{
		g_led_on = 1;
		GPIO_SetBits( EXTERNAL_SYS_LEN );
	}
	else
	{
		g_led_on = 0;
		GPIO_ResetBits( EXTERNAL_SYS_LEN );
	}
}






