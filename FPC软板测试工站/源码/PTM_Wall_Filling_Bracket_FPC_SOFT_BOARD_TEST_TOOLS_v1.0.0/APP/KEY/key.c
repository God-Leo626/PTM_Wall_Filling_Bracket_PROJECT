/**********************************************************************************
*Copyright (c) 2016, Jimtch.ltd
*All rights reserved.
*
* 文件名称：	KEY EVENT
* 内容摘要：	按键
* 文件标识：	无
* 其它说明：	

* 当前版本：	V1.0
*
*修改日期        	版本号              	修改人         		修改内容
* -----------------------------------------------------------------
* 2017412             V1.0                         Alex                       创建
************************************************************************************/

#include "string.h"
#include "key.h"
#include "usart.h"
#include "fifo.h"

volatile uint8_t key4_press_flag;


// 按键结构体
static KEY_MONITOR_MARK	key_monitor_st[MAX_KEY_COUNT_NUM];
static KEY_GPIO_IO_PIN		key_io_pin[MAX_KEY_COUNT_NUM]={DEVICE_KEY_GPIO_CFG};
static KEY_QUEUE  key_q;

// IO 输入检测
static KEY_MONITOR_MARK		mode_in_st[IO_MAX_IN_COUNT_NUM];
static KEY_GPIO_IO_PIN		in_io_pin[IO_MAX_IN_COUNT_NUM]={IN_KEY_GPIO_CFG};


KEY_MONITOR_MARK io_test_state[IO_MAX_IN_COUNT_NUM];

/************************************************************************************************
* 功能描述： 	按键初始化事件
* 输入参数：   	无
* 输出参数：	无
* 返回值     ： 	无
* 其它说明：
* 修改记录：   
* 修改日期		  版本号		       修改人		 修改内容
*  ----------------------------------------------------------------------------
* 20160316		    V1.0 			        Alex		    创建
**************************************************************************************************/
void key_extern_monitor_init(void )
{
	uint8_t	i;

	// KEY 
	for(i=0;i<MAX_KEY_COUNT_NUM;i++)
	{
		key_monitor_st[i].en_key_down = 0;
		key_monitor_st[i].key_mask = KEY_EVENT_UNKNOW;
		key_monitor_st[i].key_down_time = 0;		
	}

	for(i=0;i<IO_MAX_IN_COUNT_NUM;i++)
	{
		mode_in_st[i].en_key_down = 0;
		mode_in_st[i].key_mask = KEY_EVENT_UNKNOW;
		mode_in_st[i].key_down_time = 0;	

	}

}


/************************************************************************************************
* 功能描述： 	按键初始化事件
* 输入参数：   	无
* 输出参数：	无
* 返回值     ： 	无
* 其它说明：
* 修改记录：   
* 修改日期		  版本号		       修改人		 修改内容
*  ----------------------------------------------------------------------------
* 20160316		    V1.0 			        Alex		    创建
**************************************************************************************************/
void key_extern_clear_state(void )
{
	uint8_t	i;

	for(i=0;i<IO_MAX_IN_COUNT_NUM;i++)
	{
		io_test_state[i].en_key_down = 0;
		io_test_state[i].key_mask = KEY_EVENT_UNKNOW;
		io_test_state[i].key_down_time = 0;	

	}

}


/************************************************************************************************
* 功能描述： 	将键值加入队列
* 输入参数：   	无
* 输出参数：	无
* 返回值     ： 	无
* 其它说明：
* 修改记录：   
* 修改日期		  版本号		       修改人		 修改内容
*  ----------------------------------------------------------------------------
* 20160316		    V1.0 			        Alex		    创建
**************************************************************************************************/
void put_key_code( uint8_t key_code )
{

	if ( key_q.count < 10 )
	{
		key_q.queue[key_q.wi++] = key_code;
		key_q.last_key = key_code;
		if ( key_q.wi>=10 )
		{
			key_q.wi = 0;
		}
		key_q.count++;
	}
	else
	{
		// 按键事件过多超过10个,  抛弃
		get_key_code();
		TRACE_WP("Discard key events\r\n");
		
		key_q.queue[key_q.wi++] = key_code;
		key_q.last_key = key_code;
		if ( key_q.wi>=10 )
		{
			key_q.wi = 0;
		}
		key_q.count++;
	}
	
}

/************************************************************************************************
* 功能描述： 	从队列中读取按键值
* 输入参数：   	无
* 输出参数：	无
* 返回值     ： 	无
* 其它说明：
* 修改记录：   
* 修改日期		  版本号		       修改人		 修改内容
*  ----------------------------------------------------------------------------
* 20160316		    V1.0 			        Alex		    创建
**************************************************************************************************/
uint8_t get_key_code( void )
{
	uint8_t key = KEY_VAL_NONE;
	
	if ( key_q.count > 0 )
	{
		key = key_q.queue[key_q.ri++];
		if ( key_q.ri>=10 )
		{
			key_q.ri = 0;
		}
		key_q.count--;
	}
	
	key_q.last_key = key;
	return key;
	
}





/************************************************************************************************
* 功能描述： 	按键ms 级定时事件
* 输入参数：   	无
* 输出参数：	无
* 返回值     ： 	无
* 其它说明：
* 修改记录：   
* 修改日期		  版本号		       修改人		 修改内容
*  ----------------------------------------------------------------------------
* 20160316		    V1.0 			        Alex		    创建
**************************************************************************************************/
void key_extern_timer_1ms_handle(void)
{

	uint8_t	i;
	
	// key 
	for(i=0;i<MAX_KEY_COUNT_NUM;i++)
	{
		if( key_monitor_st[i].en_key_down )
		{
			key_monitor_st[i].key_down_time++; 
		}
	}

	// IO 
	for(i=0;i<IO_MAX_IN_COUNT_NUM;i++)
	{
		if( mode_in_st[i].en_key_down )
		{
			mode_in_st[i].key_down_time++; 
		}
	}

	if( key4_press_flag != system_run_ram.start_testing)
	{
		if(system_run_ram.start_tick < 0xff)
		{
			system_run_ram.start_tick++;
		}
	}
	else
	{
		system_run_ram.start_tick = 0;
	}
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
void gpio_extern_key_short_down_handle(void)
{
	// 启动程序烧录
	system_run_ram.en_burn_bl_chip = 1;
	
	// 状态切换
	system_run_ram.burn_and_test_result = 0;
	//system_run_ram.test_io_success = 0;
	//ui_extern_refresh();


	TRACE_WP("[AT]Change Mode To AT Factory\r\n");
	system_run_ram.wait_chip_work_mode = 1;
	//system_run_ram.wait_at_cmd_tick = 0;
//	EXTERNAL_UART_SET_DUT;
	memset( (void *)(&system_run_ram.dev_work_state), 0 , sizeof(DEV_WORK_STATE) );
//	ms_nop_delay(300);
//	ui_extern_refresh();
	EXTERNAL_POWER5V_ON;
	EXTERNAL_POWER3V3_ON;
	vTimeDelayMs(100);
	return;
}



/************************************************************************************************
* 功能描述： 	按键监测
* 输入参数：   	无
* 输出参数：	无
* 返回值     ： 	无
* 其它说明：
* 修改记录：   
* 修改日期		  版本号		       修改人		 修改内容
*  ----------------------------------------------------------------------------
* 20160316		    V1.0 			        Alex		    创建
**************************************************************************************************/
void key_extern_monitor_handle(void)
{
	uint8_t	i;
	UCHAR8 corotation_start_cmd_buf[] = {0x7f,0x06,0x80,0xf0,0x00,0x76}; 
	UCHAR8  corotation_stop_cmd_buf[] = {0x7f,0x06,0x80,0xff,0x00,0x79}; 
	UCHAR8  corotation_read_cmd_buf[] = {0x7f,0x06,0x80,0x49,0x01,0xCE}; 

	if( 0 == GPIO_ReadInputDataBit( IN_PINS_KEY_MODE_DOWN ) )
	{
		key4_press_flag = 1;
		if(!system_run_ram.start_testing && system_run_ram.start_tick >= 50)
		{
				system_run_ram.start_testing = 1;
				TRACE_WP("Module start testing #####\r\n");
				uart_fifo_extern_clear_fifo_usb();
				usb_extern_txrx_send_buf(corotation_start_cmd_buf,6);
				gpio_extern_key_short_down_handle();
//				LED_B_ON;
				system_run_ram.start_tick = 0;
				ms_nop_delay(50);
		}
	}
	else
	{		
		key4_press_flag = 0;
		// Module not start testing
		if(system_run_ram.start_testing && system_run_ram.start_tick >= 50)
		{
//			EXTERNAL_UART_SET_MCU;
//			ms_nop_delay(50);
			system_run_ram.start_testing = 0;
			system_run_ram.start_tick = 0;
			TRACE_WP("Module test end ******\r\n");
			uart_fifo_extern_clear_fifo_usb();
			usb_extern_txrx_send_buf(corotation_stop_cmd_buf,6);
//			power_and_uart_state_off();
//			if( system_run_ram.en_burn_bl_chip == 1 )
//			{
//				bouffalo_isp_extern_device_fail_burn();
//			}
//			else
//			{
//			LED_R_ON;
			bouffalo_isp_extern_device_end_burn();
//			}
		}
	}
}


