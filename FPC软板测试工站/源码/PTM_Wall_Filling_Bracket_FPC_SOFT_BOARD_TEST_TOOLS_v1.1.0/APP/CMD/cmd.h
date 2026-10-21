
#ifndef __AT_CMD_HANDLE_H__
#define __AT_CMD_HANDLE_H__

#include "user.h"


// ÿһ��ATָ��������ʱ��
#define  AT_CMD_INTERVAL_TIME_OUT					2200
#define  AT_CMD_ANALYSIS_BUF_LEN					512

//0x86 0x12 0x0b 0x00 0x00 0x00
#define  SKU   ""


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
#define  SEND_PC_DATA_CMD         0x90
#define  SEND_PC_DATA_SUBCMD      0x00





#define CMD_HEAD        		 0x7F
#define CMD_DATA_SITE  			 5    // 这个表示数据的位置 位置是从0开始算的 也就是CMDHEAD 的位置是0
#define CMD_VALID_DATA_LEN       6    // 有效数据包括数据包头 和 校验位 不包括数据





#define PROFUCT_CMD_NONE        		 0     //表示空数据，且后续无其它数据
#define PROFUCT_CMD_NONE_DATA_LEN	 	(1)    //数据的长度

#define PROFUCT_CMD_UNDEFINE             1     //1个字节表示的设备类型(还未统一约定)
#define PROFUCT_CMD_UNDEFINE_LEN        (1)    //长度

#define PROFUCT_CMD_LICENSE              2     //LICENSE(DeviceId)
#define PROFUCT_CMD_LICENSE_LEN         (5)    //10个字符DeviceId，用5个字节十六进制值表示

#define PROFUCT_CMD_CHIPID               3     //CHIP_ID
#define PROFUCT_CMD_CHIPID_LEN          (6)    //6个字节十六进制值(和服务端定义同步)

#define PROFUCT_CMD_MAC             	 4     //MAC
#define PROFUCT_CMD_MAC_LEN       		(6)    //6个字节十六进制值

#define PROFUCT_CMD_PROFUCT_ID             5     //产品识别码
#define PROFUCT_CMD_PROFUCT_ID_LEN      (7)    //7个字节14个字符，数字字符串，使用BCD码

#define PROFUCT_CMD_RSSI_ID          	0xff    //产品识别码
#define PROFUCT_CMD_RSSI_ID_LEN      	(1)    //7个字节14个字符，数字字符串，使用BCD码




// AT ָ������ṹ��
typedef struct _AT_CMD_ANALYSIS_ST_
{
	uint32_t end_at_time;
	uint32_t at_len;
	uint8_t at_buf[AT_CMD_ANALYSIS_BUF_LEN];
} AT_CMD_ANALYSIS;


extern AT_CMD_ANALYSIS g_at_analysis;

// ��������
void at_cmd_extern_tx_string( uint8_t * p_str );
void at_cmd_extern_tx_cmd( uint8_t * p_tx_buf, uint16_t len );

uint8_t at_cmd_extern_analysis_special_cmd_find( void );
uint8_t at_cmd_extern_analysis_uart_fifo( uint8_t tmp);

void at_cmd_extern_reset_dev_state( void );
void at_cmd_extern_into_factory_handle(void);

void power_and_uart_state_off();

void at_cmd_extern_pkt_main_handle(void);
void at_cmd_extern_pkt_main_handle_usb(void);
void at_cmd_extern_temperature_humidity_send(void);
void extern_red_led_and_blue_led_loop_light(void);

void bouffalo_isp_extern_device_start_burn(void);
void bouffalo_isp_extern_device_end_burn(void);
void bouffalo_isp_extern_device_fail_burn( void );
uint8_t cmd_init_dongle(void);
   
        

#endif


