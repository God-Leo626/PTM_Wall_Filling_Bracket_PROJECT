/**********************************************************************************
*Copyright (c) 2016, Jimtch.ltd
*All rights reserved.
*
* �ļ����ƣ�	KEY EVENT
* ����ժҪ��	����
* �ļ���ʶ��	��
* ����˵����	

* ��ǰ�汾��	V1.0
*
*�޸�����        	�汾��              	�޸���         		�޸�����
* -----------------------------------------------------------------
* 2017412             V1.0                         Alex                       ����
************************************************************************************/

#ifndef __KEY_EVENT_DRV_H__
#define __KEY_EVENT_DRV_H__


#include "user.h"

// ������ֵ˵��
#define	KEY_VAL_NONE					0X00
#define	KEY_VAL_START					0X01
#define	KEY_VAL_CANCEL					0X02
#define	KEY_VAL_UP						0X10
#define	KEY_VAL_OK						0X11
#define	KEY_VAL_DOWN					0X12



// �����¼�
#define KEY_EVENT_DOWN  				1
#define KEY_EVENT_UP   					2
#define KEY_EVENT_LONG_DOWN   			4
#define KEY_EVENT_UNKNOW   				0

#define KEY_DOWN_MIN_TIME 				20
#define KEY_DOWN_LONG_TIME 				3000

#define MAX_KEY_COUNT_NUM				3   


// wifi ��С�ź�Ҫ��
#define MIN_WIFI_RSSI_VAL				(-62)
// IO ����
#define IO_IN_HIGHT_MIN_TIME 			20
#define IO_MAX_IN_COUNT_NUM				6  


// �������ṹ��
typedef struct _KEY_MONITOR_MARK_ST_
{
	uint8_t 		en_key_down;			// ��������
	uint8_t 		key_mask;				// �����¼�
	uint16_t  	    key_down_time;			// ����ʱ��
	uint32_t 		mark_time;				// ����ϵͳʱ��
}KEY_MONITOR_MARK;


// ����PIN  IO �ڽṹ��
typedef struct  _KEY_GPIO_IO_PIN_
{
	GPIO_TypeDef* 	key_GPIOx;
	uint16_t		key_GPIO_Pin;
}KEY_GPIO_IO_PIN;

// ��������
typedef struct  _KEY_QUEUE_ST_T_
{
	uint8_t queue[30];		// �������л���, ����10������
	uint8_t count;			// �����ĸ���
	uint8_t ri;				// д��λ��
	uint8_t wi;				// ��ȡλ��
	uint8_t last_key;			// ���һ��������ֵ

} KEY_QUEUE;



extern KEY_MONITOR_MARK io_test_state[IO_MAX_IN_COUNT_NUM];


void key_extern_clear_state(void );


void put_key_code( uint8_t key_code );
uint8_t get_key_code( void );
void key_extern_timer_1ms_handle(void);
void key_extern_monitor_handle(void);
void key_extern_monitor_init(void);
void key_extern_monitor_clear(uint8_t index);

void gpio_extern_key_short_down_handle(void);


#endif


