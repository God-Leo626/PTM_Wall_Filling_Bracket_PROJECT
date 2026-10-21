/*
 * @Author: “HP “1357243695@qq.com”
 * @Date: 2023-11-10 14:48:34
 * @LastEditors: “HP “1357243695@qq.com”
 * @LastEditTime: 2025-02-25 14:47:52
 * @FilePath: \镭雕\USER\main.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "user.h"


void prvPintfSoftWare(void)
{
	TRACE_WP("[main]mcu software :%s,%s,%s\r\n",VERSION, __DATE__, __TIME__);
}


int main(void)
{
	init_extern_system_init();
  prvPintfSoftWare();
	while (1)
	{
		key_extern_monitor_handle();
		if(system_run_ram.wait_chip_work_mode == 1)
		{
			
		}
//		at_cmd_extern_pkt_main_handle_usb();
//		at_cmd_extern_temperature_humidity_send();	
//		ms_nop_delay(100);
	}
}





void HardFault_Handler(void)
{
	uint32_t 	r_psp,r_msp ;
	uint32_t	* sp;
	
	WARN_NOUSER(r_psp);
	WARN_NOUSER(sp);

  	r_psp = __get_PSP(); 
  	r_msp = __get_MSP(); 
  	sp = (uint32_t *)r_msp;

  	TRACE_WP("[HardFault]hard fault psp=%08X,msp=%08X\r\n",r_psp,r_msp);
  	TRACE_WP("[HardFault]R0=%08X\r\n",*sp++);
  	TRACE_WP("[HardFault]R1=%08X\r\n",*sp++);
  	TRACE_WP("[HardFault]R2=%08X\r\n",*sp++);
  	TRACE_WP("[HardFault]R3=%08X\r\n",*sp++);
  	TRACE_WP("[HardFault]R12=%08X\r\n",*sp++);
  	TRACE_WP("[HardFault]LR=%08X\r\n",*sp++);
  	TRACE_WP("[HardFault]PC=%08X\r\n",*sp++);
  	TRACE_WP("[HardFault]PSR=%08X\r\n",*sp++);
	while (1)
	{
	}
}


