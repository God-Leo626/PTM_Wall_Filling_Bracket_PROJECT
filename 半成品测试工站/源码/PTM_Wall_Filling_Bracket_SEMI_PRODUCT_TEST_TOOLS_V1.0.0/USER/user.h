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

#ifndef __USER_H__
#define __USER_H__


#include <stdint.h>
#include "stm32f4xx.h" 


//peripheral
#include "key.h"
#include "gpio.h"
#include "time.h"
#include "timer.h"
#include "usart.h"
#include "lcd.h"
#include "cmd.h"
#include "display.h"
#include "font.h"
#include "fifo.h"
//usb
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h" 
#include "usbd_cdc_core.h"
#include "usbd_cdc_vcp.h" 

#define TOOL_LEIDIAO               0
#define TOOL_LEIDIAO_JIAOYAN       1

#define LEIDIAO_STATE					TOOL_LEIDIAO
#define	SOFTWARE_VER					"BL6XX_DOWN_FYV2_V2.1" 


#define WARN_NOUSER(X)  			  ( (void)(X) )
#define TRACE_ERROR(str,...)      	  {	printf("-E-%s::%d:\t"str"\r\n",  __FILE__ , __LINE__, ##__VA_ARGS__);		}
#define TRACE_WP(...)    			  { 	printf(__VA_ARGS__); 	}
#define TRACE_INTO_FUN()              TRACE_WP(" ############ into %s  ############\r\n",__func__)
#define TRACE_TEST_FUN_OK()           TRACE_WP(" ############ SUCCESS %s  ############\r\n",__func__)



#define uart_fifo_extern_get_fifo_count 	vFifoUatrGetFifoCount
#define uart_fifo_extern_clear_fifo			vFifoUatrClearFifo
#define uart_fifo_extern_get_rx_dat			vFifoUatrGetFifoDat

#define uart_fifo_extern_get_fifo_count_usb 	vFifoUsbGetFifoCount
#define uart_fifo_extern_clear_fifo_usb			vFifoUsbClearFifo
#define uart_fifo_extern_get_rx_dat_usb			xFifoUsbGetFifoDat


#define us_nop_delay						vTimeDelayUs
#define ms_nop_delay						vTimeDelayMs






#define RETURN_SUCCESS				0
#define RETURN_ERROR				1


#define SYSTEM_FLAG_100MS_EVENT			(1<<0)		
#define SYSTEM_FLAG_1S_EVENT			(1<<1)		
#define SYSTEM_FLAG_100MS_EVENT_USB     (1<<2)


#define SYSTEM_FLAG_MEASURE_EVENT		(1<<10)		
#define SYSTEM_FLAG_AT_UPLOAD_EVENT		(1<<11)		

#define SYSTEM_FLAG_USB_WAIT_RX			(1<<17)	
#define SYSTEM_FLAG_USB_PKT_UNSEND		(1<<18)		
#define SYSTEM_FLAG_USB_ONCE_RX			(1<<19)		
#define SYSTEM_FLAG_USB_RX_END			(1<<20)		

#define SYSTEM_FLAG_MCU_UPDATE			(1<<21)		
#define SYSTEM_FLAG_MCU_RELOAD			(1<<22)		

#define SYSTEM_FLAG_UART_MSG			(1<<25)		

#define SYSTEM_FLAG_MSG_CRC_EVENT		(1<<27)		
#define SYSTEM_FLAG_MSG_SEND_EVENT		(1<<28)		

#define SYSTEM_FLAG_SOFT_RESET			(1<<30)		


typedef unsigned char					UCHAR8;
typedef char							CHAR8;
typedef unsigned short					UINT16;
typedef short							INT16;
typedef unsigned int					UINT32;
typedef int								INT32;
typedef unsigned long long				UINT64;
typedef long long						INT64;
typedef signed int						BOOL;
typedef float							FLOAT32;
typedef unsigned int 					size_t;



#define KEYDOWN  							1
#define KEYUP   							2
#define KEYLONGDOWN   						4
#define KEYUNKNOW   						0

#define KEYDOWNMINTIME 						30
#define KEYDOWNLONGTIME 					2800

#define FALSE 0
#define TRUE  1




//获取nrf固件信息的指令
#define  GET_FWVER_CMD            0x80
#define  GET_FWVER_SUBCMD_CMD     0x00

//设置nrf rssi的指令
#define  SET_RSSI_CMD             0x81
#define  SET_RSSI_SUBCMD_CMD      0x00

//获取nrf 发送过来的数据信息
#define  GET_NRF_DATA_CMD         0x82
#define  GET_NRF_SUBCMD_CMD       0x00


//获取nrf 发送过来的数据信息
#define  SEND_PC_DATA_CMD         0x87
#define  SEND_PC_DATA_SUBCMD      0x00





#define CMD_HEAD        		 0x7F
#define CMD_DATA_SITE  			 5    // 这个表示数据的位置 位置是从0开始算的 也就是CMDHEAD 的位置是0
#define CMD_VALID_DATA_LEN       6    // 有效数据包括数据包头 和 校验位 不包括数据






typedef struct _KEY_SET_MARK_ST_
{
	uint16_t  	key_down_time;
	uint8_t 	key_mask;
	uint8_t 	en;	
}SYS_KEY_MARK;


typedef struct  _DEV_WORK_STATE_
{

	uint8_t in_factory_mode;				// �豸����ģʽ0 δ���빤��ģʽ��1 �Ѿ����빤��ģʽ

	uint8_t get_fw_ver_sign;					
	uint8_t get_pcba_sign;				
	uint8_t get_test_gpio_sign;				
	uint8_t get_license_id_sign;				
	uint8_t get_mac_sign;  
	uint8_t read_rssi_sign;				


	uint8_t seng_start_pc_sign;
	uint8_t seng_informa_to_pc_sign;


	uint8_t tell_pc_start_sign;
	uint8_t wait_pc_test_result_sign;

	UCHAR8  get_fw_ver;
	UCHAR8 ask_qfn52832;
	signed char ak803_rssi;      		// 

	UCHAR8 seng_rssi_qfn52832;
	UCHAR8 seng_sku_sign;

	UCHAR8 receive_informa;
	UCHAR8 devideid[10];
	UCHAR8 chip_id[10];
	UCHAR8 mac[10];
	UCHAR8 productid[10];

	UCHAR8 get_pid_ok;
	UCHAR8 usb_seng_informa;
	signed char usb_seng_rssi; //上位机发送过来的阈值

	UCHAR8 rssi_start;					// ��ʼ����
	UCHAR8 rssi_stop;					// ֹͣ����
	UCHAR8 rssi_get_rssi;               // ��ȡRSSI
	UCHAR8 read_mac[50];
	UCHAR8 read_pid[50];
	UCHAR8 read_pid_mac_sign;
	UCHAR8 send_start_signal;
	UCHAR8 send_informa_to_pc;
	signed char  read_rssi;		
	
}DEV_WORK_STATE;






typedef struct  _SYSTEM_RUN_RAM_ST_
{

	uint8_t fw_ver[70];
	uint8_t mac[20];
	uint8_t model_pcba[30];
	int8_t wifi_rssi;
	uint32_t chip_burn_time;
	uint8_t read_rssi_sign;

	uint32_t rtc_time_unix;

	uint8_t test_io_success;


	uint8_t start_testing;
	uint8_t start_tick;

	uint8_t burn_and_test_result;

	uint8_t need_test_io;


	uint8_t en_burn_bl_chip;
	uint8_t burn_bl_fail_tick;
	uint8_t at_bl_chip_fail_tick;
	
	uint8_t wait_chip_work_mode;


	uint16_t burn_time_out_tick;
	uint16_t wait_at_cmd_tick;

	DEV_WORK_STATE dev_work_state;
	

	uint32_t success_burn_num;
	uint32_t fail_burn_num;
	
	SYS_KEY_MARK sys_key;
	uint32_t device_id;
	
	uint32_t end_at_cmd_time;
    uint32_t start_bootload_time;


	uint8_t seng_rssi_qfn52832;
	uint8_t usb_sku[10];
	char aa[100];
	char usb_seng_rssi;


}SYSTEM_RUN_RAM;




#define	MCU_INFO_CHECH_START			0X35e789f7
#define	MCU_INFO_CHECH_END				0Xcb8a59fc



extern SYSTEM_RUN_RAM	system_run_ram;
extern volatile uint32_t sys_work_info;



uint8_t  init_extern_test_write_flash_data(void);
uint8_t  init_extern_burn_write_flash_data(void);
void     init_extern_burn_read_flash_data(void);

uint8_t  init_extern_data_write_flash_data(void);
void     init_extern_data_read_flash_data(void);


void init_extern_system_init(void);
void init_extern_system_reset(void);




#endif


