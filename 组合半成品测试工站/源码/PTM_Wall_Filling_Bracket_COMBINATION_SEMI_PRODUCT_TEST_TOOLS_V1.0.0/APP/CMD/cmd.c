
#include "user.h"
#include "string.h"

unsigned char board_number;   
	 
AT_CMD_ANALYSIS g_at_analysis;
AT_CMD_ANALYSIS g_at_analysis_usb;

UCHAR8 sku_buf[7] = {0x86,0x12,0x0b,0x00,0x01,0x00};
UCHAR8 read_sku_buf[10] = {0};
 
static UCHAR8  record_mac_buf[30];


//UCHAR8 sku_buf[7] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
typedef struct _MSG_APP_PKT_
{
	UCHAR8	dat[128];
	UINT32	lenth;
}MSG_APP_PKT;

MSG_APP_PKT msg_ptk;
MSG_APP_PKT rssi_ptk;


#define CMD_HEAD        		 0x7F
#define CMD_DATA_SITE  			 5    // �����ʾ���ݵ�λ�� λ���Ǵ�0��ʼ��� Ҳ����CMDHEAD ��λ����0
#define CMD_VALID_DATA_LEN       6    // ��Ч���ݰ������ݰ�ͷ �� У��λ ����������

uint8_t at_cmd_extern_send_start_to_pc( void );
uint8_t at_cmd_extern_analysis_uart_fifo_usb( uint8_t tmp);

UCHAR8 at_cmd_extern_config_hex_eigenvalue(void);
UCHAR8 at_cmd_extern_config_save(void);
UCHAR8 at_cmd_extern_rssi_stop_test(void);
UCHAR8 at_cmd_extern_rssi_start_test(void);

uint8_t * StrFindSubString( uint8_t * p_str, uint8_t * p_sub_str )
{
	uint16_t i;
	
	while( *p_str )
	{
		i = 0;
		while (( p_str[i]!=0 )&&( p_str[i] == p_sub_str[i] ))
		{
			if ( p_sub_str[i+1]==0 )
			{
				return p_str;
			}
			i++;
		}
		p_str++;
	}
	return NULL;
}


uint8_t factory_crc(uint8_t *buf,uint8_t len){
    uint8_t i = 0;
    uint8_t x = 0;
  
    for(i=0; i<len; i++){
        x = x^(*(buf+i));
    }
  
    return x;
}


static char prvPcVerificationCmd(unsigned char *p_input_save_fifo_buf  , unsigned short int input_num)
{
    unsigned char i = 0;
    unsigned char fcs = 0;    //�Լ������У��ֵ
    unsigned char cmd_fcs = 0;//CMD�Ӷ�ȡ��У��ֵ
    unsigned char len = 0;    //CMD�ж�ȡ�����ݳ���
    for ( i = 0; i < input_num; i++)
    {
        //�ж�֡ͷ
        if(p_input_save_fifo_buf[i] == 0x7f) 
        {
           //�õ�����
           len = p_input_save_fifo_buf[i+1];
           //��ȡfcsУ��
           cmd_fcs = p_input_save_fifo_buf[i+len-1];
           //�Լ������У��ֵ
           fcs = factory_crc(&p_input_save_fifo_buf[i+1], len - 2);

           if( (fcs == cmd_fcs) && (len >= CMD_VALID_DATA_LEN))
           {
        	   return 1;
           }
        }
    }
    return 0;
}




static char BL702_static_verification_cmd(unsigned char *p_input_save_fifo_buf  , unsigned short int input_num)
{
    unsigned char i = 0;
    unsigned char fcs = 0;   
    unsigned char cmd_fcs = 0;
    unsigned char len = 0;   
    for ( i = 0; i < input_num; i++)
    {
        if(p_input_save_fifo_buf[i] == 0x7f) 
        {
           len = p_input_save_fifo_buf[i+1];
           cmd_fcs = p_input_save_fifo_buf[i+len-1];
           fcs = factory_crc(&p_input_save_fifo_buf[i+1], len - 2);
           if( (fcs == cmd_fcs) && (len >= CMD_VALID_DATA_LEN))
           {     
        	   return 1;
           }
        }
    }
    return 0;
}



void at_cmd_extern_tx_string( uint8_t * p_str )
{
	uart_extern_burn_and_at_uart_tx_buf( p_str, strlen( (char *)p_str ) );
}


void at_cmd_extern_tx_cmd( uint8_t * p_tx_buf, uint16_t len )
{
	uart_extern_burn_and_at_uart_tx_buf( p_tx_buf, len );
}

void at_cmd_extern_reset_dev_state( void )
{
	memset((void *)(&system_run_ram.dev_work_state), 0 , sizeof(DEV_WORK_STATE) );
}

UCHAR8 at_cmd_extern_analysis_special_cmd_find( void )
{	
	int i;
	int j;
	UCHAR8 ret;
	UCHAR8 dat;
	UCHAR8 receive_buf[150];
	signed char rssi;
	UCHAR8 *p_receie = NULL;
	char *p_rssi = NULL;
	UCHAR8 crc = 0;
	UCHAR8 *p = NULL;
	UCHAR8  offset = 17;//偏移量 
	TRACE_WP("at_cmd_extern_analysis_special_cmd_find\r\n");
	if( NULL != StrFindSubString( g_at_analysis.at_buf, (UCHAR8 *)("read broadcast success <") ) )
	{
		p_receie = StrFindSubString( g_at_analysis.at_buf, (UCHAR8 *)("read broadcast success <") );
		p_receie = p_receie + strlen("read broadcast success <");
		TRACE_WP("p_receie %s\r\n",p_receie);
		p_rssi = strstr( (char *)g_at_analysis.at_buf, ">rssi");
		if(p_rssi != NULL)
		{
			sscanf(p_rssi,">rssi %d",&system_run_ram.dev_work_state.read_rssi);	
			TRACE_WP("system_run_ram.dev_work_state.read_rssi %d\r\n",system_run_ram.dev_work_state.read_rssi);

			*p_rssi = ' ';
			*(p_rssi + 1) = 0;
			p = receive_buf;
			sscanf(p_receie,"%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x ",p,p+1,
			p+2,p+3,p+4,p+5,p+6,p+7,p+8,p+9,p+10,p+11,p+12,p+13,p+14,p+15,p+16,p+17,p+18,p+19,p+20,p+21,p+22,p+23,p+24,p+25,p+26,p+27,p+28,p+29,p+30,p+31,p+32);
			for(j = 0; j < 33 ; j++)
			{
				TRACE_WP("receive_buf[%d] = %x\r\n",j,receive_buf[j])
			}

  			// 垃圾项目没 CRC   检测CRC
			// TRACE_WP("CRC receive_buf[30] = %x  factory_crc(receive_buf,30) = %x\r\n",receive_buf[30], factory_crc(receive_buf,30));
			// if(receive_buf[30] != factory_crc(receive_buf,30))
			// {
			// 	TRACE_WP("ERROR CRC receive_buf[30] = %x  factory_crc(receive_buf,30) = %x\r\n",receive_buf[30], factory_crc(receive_buf,30));
			// 	g_at_analysis.at_len = 0;	
			// 	return RETURN_ERROR;
			// }
			// //检测特征值
			// for(j = 0; j < 6 ; j++)
			// {
			// 	if(receive_buf[j] != sku_buf[j])
			// 	{
			// 		TRACE_WP("ERROR sku j = %d receive_buf[j] = %x  sku_buf[j] = %x\r\n",j,receive_buf[j], sku_buf[j]);
			// 		g_at_analysis.at_len = 0;	
			// 		return RETURN_ERROR;
			// 	}
			// }
			//判断RSSI
			if(system_run_ram.dev_work_state.read_rssi <= system_run_ram.usb_seng_rssi)
			{	
				TRACE_WP("ERROR rssi  :read_rssi = %d usb_seng_rssi = %d \r\n",system_run_ram.dev_work_state.read_rssi, system_run_ram.usb_seng_rssi);
				ms_nop_delay(500);
				g_at_analysis.at_len = 0;	
				uart_fifo_extern_clear_fifo();
				system_run_ram.dev_work_state.send_informa_to_pc = 0;
				return 0;
			}
			//PID前面标志的
			//复制mac
			//防止重复发送数据给上位机
			for(j = 0; j < 8 ; j++)
			{
				if(record_mac_buf[0] != receive_buf[offset+j])
				{
					break;
				}
			}
			
			// if(j < 16)
			// {
			// 	TRACE_WP("system_run_ram.dev_work_state.read_mac  = \r\n");
			// 	for(j = 0; j < 16 ; j++)
			// 	{
			// 		system_run_ram.dev_work_state.read_mac[j] = receive_buf[6+j];
			// 		record_mac_buf[j] = receive_buf[6+j];
			// 		TRACE_WP("0x%x ",system_run_ram.dev_work_state.read_mac[j]);
			// 	}
			// 	//复制PID
			// 	TRACE_WP("\r\n system_run_ram.dev_work_state.read_pid  \r\n");
			// 	for(j = 0; j < 7 ; j++)
			// 	{
			// 		system_run_ram.dev_work_state.read_pid[j] = receive_buf[23+j];
			// 		TRACE_WP("0x%x ",system_run_ram.dev_work_state.read_pid[j]);
			// 	}
			// 	TRACE_WP("\r\nread mac rssi  success\r\n");
			// 	g_at_analysis.at_len = 0;	
			// 	system_run_ram.dev_work_state.send_informa_to_pc = 1;
			// }	
		
			if(j < 16)
			{
				TRACE_WP("system_run_ram.dev_work_state.read_mac  = \r\n");		
				for(j = 0; j < 8 ; j++)
				{
					system_run_ram.dev_work_state.read_mac[j] = receive_buf[offset+j];
					record_mac_buf[j] = receive_buf[6+j];
					TRACE_WP("0x%x ",system_run_ram.dev_work_state.read_mac[j]);
				}
				//复制PID
				TRACE_WP("\r\n system_run_ram.dev_work_state.read_pid  \r\n");
				for(j = 0; j < 7 ; j++)
				{
					system_run_ram.dev_work_state.read_pid[j] = receive_buf[offset+9+j];
					TRACE_WP("0x%x ",system_run_ram.dev_work_state.read_pid[j]);
				}
				TRACE_WP("\r\nread mac rssi  success\r\n");
				g_at_analysis.at_len = 0;	
				system_run_ram.dev_work_state.send_informa_to_pc = 1;
			}	

			
			return 1;
		}

	}


	return 1;
}



UCHAR8 at_cmd_extern_analysis_uart_fifo( UCHAR8 tmp)
{
	UCHAR8 ret;
	UINT32 now_time;
	static UINT32 last_time;


	// AT ָ���������ʱ�ж�
	now_time = time_system_now_time;
	now_time = now_time-last_time;
	if ( now_time > AT_CMD_INTERVAL_TIME_OUT )
	{
		TRACE_WP("[Serial]clear at cmd buf time=%d,len=%d :%s\r\n",now_time,g_at_analysis.at_len,g_at_analysis.at_buf);
		g_at_analysis.at_len = 0;
		
	}
	last_time = time_system_now_time;

	// AT����Ϊ��
	if ( 0==g_at_analysis.at_len )	
	{
		// �յ���Ч��AT�ַ�(ASCII�����ڿո��Ժ���ַ��������ո�)���ж�ΪAT���ʼ
		if (( tmp>' ' )&&( tmp<0x80 ))		
		{
			g_at_analysis.at_buf[g_at_analysis.at_len++] = tmp;		
		}
	}
	else
	{

		g_at_analysis.at_buf[g_at_analysis.at_len++] = tmp;			
		g_at_analysis.end_at_time = time_system_now_time;
		if ( ( tmp < ' ')||( tmp > 0x80 ) )	
		{
			g_at_analysis.at_buf[g_at_analysis.at_len-1] = '\0';		
			g_at_analysis.at_len = g_at_analysis.at_len - 1;
			
			TRACE_WP("[AT CMD]len=%d::%s \r\n", g_at_analysis.at_len,g_at_analysis.at_buf  );
			ret = at_cmd_extern_analysis_special_cmd_find();
	
			return ret;	
		}
		if ( g_at_analysis.at_len >= AT_CMD_ANALYSIS_BUF_LEN )	
		{
			g_at_analysis.at_len = 0;		
		}
	}
	return 0;
}

#define AT_CMD_ERROR

uint8_t at_cmd_extern_find_cmd_error( void )
{

	// AT ָ���쳣�ж�
	system_run_ram.at_bl_chip_fail_tick++;
	if(system_run_ram.at_bl_chip_fail_tick >= 3 )
	{
		TRACE_WP("[at]at cmd error more than %d timers \r\n",system_run_ram.at_bl_chip_fail_tick);
		bouffalo_isp_extern_device_fail_burn();
	}
	
	return RETURN_SUCCESS;
}


uint8_t at_cmd_extern_find_cmd_time_out( void )
{
	
	// AT ָ���쳣�ж�
	system_run_ram.at_bl_chip_fail_tick++;
	if(system_run_ram.at_bl_chip_fail_tick >= 3 )
	{
		TRACE_WP("[at]at cmd error more than %d timers \r\n",system_run_ram.at_bl_chip_fail_tick);
		system_run_ram.dev_work_state.send_informa_to_pc = 0;
		bouffalo_isp_extern_device_fail_burn();
	}
	
	return RETURN_SUCCESS;
}


void power_and_uart_state_off(void)
{
	TEST_POWER_OFF;
	TEST_usart_OFF;
}

UCHAR8 tool_check_board_number(void)
{
	UCHAR8 bit0 = 0, bit1 = 0, bit2 = 0, bit3 = 0;
	
	board_number = 0;
	
	bit0 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9);
	bit1 = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8);
	bit2 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
	bit3 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);

	board_number = (bit3 << 3) | (bit2 << 2) | (bit1 << 1) | (bit0 << 0);
	TRACE_WP("board_number = %d - %x \r\n", board_number, board_number);
	
	return board_number;
}

UCHAR8 at_cmd_extern_analysis_special_cmd_find_usb( void )
{
	int i,j,k = 0;
	unsigned char position_board_number; 
	unsigned char DetSate;
	UCHAR8  send_into_factory[20] = {0x7F,0x06,0x80,0xE6,0x00,0x00};

	if ( BL702_static_verification_cmd( g_at_analysis_usb.at_buf , g_at_analysis_usb.at_len) == 1 )
	{
		if((g_at_analysis_usb.at_buf[2] == 0x80) && (g_at_analysis_usb.at_buf[3] == 0xF4) && (g_at_analysis_usb.at_buf[4] == 0x01))
		{
			TRACE_WP("************** [AT] START POWER POSITION *******************\r\n");
			EXTERNAL_POWER5V_ON;
			EXTERNAL_POWER3V3_ON;
			system_run_ram.at_bl_chip_fail_tick = 0;
		}
		else if((g_at_analysis_usb.at_buf[2] == 0x80) && (g_at_analysis_usb.at_buf[3] == 0xFF) && (g_at_analysis_usb.at_buf[4] == 0x01))
		{
			TRACE_WP("************** [AT] STOP POWER POSITION / TEST FAIL *******************\r\n");
			EXTERNAL_POWER5V_OFF;
			EXTERNAL_POWER3V3_OFF;
			system_run_ram.at_bl_chip_fail_tick = 0;
		}
		else if((g_at_analysis_usb.at_buf[2] == 0x80) && (g_at_analysis_usb.at_buf[3] == 0xF1) && (g_at_analysis_usb.at_buf[4] == 0x01))
		{
			TRACE_WP("************** [AT] TEST SUCCESS *******************\r\n");
			EXTERNAL_POWER5V_OFF;
			EXTERNAL_POWER3V3_OFF;
			system_run_ram.at_bl_chip_fail_tick = 0;
		}
//		else if((g_at_analysis_usb.at_buf[2] == 0x80) && (g_at_analysis_usb.at_buf[3] == 0x60) && (g_at_analysis_usb.at_buf[4] == 0x00))
//		{
//			TRACE_WP("************** [AT] IO STREAM TEST *******************\r\n");
//			system_run_ram.need_test_io = 1;
//			system_run_ram.test_io_success = 1;
//			key_extern_clear_state();
//			system_run_ram.at_bl_chip_fail_tick = 0;
//		}
		else if((g_at_analysis_usb.at_buf[2] == 0x80) && (g_at_analysis_usb.at_buf[3] == 0x66) && ((g_at_analysis_usb.at_buf[4] == 0x01)))
		{
			k = 0;
			TRACE_WP("************** [AT] board number read *******************\r\n");
			send_into_factory[k++] = 0x7F;
			send_into_factory[k++] = 0x00;
			send_into_factory[k++] = 0x80;
			send_into_factory[k++] = 0xE6;
			send_into_factory[k++] = 0x01;
			position_board_number = tool_check_board_number();
//			if(position_board_number == 0x01)
//			{
//				send_into_factory[k++] = 0x01;
//			}
//			else if(position_board_number == 0x02)
//			{
//				send_into_factory[k++] = 0x02;
//			}
//			else if(position_board_number == 0x03)
//			{
//				send_into_factory[k++] = 0x03;
//			}
//			else if(position_board_number == 0x04)
//			{
//				send_into_factory[k++] = 0x04;
//			}
//			else if(position_board_number == 0x0f)
//			{
//				TRACE_WP("No Position Board\r\n");
//				send_into_factory[k++] = 0x00;
//			}
			if((position_board_number >= 0x01) && (position_board_number <= 0x0f))
			{
				send_into_factory[k++] = position_board_number;
			}
			else
			{
				TRACE_WP("Exceeding the label range\r\n");
				send_into_factory[k++] = 0x00;
			}
			send_into_factory[1] = k + 1; 
			send_into_factory[k] = factory_crc(&send_into_factory[1], send_into_factory[1] - 2);
			usb_extern_txrx_send_buf(send_into_factory,send_into_factory[1]);
			system_run_ram.at_bl_chip_fail_tick = 0;
		}
		else
		{
			TRACE_WP("************** [AT] ORDER ERROR *******************\r\n");
			LED_R_ON;
			system_run_ram.at_bl_chip_fail_tick = 0;
		}
		return 1;
	}
	else
	{
		return 2; //���ض������������
	}
}




uint8_t at_cmd_extern_analysis_uart_fifo_usb( uint8_t tmp)
{
	int i,j;
	uint8_t at_buf[AT_CMD_ANALYSIS_BUF_LEN];
	uint32_t at_len;
	static uint8_t CMD_NUM = 0;
	uint8_t re_ret;
	
	uint8_t ret;
	uint32_t now_time;
	static uint32_t last_time;
	static uint8_t find_cmd_head_sign = 0;

	now_time = time_system_now_time;
	now_time = now_time-last_time;
	if ( now_time > AT_CMD_INTERVAL_TIME_OUT )
	{
		g_at_analysis_usb.at_len = 0;
	}
	last_time = time_system_now_time;


	if(g_at_analysis_usb.at_len == 0)	
	{
		if(tmp == 0x7f)
		{
			g_at_analysis_usb.at_buf[g_at_analysis_usb.at_len++] = tmp;
		}
	}
	else
	{
		g_at_analysis_usb.at_buf[g_at_analysis_usb.at_len++] = tmp;			
		g_at_analysis_usb.end_at_time = time_system_now_time;
		
		if ( g_at_analysis_usb.at_len >= AT_CMD_ANALYSIS_BUF_LEN )	
		{
			//TRACE_WP("[AT CMD]lost at more len %d\r\n",AT_CMD_ANALYSIS_BUF_LEN);
			g_at_analysis_usb.at_len = 0;		
		}
	}
	return 0;
}



uint8_t at_cmd_extern_send_start_to_pc( void )
{
	int i,k,j;
	uint8_t ret;
	uint8_t dat;
	uint8_t corotation_cmd_buf[20] = {0x7F,0x06,0x80,0xF0,0x00}; 
	TRACE_INTO_FUN();
	corotation_cmd_buf[5] = factory_crc(&corotation_cmd_buf[1], corotation_cmd_buf[1] - 2);
	vFifoUsbClearFifo();
	usb_extern_txrx_send_buf(corotation_cmd_buf,corotation_cmd_buf[1]);
	g_at_analysis_usb.at_len = 0;	
	vFifoUatrClearFifo();
	system_run_ram.at_bl_chip_fail_tick = 0;
	return RETURN_SUCCESS;
}


void at_cmd_extern_send_test_succes_to_pc( void )
{
	int i,k,j;
	uint8_t ret;
	uint8_t dat;
	TRACE_INTO_FUN();
	uint8_t corotation_cmd_buf[20] = {0x7F,0x06,0x80,0xF1,0x00}; 

	corotation_cmd_buf[5] = factory_crc(&corotation_cmd_buf[1], corotation_cmd_buf[1] - 2);

	vFifoUsbClearFifo();

	usb_extern_txrx_send_buf(corotation_cmd_buf,corotation_cmd_buf[1]);	

	vTimeDelayMs(500);
	vFifoUatrClearFifo();
	// Э�鴦����ɺ��������������ݰ�����
	g_at_analysis_usb.at_len = 0;	
	system_run_ram.at_bl_chip_fail_tick = 0;
}



void at_cmd_extern_send_test_error_to_pc( void )
{
	int i,k,j;
	uint8_t ret;
	uint8_t dat;
	uint8_t corotation_cmd_buf[20] = {0x7F,0x06,0x80,0xFF,0x00}; 
	TRACE_INTO_FUN();

	corotation_cmd_buf[5] = factory_crc(&corotation_cmd_buf[1], corotation_cmd_buf[1] - 2);

	vFifoUsbClearFifo();

	usb_extern_txrx_send_buf(corotation_cmd_buf,corotation_cmd_buf[1]);

	vFifoUatrClearFifo();
	// Э�鴦����ɺ��������������ݰ�����
	g_at_analysis_usb.at_len = 0;	
	system_run_ram.at_bl_chip_fail_tick = 0;
}



//一开始设置模式
UCHAR8 at_cmd_extern_config_dongle_type(void)
{
	int i;
	int j;
	UCHAR8 ret;
	UCHAR8 dat;
	UCHAR8 start_cmd_buf[] = {"config dongle type 3\r\n"};    
	TRACE_WP("******************* at_cmd_extern_config_dongle_type ***************************** \r\n");
	uart_fifo_extern_clear_fifo();
	g_at_analysis.at_len = 0;	
	uart_extern_burn_and_at_uart_tx_buf(start_cmd_buf,strlen(start_cmd_buf));
	us_nop_delay(10000);

	for(i=0;i<30;i++)
	{
		while(1)
		{
			if ( 0==uart_fifo_extern_get_fifo_count() )	
			{
				break;		
			}
			dat = uart_fifo_extern_get_rx_dat();
			ret = at_cmd_extern_analysis_uart_fifo(dat);
			if( ret == 1 )
			{
				if ( NULL != StrFindSubString( g_at_analysis.at_buf, (UCHAR8 *)("done") ) )
				{
					TRACE_WP("[AT]%s \r\n", g_at_analysis.at_buf  );
					uart_fifo_extern_clear_fifo();
					g_at_analysis.at_len = 0;	
					TRACE_WP("SET config dongle type ok\r\n" );
					return RETURN_SUCCESS;
				}
				else
				{
					TRACE_WP("[AT]Error AT cmd len:%d::%s \r\n", g_at_analysis.at_len,g_at_analysis.at_buf  );
					at_cmd_extern_find_cmd_error();
					g_at_analysis.at_len = 0;	
				}
			}
			else if( ret == 2 )
			{
				TRACE_WP("[AT]ACK AT cmd ERROR len:%d::%s \r\n", g_at_analysis.at_len,g_at_analysis.at_buf  );
				at_cmd_extern_find_cmd_error();
				g_at_analysis.at_len = 0;	
				uart_fifo_extern_clear_fifo();
				return RETURN_ERROR;
			}
		}
		us_nop_delay(50000);
	}
	TRACE_WP("[AT]Time out AT cmd\r\n");
	at_cmd_extern_find_cmd_time_out();
	g_at_analysis.at_len = 0;	
	uart_fifo_extern_clear_fifo();
	return RETURN_ERROR;
}



UCHAR8 at_cmd_extern_read_dongle_type(void)
{
	int i;
	int j;
	UCHAR8 ret;
	UCHAR8 dat;
	UCHAR8 read_type_sign = 0;
	UCHAR8 start_cmd_buf[] = {"read dongle type\r\n"};    
	TRACE_WP("******************* at_cmd_extern_read_dongle_type ***************************** \r\n");
	uart_fifo_extern_clear_fifo();
	g_at_analysis.at_len = 0;
	uart_extern_burn_and_at_uart_tx_buf(start_cmd_buf,strlen(start_cmd_buf));
	us_nop_delay(10000);
	for(i=0;i<50;i++)
	{
		while(1)
		{
			if ( 0==uart_fifo_extern_get_fifo_count() )	
			{
				break;
			}
			dat = uart_fifo_extern_get_rx_dat();
			TRACE_WP("%c",dat);
			ret = at_cmd_extern_analysis_uart_fifo(dat);
			if( ret == 1 )
			{
				if(NULL != StrFindSubString( g_at_analysis.at_buf, (UCHAR8 *)("3")))
				{
					TRACE_WP("at_cmd_extern_read_dongle_type ok dongle type = 3\r\n");
					g_at_analysis.at_len = 0;	
					return RETURN_SUCCESS;	
				}		
			}
			else if( ret == 2 )
			{
				TRACE_WP("[AT]ACK AT cmd ERROR len:%d::%s \r\n", g_at_analysis.at_len,g_at_analysis.at_buf  );
				at_cmd_extern_find_cmd_error();
				g_at_analysis.at_len = 0;	
				uart_fifo_extern_clear_fifo();
				return RETURN_ERROR;
			}
		}
		ms_nop_delay(40);
		if((i % 10) == 0)
		{
			uart_extern_burn_and_at_uart_tx_buf(start_cmd_buf,strlen(start_cmd_buf));
		}
	}
	TRACE_WP("[AT]Time out AT cmd\r\n");
	g_at_analysis.at_len = 0;	
	uart_fifo_extern_clear_fifo();
	return RETURN_ERROR;
}


//读取特征值
UCHAR8 at_cmd_extern_read_hex_eigenvalue(void)
{
	int i;
	int j;
	UCHAR8 ret;
	UCHAR8 dat;
	UCHAR8 *p_sku = NULL;
	UCHAR8 will_read_sign = 0;
	UCHAR8 start_cmd_buf[100] = {0};
	TRACE_WP("******************* at_cmd_extern_read_hex_eigenvalue ***************************** \r\n");


	sprintf(start_cmd_buf,"read hex eigenvalue\r\n");  
	// %02X %02X %02X %02X %02X %02X\r\n",p_sku[0],p_sku[1],p_sku[2],p_sku[3],p_sku[4],p_sku[5]);
	uart_fifo_extern_clear_fifo();
	g_at_analysis.at_len = 0;	
	uart_extern_burn_and_at_uart_tx_buf(start_cmd_buf,strlen(start_cmd_buf));
	ms_nop_delay(100);

	for(i=0;i<30;i++)
	{
		while(1)
		{
			if ( 0==uart_fifo_extern_get_fifo_count() )	
			{
				break;		
			}
			dat = uart_fifo_extern_get_rx_dat();
			ret = at_cmd_extern_analysis_uart_fifo(dat);
			if( ret == 1 )
			{
				if ( NULL != StrFindSubString( g_at_analysis.at_buf, (UCHAR8 *)("read hex eigenvalue") ) )
				{	
					will_read_sign = 1;	
					g_at_analysis.at_len = 0;	
				}
				else
				{
					if(will_read_sign == 1)
					{
						sscanf(g_at_analysis.at_buf,"%02X %02X %02X %02X %02X %02X",&read_sku_buf[0],&read_sku_buf[1],&read_sku_buf[2],
																				   &read_sku_buf[3],&read_sku_buf[4],&read_sku_buf[5]);
						TRACE_WP("[AT]READ SKU =  %02X %02X %02X %02X %02X %02X",read_sku_buf[0],read_sku_buf[1],read_sku_buf[2],
																				   read_sku_buf[3],read_sku_buf[4],read_sku_buf[5]);
						g_at_analysis.at_len = 0;
						return RETURN_SUCCESS;															
					}
				}
			}
			else if( ret == 2 )
			{
				TRACE_WP("[AT]ACK AT cmd ERROR len:%d::%s \r\n", g_at_analysis.at_len,g_at_analysis.at_buf  );
				at_cmd_extern_find_cmd_error();
				g_at_analysis.at_len = 0;	
				uart_fifo_extern_clear_fifo();
				return RETURN_ERROR;
			}
		}
		us_nop_delay(50000);
	}
	TRACE_WP("[AT]Time out AT cmd\r\n");
	at_cmd_extern_find_cmd_time_out();
	g_at_analysis.at_len = 0;	
	uart_fifo_extern_clear_fifo();
	return RETURN_ERROR;
}



//第二部设置特征值
UCHAR8 at_cmd_extern_config_hex_eigenvalue(void)
{
	int i;
	int j;
	UCHAR8 ret;
	UCHAR8 dat;
	UCHAR8 *p_sku = NULL;
	UCHAR8 start_cmd_buf[100] = {0};
	TRACE_WP("******************* at_cmd_extern_config_hex_eigenvalue ***************************** \r\n");


	p_sku = system_run_ram.usb_sku;




	sprintf(start_cmd_buf,"config hex eigenvalue %02X %02X %02X %02X %02X %02X\r\n",p_sku[0],p_sku[1],p_sku[2],p_sku[3],p_sku[4],p_sku[5]);
	TRACE_WP("start_cmd_buf = %s \r\n",start_cmd_buf);
	uart_fifo_extern_clear_fifo();
	g_at_analysis.at_len = 0;	
	uart_extern_burn_and_at_uart_tx_buf(start_cmd_buf,strlen(start_cmd_buf));
	us_nop_delay(10000);

	for(i=0;i<30;i++)
	{
		while(1)
		{
			if ( 0==uart_fifo_extern_get_fifo_count() )	
			{
				break;		
			}
			dat = uart_fifo_extern_get_rx_dat();
			ret = at_cmd_extern_analysis_uart_fifo(dat);
			if( ret == 1 )
			{
				if ( NULL != StrFindSubString( g_at_analysis.at_buf, (UCHAR8 *)("done") ) )
				{
					TRACE_WP("[AT]%s \r\n", g_at_analysis.at_buf  );
					uart_fifo_extern_clear_fifo();
					g_at_analysis.at_len = 0;	
					TRACE_WP("at_cmd_extern_config_hex_eigenvalue ok\r\n" );
					return RETURN_SUCCESS;
				}
				else
				{
					TRACE_WP("[AT]Error AT cmd len:%d::%s \r\n", g_at_analysis.at_len,g_at_analysis.at_buf  );
					at_cmd_extern_find_cmd_error();
					g_at_analysis.at_len = 0;	
				}
			}
			else if( ret == 2 )
			{
				TRACE_WP("[AT]ACK AT cmd ERROR len:%d::%s \r\n", g_at_analysis.at_len,g_at_analysis.at_buf  );
				at_cmd_extern_find_cmd_error();
				g_at_analysis.at_len = 0;	
				uart_fifo_extern_clear_fifo();
				return RETURN_ERROR;
			}
		}
		us_nop_delay(50000);
	}
	TRACE_WP("[AT]Time out AT cmd\r\n");
	at_cmd_extern_find_cmd_time_out();
	g_at_analysis.at_len = 0;	
	uart_fifo_extern_clear_fifo();
	return RETURN_ERROR;
}



//最后保存参数 准备接收
UCHAR8 at_cmd_extern_config_save(void)
{
	int i;
	int j;
	UCHAR8 ret;
	UCHAR8 dat;
	UCHAR8 start_cmd_buf[100] = {0};
	ms_nop_delay(100);
	TRACE_WP("******************* at_cmd_extern_config_save ***************************** \r\n");
	sprintf(start_cmd_buf,"config save\r\n");
	uart_fifo_extern_clear_fifo();
	g_at_analysis.at_len = 0;	
	uart_extern_burn_and_at_uart_tx_buf(start_cmd_buf,strlen(start_cmd_buf));

	for(i=0;i<30;i++)
	{
		while(1)
		{
			if ( 0==uart_fifo_extern_get_fifo_count() )	
			{
				break;		
			}
			dat = uart_fifo_extern_get_rx_dat();
			ret = at_cmd_extern_analysis_uart_fifo(dat);
			if( ret == 1 )
			{
				if ( NULL != StrFindSubString( g_at_analysis.at_buf, (UCHAR8 *)("done") ) )
				{
					TRACE_WP("[AT]%s \r\n", g_at_analysis.at_buf  );
					uart_fifo_extern_clear_fifo();
					g_at_analysis.at_len = 0;	
					TRACE_WP("at_cmd_extern_config_save ok\r\n" );
					return RETURN_SUCCESS;
				}
				else
				{
					TRACE_WP("[AT]Error AT cmd len:%d::%s \r\n", g_at_analysis.at_len,g_at_analysis.at_buf  );
					g_at_analysis.at_len = 0;	
				}
			}
			else if( ret == 2 )
			{
				TRACE_WP("[AT]ACK AT cmd ERROR len:%d::%s \r\n", g_at_analysis.at_len,g_at_analysis.at_buf  );
				at_cmd_extern_find_cmd_error();
				g_at_analysis.at_len = 0;	
				uart_fifo_extern_clear_fifo();
				return RETURN_ERROR;
			}
		}
		us_nop_delay(50000);
	}
	TRACE_WP("[AT]Time out AT cmd\r\n");
	at_cmd_extern_find_cmd_time_out();
	g_at_analysis.at_len = 0;	
	uart_fifo_extern_clear_fifo();
	return RETURN_ERROR;
}



UCHAR8 at_cmd_extern_rssi_stop_test(void)
{
	int i;
	int j;
	UCHAR8 ret;
	UCHAR8 dat;
	UCHAR8 start_cmd_buf[] = {"dongle stop\r\n"};
	TRACE_WP("******************* at_cmd_extern_rssi_stop_test ***************************** \r\n");
	uart_fifo_extern_clear_fifo();
	g_at_analysis.at_len = 0;	
	uart_extern_burn_and_at_uart_tx_buf(start_cmd_buf,strlen(start_cmd_buf));
	us_nop_delay(10000);

	for(i=0;i<30;i++)
	{
		while(1)
		{
			if ( 0==uart_fifo_extern_get_fifo_count() )	
			{
				break;		
			}

			dat = uart_fifo_extern_get_rx_dat();
			ret = at_cmd_extern_analysis_uart_fifo(dat);
			if( ret == 1 )
			{
				if ( NULL != StrFindSubString( g_at_analysis.at_buf, (UCHAR8 *)("done") ) )
				{

					TRACE_WP("[AT]%s \r\n", g_at_analysis.at_buf  );
					g_at_analysis.at_len = 0;	
					TRACE_WP("at_cmd_extern_rssi_stop_test OK\r\n");
					return RETURN_SUCCESS;
				}
				else
				{
					TRACE_WP("[AT]Error AT cmd len:%d::%s \r\n", g_at_analysis.at_len,g_at_analysis.at_buf  );
					g_at_analysis.at_len = 0;	
				}
			}
			else if( ret == 2 )
			{
				TRACE_WP("[AT]ACK AT cmd ERROR len:%d::%s \r\n", g_at_analysis.at_len,g_at_analysis.at_buf  );
				at_cmd_extern_find_cmd_error();
				g_at_analysis.at_len = 0;	
				uart_fifo_extern_clear_fifo();
				return RETURN_ERROR;
			}
		}
		ms_nop_delay(50);
	}
	TRACE_WP("[AT]Time out AT cmd\r\n");
	at_cmd_extern_find_cmd_time_out();
	g_at_analysis.at_len = 0;	
	uart_fifo_extern_clear_fifo();
	return RETURN_ERROR;
}



UCHAR8 at_cmd_extern_rssi_start_test(void)
{
	int i;
	int j;
	UCHAR8 ret;
	UCHAR8 dat;
	UCHAR8 start_cmd_buf[] = {"dongle start\r\n"};
	TRACE_WP("******************* into at_cmd_extern_rssi_start_test ***************************** \r\n");
	uart_fifo_extern_clear_fifo();
	g_at_analysis.at_len = 0;	
	uart_extern_burn_and_at_uart_tx_buf(start_cmd_buf,strlen(start_cmd_buf));
	us_nop_delay(10000);
	for(i=0;i<30;i++)
	{
		while(1)
		{
			if ( 0==uart_fifo_extern_get_fifo_count() )	
			{
				break;		
			}

			dat = uart_fifo_extern_get_rx_dat();
			ret = at_cmd_extern_analysis_uart_fifo(dat);
			if( ret == 1 )
			{
				if ( NULL != StrFindSubString( g_at_analysis.at_buf, (UCHAR8 *)("done") ) )
				{

					TRACE_WP("[AT]%s \r\n", g_at_analysis.at_buf  );
					g_at_analysis.at_len = 0;	
					TRACE_WP("at_cmd_extern_rssi_start_test OK\r\n");
					return RETURN_SUCCESS;
				}
				else
				{
					TRACE_WP("[AT]Error AT cmd len:%d::%s \r\n", g_at_analysis.at_len,g_at_analysis.at_buf  );
					g_at_analysis.at_len = 0;	
				}
			}
			else if( ret == 2 )
			{
				TRACE_WP("[AT]ACK AT cmd ERROR len:%d::%s \r\n", g_at_analysis.at_len,g_at_analysis.at_buf  );
				at_cmd_extern_find_cmd_error();
				g_at_analysis.at_len = 0;	
				uart_fifo_extern_clear_fifo();
				return RETURN_ERROR;
			}
		}
		ms_nop_delay(50);
	}
	TRACE_WP("[AT]Time out AT cmd\r\n");
	at_cmd_extern_find_cmd_time_out();
	g_at_analysis.at_len = 0;	
	uart_fifo_extern_clear_fifo();
	return RETURN_ERROR;
}



UCHAR8 at_cmd_extern_get_rssi_mac(void)
{
	int i;
	int j;
	UCHAR8 ret;
	UCHAR8 dat;
	UCHAR8 receive_buf[150];
	signed char rssi;
	UCHAR8 *p_receie = NULL;
	char *p_rssi = NULL;
	UCHAR8 crc = 0;
	UCHAR8 *p = NULL;
	TRACE_WP("******************* into at_cmd_extern_get_rssi_mac ***************************** \r\n");


	for(i=0;i<150;i++)
	{
		while(1)
		{
			if ( 0==uart_fifo_extern_get_fifo_count() )	
			{
				// FIFO ��������
				break;		
			}

			// �ӽ��ջ����ȡһ���ַ�
			dat = uart_fifo_extern_get_rx_dat();	
			// Э�����
			ret = at_cmd_extern_analysis_uart_fifo(dat);
			if( ret == 1 )
			{
 				if( NULL != StrFindSubString( g_at_analysis.at_buf, (UCHAR8 *)("read broadcast success <") ) )
				{
					p_receie = StrFindSubString( g_at_analysis.at_buf, (UCHAR8 *)("read broadcast success <") );
					p_receie = p_receie + strlen("read broadcast success <");
					TRACE_WP("p_receie %s\r\n",p_receie);
					p_rssi = strstr( (char *)g_at_analysis.at_buf, ">rssi");
					if(p_rssi != NULL)
					{
						sscanf(p_rssi,">rssi %d",&system_run_ram.dev_work_state.read_rssi);	
						TRACE_WP("system_run_ram.dev_work_state.read_rssi %d\r\n",system_run_ram.dev_work_state.read_rssi);

						*p_rssi = ' ';
						*(p_rssi + 1) = 0;
						p = receive_buf;
						sscanf(p_receie,"%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x ",p,p+1,
						p+2,p+3,p+4,p+5,p+6,p+7,p+8,p+9,p+10,p+11,p+12,p+13,p+14,p+15,p+16,p+17,p+18,p+19,p+20,p+21,p+22);
						for(j = 0; j < 23 ; j++)
						{
							TRACE_WP("receive_buf[%d] = %x\r\n",j,receive_buf[j])
						}

						//检测CRC
						TRACE_WP("CRC receive_buf[22] = %x  factory_crc(receive_buf,22) = %x\r\n",receive_buf[22], factory_crc(receive_buf,22));
						if(receive_buf[22] != factory_crc(receive_buf,22))
						{
							TRACE_WP("ERROR CRC receive_buf[22] = %x  factory_crc(receive_buf,22) = %x\r\n",receive_buf[22], factory_crc(receive_buf,22));
							g_at_analysis.at_len = 0;	
							return RETURN_ERROR;
						}
						// //检测特征值
						// for(j = 0; j < 6 ; j++)
						// {
						// 	if(receive_buf[j] != sku_buf[j])
						// 	{
						// 		TRACE_WP("ERROR sku j = %d receive_buf[j] = %x  sku_buf[j] = %x\r\n",j,receive_buf[j], sku_buf[j]);
						// 		g_at_analysis.at_len = 0;	
						// 		return RETURN_ERROR;
						// 	}
						// }
						//判断RSSI
						if(system_run_ram.dev_work_state.read_rssi >= system_run_ram.usb_seng_rssi)
						{	
							TRACE_WP("ERROR rssi  :read_rssi = %d usb_seng_rssi = %d \r\n",system_run_ram.dev_work_state.read_rssi, system_run_ram.usb_seng_rssi);
						}
						//PID前面标志的

						//复制mac
						for(j = 0; j < 8 ; j++)
						{
							system_run_ram.dev_work_state.read_mac[j] = receive_buf[6+j];
						}
						//复制PID
						for(j = 0; j < 7 ; j++)
						{
							system_run_ram.dev_work_state.read_pid[j] = receive_buf[15+j];
						}
						TRACE_WP("read mac rssi  success\r\n");
						system_run_ram.dev_work_state.rssi_get_rssi = 1;
						return RETURN_SUCCESS;
					}
					else
					{
						TRACE_WP("[AT]Error AT cmd len:%d::%s \r\n", g_at_analysis.at_len,g_at_analysis.at_buf  );
						g_at_analysis.at_len = 0;		
					}
				}
				else
				{
					// Э�鴦����ɺ��������������ݰ�����
					TRACE_WP("[AT]Error AT cmd len:%d::%s \r\n", g_at_analysis.at_len,g_at_analysis.at_buf  );		
					g_at_analysis.at_len = 0;	
				}
			}
			else if( ret == 2 )
			{
				// Э�鴦����ɺ��������������ݰ�����
				TRACE_WP("[AT]ACK AT cmd ERROR len:%d::%s \r\n", g_at_analysis.at_len,g_at_analysis.at_buf  );
				at_cmd_extern_find_cmd_error();
				g_at_analysis.at_len = 0;	
				uart_fifo_extern_clear_fifo();
				return RETURN_ERROR;
			}
		}
		// ��Чָ��ֱ�ӷ���
		vTimeJustDelayMs(50);
	}
	// ָ�ʱ
	TRACE_WP("[AT]Time out AT cmd\r\n");
	at_cmd_extern_find_cmd_time_out();
	g_at_analysis.at_len = 0;	
	uart_fifo_extern_clear_fifo();
	return RETURN_ERROR;
}

UCHAR8 PrvGetMacPid(UCHAR8 *input_data,UCHAR8 input_len)
{
	UCHAR8 i = 0;	
	UCHAR8 j = 0;	
	UCHAR8 k = 0;
	UCHAR8 pid_buf[12] = {0x5D,0x5A,0xC5,0xF0,0x49,0xD2,0x1E,0xF7,0x11,0x27};

	if(input_data == NULL)
	{
		return 0;
	}
	for(i = 0 ; i < input_len ; i ++)
	{
		if(input_data[i] == pid_buf[0])
		{
			for(j = 0 ; j <  10 ; j ++)
			{
				if(input_data[i+j] != pid_buf[j] )
				{
					TRACE_WP("J = %d,input_data = %02x,pid_buf = %02x\r\n",j,input_data[i+j],pid_buf[j]);	
					break;
				}
				
			}
			if(j >= 10)
			{
				system_run_ram.dev_work_state.read_rssi = input_data[i - 12];
				TRACE_WP("system_run_ram.dev_work_state.read_rssi = %d\r\n",system_run_ram.dev_work_state.read_rssi);
				TRACE_WP("system_run_ram.usb_seng_rssi = %d\r\n",system_run_ram.usb_seng_rssi);
				if(system_run_ram.dev_work_state.read_rssi < system_run_ram.usb_seng_rssi)
				{
					TRACE_WP("RSSI ERROR \r\n");
					return 0;	
				}

				
				memset(system_run_ram.dev_work_state.read_mac,0,10);
				memset(system_run_ram.dev_work_state.read_pid,0,10); 
				for(k = 0 ; k < 8 ; k++)
				{
					system_run_ram.dev_work_state.read_mac[k] = input_data[10+i+k];
					system_run_ram.dev_work_state.read_pid[k] = input_data[19+i+k];
					
				}
				TRACE_WP("system_run_ram.dev_work_state.read_mac[k] ");
				for(k = 0 ; k < 8 ; k++)
				{
					TRACE_WP("%02X ",system_run_ram.dev_work_state.read_mac[k]);
				}
				TRACE_WP("\r\n");

				TRACE_WP("system_run_ram.dev_work_state.read_pid\r\n");
				for(k = 0 ; k < 8 ; k++)
				{
					TRACE_WP("%02X",system_run_ram.dev_work_state.read_pid[k]);
				}
				TRACE_WP("\r\n");
				system_run_ram.dev_work_state.read_pid_mac_sign = 1;
				return 1;
			}
		}
	}
	TRACE_WP("i = %d\r\n",i);	
	return 0;
}

static char at_cmd_static_check_rssi_cmd_format(UCHAR8 tmp)
{	
	UCHAR8 a;	
    static UCHAR8 pkt_flag = 0x00;
 	if ( 0 == pkt_flag)
	{
		//�ȴ����ָ���־ '='�� ��ʼ����
		if ( 0x3d == tmp )
		{
			pkt_flag = 1;
			rssi_ptk.lenth = 0;
		}
	} 
	else
	{
		rssi_ptk.dat[rssi_ptk.lenth] = tmp;
		rssi_ptk.lenth++;
		//���Ⱥ���
		if(rssi_ptk.dat[4] == rssi_ptk.lenth - 5)
		{
			TRACE_WP("rssi_ptk.dat[4] = %d\r\n",rssi_ptk.dat[4]);
			if(PrvGetMacPid(rssi_ptk.dat, rssi_ptk.lenth ) == 1)
			{
				pkt_flag = 0; 
				return 1;	
			}	
			else
			{
				pkt_flag = 0; 
				//ʶ��ʧ��
				return 2;
			}
		}

	}
	//��δ���յ����������ݾͷ���0 ��ʾ��Ҫ��������
	return 0;
}

UCHAR8 at_cmd_extern_rssi_get_rssi(void)
{
	int i,j;
	UCHAR8 ret;
	UCHAR8 dat;
	UCHAR8 get_pid_cmd_buf[30] = {"AT+GET_PID?\r\n"};    
	UCHAR8 pid_result;	


	TRACE_WP("******************* into at_cmd_extern_rssi_get_rssi ***************************** \r\n");

	uart_fifo_extern_clear_fifo();
	//������תָ��

	uart_extern_burn_and_at_uart_tx_buf(get_pid_cmd_buf,strlen(get_pid_cmd_buf));

	us_nop_delay(10000);

	for(i=0;i<10;i++)
	{
		while(1)
		{
			if ( 0==uart_fifo_extern_get_fifo_count() )	
			{
				// FIFO ��������
				break;		
			}

			// �ӽ��ջ����ȡһ���ַ�
			dat = uart_fifo_extern_get_rx_dat();

			TRACE_WP("dat = %c %02x \r\n",dat,dat);

			ret = at_cmd_static_check_rssi_cmd_format(dat);

			if( ret == 1 )
			{	
				uart_fifo_extern_clear_fifo();
				system_run_ram.dev_work_state.rssi_get_rssi = 1;
				// Э�鴦����ɺ��������������ݰ�����
				g_at_analysis.at_len = 0;	
				return RETURN_SUCCESS;
				
			}
			else if(ret == 2)
			{
				// Э�鴦����ɺ��������������ݰ�����
				TRACE_WP("[AT]Error AT cmd len:%d::%s \r\n", g_at_analysis.at_len,g_at_analysis.at_buf  );
				at_cmd_extern_find_cmd_error();
				g_at_analysis.at_len = 0;	
				uart_fifo_extern_clear_fifo();
				return RETURN_ERROR;
			}
		
		}
		// ��Чָ��ֱ�ӷ���
		us_nop_delay(50000);
	}
	// ָ�ʱ
	TRACE_WP("[AT]Time out AT cmd\r\n");
	at_cmd_extern_find_cmd_time_out();
	uart_fifo_extern_clear_fifo();
	return RETURN_ERROR;
}

UCHAR8 at_cmd_extern_send_informa_to_pc( void )
{
	int i,k,j;
	UCHAR8 ret;
	UCHAR8 dat;
	UCHAR8 cmd_data = 0xB8;
	UCHAR8 sub_cmd_data = 0x00;
	UCHAR8 seq_cmd_data = 0x00;
	UCHAR8 corotation_cmd_buf[100] = {0}; 
	UCHAR8 sonoff_cmd_buf[10] = {0x53,0x4F,0x6E,0x6F,0x46,0x66,0x32,0x32 };
	TRACE_WP(" ############ into at_cmd_extern_send_informa_to_pc  ############\r\n");

	k = 0;
	uart_fifo_extern_clear_fifo_usb(); 
	corotation_cmd_buf[k++] = 0x7F; //head
	corotation_cmd_buf[k++] = 0; 	//len
	corotation_cmd_buf[k++] = cmd_data;
	corotation_cmd_buf[k++] = sub_cmd_data;
	corotation_cmd_buf[k++] = seq_cmd_data;

	for(i = 0 ; i < 8 ; i++)
	{
		corotation_cmd_buf[k++] = sonoff_cmd_buf[i];
	}


	for(i = 0 ; i < 8 ; i++)
	{
		corotation_cmd_buf[k++] = system_run_ram.dev_work_state.read_mac[7-i];	
	}
	
	for(i = 0 ; i < 7 ; i++) // FF FE D5 6E E2 27 11 F7
	{
		corotation_cmd_buf[k++] = system_run_ram.dev_work_state.read_pid[i];
	}
	corotation_cmd_buf[k++] =  system_run_ram.dev_work_state.read_rssi;

	corotation_cmd_buf[1] = k+1;
	
	corotation_cmd_buf[k] = factory_crc(&corotation_cmd_buf[1], corotation_cmd_buf[1] - 2);

	us_nop_delay(100000);

	usb_extern_txrx_send_buf(corotation_cmd_buf,corotation_cmd_buf[1]);
	g_at_analysis_usb.at_len = 0;	

	for(i=0;i<100;i++)
	{
		while(1)
		{
			if ( 0==uart_fifo_extern_get_fifo_count_usb() )	
			{
				break;		
			}
			dat = uart_fifo_extern_get_rx_dat_usb();	
			ret = at_cmd_extern_analysis_uart_fifo_usb(dat);

			if(ret == 1)
			{
				if( (g_at_analysis_usb.at_buf[2] == cmd_data) && (g_at_analysis_usb.at_buf[3] ==  0x80) && ( g_at_analysis_usb.at_buf[5] ==  0x00 ) )
				{
					TRACE_WP("************** [AT] READ at_cmd_extern_send_informa_to_pc OK *******************\r\n");	
					uart_fifo_extern_clear_fifo_usb();
					// Э�鴦����ɺ��������������ݰ�����
					g_at_analysis_usb.at_len = 0;	
					system_run_ram.dev_work_state.send_informa_to_pc = 0;
					system_run_ram.at_bl_chip_fail_tick = 0;
					bouffalo_isp_extern_device_end_burn();
//					ui_extern_refresh();
//					ui_extern_main_handle(); 
					ms_nop_delay(500);
					g_at_analysis.at_len = 0;	
					uart_fifo_extern_clear_fifo();
					return RETURN_SUCCESS;
				}
				else
				{
					// Э�鴦����ɺ��������������ݰ�����
					TRACE_WP("[AT]Error AT cmd len g_at_analysis_usb.at_buf[2] = %02x\r\n",g_at_analysis_usb.at_buf[2]);
					TRACE_WP("[AT]Error AT cmd len g_at_analysis_usb.at_buf[3] = %02x\r\n",g_at_analysis_usb.at_buf[3]);
					TRACE_WP("[AT]Error AT cmd len g_at_analysis_usb.at_buf[5] = %02x\r\n",g_at_analysis_usb.at_buf[5]);
					at_cmd_extern_find_cmd_error();
					g_at_analysis_usb.at_len = 0;
					uart_fifo_extern_clear_fifo();
					return RETURN_ERROR;
				}
			}
			else if( ret == 2 )
			{
				// Э�鴦����ɺ��������������ݰ�����
				TRACE_WP("[AT]ACK AT cmd ERROR len:%d::%s \r\n", g_at_analysis_usb.at_len,g_at_analysis_usb.at_buf  );
				at_cmd_extern_find_cmd_error();
				g_at_analysis_usb.at_len = 0;	
				uart_fifo_extern_clear_fifo();
				return RETURN_ERROR;
			}
		}
		// ��Чָ��ֱ�ӷ���
		ms_nop_delay(30);
	}
	// ָ�ʱ
	TRACE_WP("[AT]Time out AT cmd\r\n");
	at_cmd_extern_find_cmd_time_out();
	g_at_analysis_usb.at_len = 0;
	uart_fifo_extern_clear_fifo();
	return RETURN_ERROR;
}


void at_cmd_extern_pkt_main_handle(void)
{
	uint8_t dat;
	uint8_t ret;

	if ( SYSTEM_FLAG_100MS_EVENT == (sys_work_info & SYSTEM_FLAG_100MS_EVENT))
	{
		// 100ms  event 
		sys_work_info &= (~SYSTEM_FLAG_100MS_EVENT);
		at_cmd_extern_into_factory_handle();
	}

	while(1)
	{
		if ( 0==uart_fifo_extern_get_fifo_count() )	
		{
			break;		
		}
	
		dat = uart_fifo_extern_get_rx_dat();
		//printf("dat = %02x\r\n",dat);
	
		ret = at_cmd_extern_analysis_uart_fifo(dat);
		if( ret == 0 )
		{
			// ��Чָ��ֱ�ӷ���	
			return;
		}
		// Э�鴦����ɺ��������������ݰ�����
		g_at_analysis_usb.at_len = 0;	
	}
}







void at_cmd_extern_into_factory_handle_usb()
{
	UCHAR8 ret;
	UCHAR8 i;
	UCHAR8 j;
	UINT32 time_tick;
	static int rssi_tick;

	ui_extern_refresh();
	ui_extern_main_handle();
	

}

void at_cmd_extern_pkt_main_handle_usb(void)
{
	UCHAR8 dat;
	UCHAR8 ret;
	UCHAR8 re_ret;
	UCHAR8 i,j,k,l;
	uint32_t at_len;
	uint8_t at_buf[AT_CMD_ANALYSIS_BUF_LEN];
	uint32_t Max_receive_length = 70;

	if(0 != uart_fifo_extern_get_fifo_count_usb())
	{
		EXTERNAL_UART_SET_MCU;
		ms_nop_delay(50);
		g_at_analysis_usb.at_len = 0;
		
		for(i = 0; i < Max_receive_length; i++)
		{
			 dat = uart_fifo_extern_get_rx_dat_usb();
			 //TRACE_WP("dat = %02x\r\n",dat);
			 ret = at_cmd_extern_analysis_uart_fifo_usb(dat);
			 if ( g_at_analysis_usb.at_len >= AT_CMD_ANALYSIS_BUF_LEN )	
			 {
				 //TRACE_WP("[AT CMD]lost at more len %d\r\n",AT_CMD_ANALYSIS_BUF_LEN);
				 g_at_analysis_usb.at_len = 0;		
			 }
		}
	
		//TRACE_WP( "--->ret : %d \r\n", ret );
		//TRACE_WP( "--->len : %d \r\n", g_at_analysis_usb.at_len );
//		TRACE_WP( "--->buf : ");
		for(j = 0; j < g_at_analysis_usb.at_len; j++)
		{
//			TRACE_WP( "%02x ", g_at_analysis_usb.at_buf[j] );
			at_buf[j] = g_at_analysis_usb.at_buf[j];
		}
//		TRACE_WP( "\r\n");
		at_len = g_at_analysis_usb.at_len;
		
		//判断指令函数
		for(k = 0; k < at_len; k++)
		{
			if((at_buf[k] == 0x7f) && (at_buf[k + 1] != 0x00))
			{
				g_at_analysis_usb.at_len = at_buf[k + 1];
				TRACE_WP( "--->len : %d \r\n", g_at_analysis_usb.at_len );
				memset(g_at_analysis_usb.at_buf, 0, sizeof(g_at_analysis_usb.at_buf));
				TRACE_WP( "--->buf : ");
				for(l = 0; l < at_buf[k + 1]; l++)
				{
					g_at_analysis_usb.at_buf[l] = at_buf[k + l];
					TRACE_WP( "%02x ", g_at_analysis_usb.at_buf[l] );
				}
				TRACE_WP("\r\n");
				re_ret = at_cmd_extern_analysis_special_cmd_find_usb();
				TRACE_WP( "--->re_ret : %d \r\n", re_ret );
				if(re_ret == 2)
				{
					TRACE_WP( "CRC VERIFICATION ERROR\r\n" );
					LED_R_ON;
				}
			}
		}
		uart_fifo_extern_clear_fifo_usb();
		TRACE_WP( "\r\n-------------------------------------------\r\n");
		
		EXTERNAL_UART_SET_DUT;
		ms_nop_delay(50);
	}
}



uint8_t cmd_init_dongle(void)
{
	UCHAR8 ret;
	ret = at_cmd_extern_read_dongle_type();
	if(ret != RETURN_SUCCESS)
	{
		ms_nop_delay(200);
		ret =  at_cmd_extern_config_dongle_type();
		if(ret == RETURN_ERROR)
		{
			TRACE_WP("at_cmd_extern_read_dongle_type ERROR\r\n");
			return 0;
		}
		
		ms_nop_delay(200);
		ret =   at_cmd_extern_config_save();
		if(ret == RETURN_ERROR)
		{
			TRACE_WP(" at_cmd_extern_config_save  ERROR\r\n");
			return 0;
		}
	}	
	ms_nop_delay(200);
	ret = at_cmd_extern_read_hex_eigenvalue();
	if(ret == RETURN_ERROR) 
	{
		TRACE_WP("at_cmd_extern_read_hex_eigenvalue ERROR\r\n");
	}
	return 0;
}


void at_cmd_extern_into_factory_handle()
{
	UCHAR8 ret;
	UCHAR8 i;
	UCHAR8 j;
	UINT32 time_tick;
//	static int rssi_tick;
	UCHAR8  send_test_success_result[20] = {0x7F,0x07,0x80,0xC6,0x00,0x00,0x41};
	UCHAR8  send_test_failure_result[20] = {0x7F,0x07,0x80,0xC6,0x00,0x01,0x40};

	// if( !system_run_ram.dev_work_state.rssi_get_rssi )
	// {
	// 	ret = at_cmd_extern_get_rssi_mac();
	// 	if( ret != RETURN_SUCCESS )
	// 	{
	// 		return ;
	// 	}
	// }

	if( system_run_ram.test_io_success == 1 )
	{
		EXTERNAL_UART_SET_MCU;
		ms_nop_delay(50);
		
		// IO 检测
		ret = 1;
		for(i = 0; i < IO_MAX_IN_COUNT_NUM; i++)
		{
			TRACE_WP("Index: %d, state: %d, time: %d ms, mark_time: %d ms\r\n",
				i,io_test_state[i].en_key_down, io_test_state[i].key_down_time, io_test_state[i].mark_time);
			// 是否可用判断
			if( io_test_state[i].en_key_down != 1 )
			{
				ret = 0;
				break;
			}

			// 时间判断  50ms
			if( ( io_test_state[i].key_down_time > 40 ) && ( io_test_state[i].key_down_time < 70 ) )
			{
			}
			else
			{
				ret = 0;
				break;
			}

			// time_tick 计算
			for(j = 0; j < IO_MAX_IN_COUNT_NUM; j++)
			{
				if(j == i)
				{
					continue;
				}
				if( io_test_state[i].mark_time > io_test_state[j].mark_time )
				{
					time_tick = io_test_state[i].mark_time - io_test_state[j].mark_time;
				}
				else
				{
					time_tick = io_test_state[j].mark_time - io_test_state[i].mark_time;
				}
				// 时间判断 time_tick是两个IO口=流水的间隔加上流水的时间 也就是100ms 这里保守为小于80就报错  对于没有时间间隔的 该判断可以写为40
				if( time_tick < 40 )  
				{
					ret = 0;
					TRACE_WP("Fail Time error i: %d, j: %d, time: %d\r\n",i,j,time_tick);
					break;
				}
			}
		}

		if(ret == 1)
		{
			system_run_ram.test_io_success = 0;
			system_run_ram.need_test_io = 0;
			usb_extern_txrx_send_buf(send_test_success_result,send_test_success_result[1]);
			TRACE_WP("IO TEST success\r\n");	
		}
		else
		{
//			system_run_ram.test_io_success = 0;
//			system_run_ram.need_test_io = 0;
//			usb_extern_txrx_send_buf(send_test_failure_result,send_test_failure_result[1]);
			TRACE_WP("IO TEST fail\r\n");
			return;
		}
		
		EXTERNAL_UART_SET_DUT;
		ms_nop_delay(50);
	}

//	if( system_run_ram.dev_work_state.send_informa_to_pc == 1 )
//	{
//		ret = at_cmd_extern_send_informa_to_pc();
//		if( ret != RETURN_SUCCESS )
//		{
//			return ;
//		}
//	}
}



#define START_BURN_BL
/************************************************************************************************
* Functional 	:	
* Input		:   	None
* Output		:	None
* Return 		: 	None
* Notes		:
* Modification	:   
* Modification date		Ver		       	Name		 		Modify 
*  ----------------------------------------------------------------------------
* 20190703		       V1.0 			 Alex		       	Establish
**************************************************************************************************/
void bouffalo_isp_extern_device_end_burn(void)
{
	EXTERNAL_POWER5V_OFF;
	EXTERNAL_POWER3V3_OFF;
	EXTERNAL_CC1_CTR_DOWN;
	EXTERNAL_CC2_CTR_DOWN;
	LED_OFF;
//	DONGLE_POWER_OFF;
//	EXTERNAL_RELAY_POWER_OFF;
	// �����¼
	system_run_ram.burn_and_test_result = 1;

	// �ر�IO ���
	system_run_ram.need_test_io = 0;
//	system_run_ram.start_testing = 0;	
	// ����������¼
	system_run_ram.en_burn_bl_chip = 0;
	// ��¼�쳣����
	system_run_ram.burn_bl_fail_tick = 0;
	system_run_ram.at_bl_chip_fail_tick = 0;
	// ����ģʽ�л� 1  ��¼ģʽ  2  AT ָ��ģʽ
	system_run_ram.wait_chip_work_mode = 0;
	// ��ʱʱ��
	system_run_ram.burn_time_out_tick = 0;

	system_run_ram.success_burn_num++;

	system_run_ram.end_at_cmd_time = time_system_now_time;
	TRACE_WP("######################################################################\r\n");
	TRACE_WP("[ISP]Success BURN BL Chip,All Burn Time is:%d ms , tick = %d , fail = %d\r\n",
		system_run_ram.end_at_cmd_time-system_run_ram.start_bootload_time,
		system_run_ram.success_burn_num,system_run_ram.fail_burn_num);

	system_run_ram.chip_burn_time = system_run_ram.end_at_cmd_time-
			system_run_ram.start_bootload_time;
//	ui_extern_refresh();
				
	TRACE_WP("[end]bouffalo_isp_extern_device_end_burn\r\n");
}

/************************************************************************************************
* Functional 	:	
* Input		:   	None
* Output		:	None
* Return 		: 	None
* Notes		:
* Modification	:   
* Modification date		Ver		       	Name		 		Modify 
*  ----------------------------------------------------------------------------
* 20190703		       V1.0 			 Alex		       	Establish
**************************************************************************************************/
void bouffalo_isp_extern_device_fail_burn( void )
{
	EXTERNAL_POWER5V_OFF;
	EXTERNAL_POWER3V3_OFF;
	EXTERNAL_CC1_CTR_DOWN;
	EXTERNAL_CC2_CTR_DOWN;
	LED_OFF;
//	DONGLE_POWER_OFF;
//	EXTERNAL_RELAY_POWER_OFF;

//	at_cmd_extern_send_test_error_to_pc();
//	system_run_ram.dev_work_state.usb_seng_informa = 0 ;
//	system_run_ram.start_testing = 0;	
	system_run_ram.need_test_io = 0;

	system_run_ram.burn_and_test_result = 2;


	system_run_ram.en_burn_bl_chip = 0;

	system_run_ram.burn_bl_fail_tick = 0;
	system_run_ram.at_bl_chip_fail_tick = 0;

	system_run_ram.wait_chip_work_mode = 0;

	system_run_ram.burn_time_out_tick = 0;

	system_run_ram.fail_burn_num++;

	system_run_ram.end_at_cmd_time = time_system_now_time;
	TRACE_WP("######################################################################\r\n");
	TRACE_WP("[ISP]ERROR BURN BL Chip,All Burn Time is:%d ms , tick = %d , fail = %d\r\n",
		system_run_ram.end_at_cmd_time-system_run_ram.start_bootload_time,
		system_run_ram.success_burn_num,system_run_ram.fail_burn_num);


	system_run_ram.chip_burn_time = system_run_ram.end_at_cmd_time-
			system_run_ram.start_bootload_time;
//	ui_extern_refresh();

	TRACE_WP("####################\r\n"); 
	TRACE_WP(" Fail burn device \r\n");
	TRACE_WP("####################\r\n");
}




