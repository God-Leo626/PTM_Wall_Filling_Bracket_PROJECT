/**********************************************************************************
*Copyright (c) 2019, Sonoff.ltd
*All rights reserved.
*
* File name			:	Sys  init 
* Summary Content		:	
* Document			:	None
* Notes				:	

* Version				:	V1.0
*
* Modification date		Ver		       	Name		 		Modify 
*  ----------------------------------------------------------------------------
* 20190703		       V1.0 			 Alex		       	Establish
************************************************************************************/

#include "user.h"


USB_OTG_CORE_HANDLE   USB_OTG_dev;
volatile uint32_t sys_work_info;
SYSTEM_RUN_RAM	system_run_ram;

/************************************************************************************************
* Functional 	:	System reset
* Input		:   	None
* Output		:	None
* Return 		: 	None
* Notes		:
* Modification	:   
* Modification date		Ver		       	Name		 		Modify 
*  ----------------------------------------------------------------------------
* 20190703		       V1.0 			 Alex		       	Establish
**************************************************************************************************/
void init_extern_system_reset()
{
	uint32_t	i;

	i = 0;
	TRACE_WP("[init]init_extern_system_reset start dealy...\r\n");
	while(1)
	{
		i++;
		vTimeDelayUs(100);
		if ( i>5000 )
		{
			// delay for 1500
			TRACE_WP("[init]init_extern_system_reset start...\r\n");
			
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
			NVIC_SystemReset();

			// Upgrade Successful, Reset Loader
			while(1);	
		}
	}
}


/************************************************************************************************
* Functional 	:	Get the unique ID of the device   Chip ID
* Input		:   	None
* Output		:	None
* Return 		: 	None
* Notes		:
* Modification	:   
* Modification date		Ver		       	Name		 		Modify 
*  ----------------------------------------------------------------------------
* 20190703		       V1.0 			 Alex		       	Establish
**************************************************************************************************/
uint32_t	init_extern_system_get_lock_id(void)
{
	uint32_t	cpu_id[3];
	uint32_t	lock_code;

	// Get CPU unique ID
	cpu_id[0] = *(vu32*)(0x1fff7a10);
	cpu_id[1] = *(vu32*)(0x1FFF7A14);
	cpu_id[2] = *(vu32*)(0x1FFF7A18);
	
	// Encryption algorithm, very simple encryption algorithm
	lock_code = ( cpu_id[0]>>1 )+( cpu_id[1]>>2 )+( cpu_id[2]>>3 );
	
	return lock_code;
	
} 








/************************************************************************************************
* Functional 	:	System Initialization
* Input		:   	None
* Output		:	None
* Return 		: 	None
* Notes		:
* Modification	:   
* Modification date		Ver		       	Name		 		Modify 
*  ----------------------------------------------------------------------------
* 20190703		       V1.0 			 Alex		       	Establish
**************************************************************************************************/
void init_extern_system_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	gpio_extern_system_driver_init();

	// IO 默认状态
	vTimeDelayInit(168);

	//usart4_init(115200);  //PC10 PC11
	//usart3_init(115200);  //PB10 PB11
	usart1_init(2000000);   //PA9 PA10 打印串口
//	usart2_init(115200);    //PA2 PA3  dongle串口
	EXTERNAL_UART_SET_MCU;
	// 按键配置
	key_extern_monitor_init();

	EXTERNAL_POWER5V_OFF;
	EXTERNAL_POWER3V3_OFF;
	EXTERNAL_GREEN_LED_OFF;
	EXTERNAL_DUT_LED_BLUE_H;
	EXTERNAL_DUT_LED_RED_H;
	EXTERNAL_DISTANCE_SENSOR_XSHUT_L;
	//lcd_init();  
//	ui_extern_main_init();

	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_CDC_cb,&USR_cb);

	// 默认device id  获取芯片ID
	system_run_ram.device_id = init_extern_system_get_lock_id();
//	system_run_ram.dev_work_state.get_pid_ok  = 0;
	system_run_ram.wait_chip_work_mode = 0;
	TRACE_WP("[init]init_extern_system_get_lock_id:[%08X]\r\n",system_run_ram.device_id);
	TIM3_Int_Init(10-1,8400-1);

	vTimeDelayMs(300);
//	LED_OFF;
	
//	sht4xx_init();
//  sht4xx_soft_reset();
}

