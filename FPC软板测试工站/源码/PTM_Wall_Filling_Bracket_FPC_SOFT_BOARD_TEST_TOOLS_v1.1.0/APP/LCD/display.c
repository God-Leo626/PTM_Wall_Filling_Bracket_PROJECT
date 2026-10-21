/**********************************************************************************
*Copyright (c) 2016, Jimtch.ltd
*All rights reserved.
*
* 文件名称：	ui display
* 内容摘要：	界面显示
* 文件标识：	无
* 其它说明：	

* 当前版本：	V1.0
*
*修改日期        	版本号              	修改人         		修改内容
* -----------------------------------------------------------------
* 2017412             V1.0                         Alex                       创建
************************************************************************************/


#include "user.h"



#ifndef	TEST_WITH_OUT_UI



volatile LP_UI_MEUI_FUNC_HANLE ui_sys_func_handle = ui_extern_sys_test_menu_handle;

volatile void * ui_sys_func_arg = NULL;

// ui 事件处理变量
uint32_t	ui_sys_info;



/************************************************************************************************
* 功能描述： 	强制重新刷新界面
* 输入参数：   	无
* 输出参数：	无
* 返回值     ： 	无
* 其它说明：
* 修改记录：   
* 修改日期		  版本号		       修改人		 修改内容
*  ----------------------------------------------------------------------------
* 20160316		    V1.0 			        Alex		    创建
**************************************************************************************************/
void ui_extern_refresh(void)
{
	ui_sys_info |= (UI_INFO_FLAG_REFRESH_EVENT);

}


/************************************************************************************************
* 功能描述： 	ui 初始化
* 输入参数：   	无
* 输出参数：	无
* 返回值     ： 	无
* 其它说明：
* 修改记录：   
* 修改日期		  版本号		       修改人		 修改内容
*  ----------------------------------------------------------------------------
* 20160316		    V1.0 			        Alex		    创建
**************************************************************************************************/
void ui_extern_main_init( void )
{
	
	ui_sys_info = 0;

	TRACE_WP("SUCCESS ui_extern_main_init\r\n");

	// 启动界面刷新
	ui_extern_refresh();
}






/************************************************************************************************
* 功能描述： 	ui 主界面
* 输入参数：   	无
* 输出参数：	无
* 返回值     ： 	无
* 其它说明：
* 修改记录：   
* 修改日期		  版本号		       修改人		 修改内容
*  ----------------------------------------------------------------------------
* 20160316		    V1.0 			        Alex		    创建
**************************************************************************************************/
void ui_extern_sys_test_menu_handle( void * arg)
{
	uint8_t buf[100]={0};
	uint8_t buf_state[100]={0};
	uint8_t len;
	uint8_t key;
	int x,y;
	int i;
	uint8_t *p_dp;
	
    

	if ( UI_INFO_FLAG_REFRESH_EVENT == (ui_sys_info & UI_INFO_FLAG_REFRESH_EVENT ))
	{    
		// 界面重绘制
		ui_sys_info &= (~UI_INFO_FLAG_REFRESH_EVENT);

		// 绘制型号
		gui_lcd_extern_set_fonts(Font16x24.width,Font16x24.height,Font16x24.table);

		// 测试结果
		if( system_run_ram.burn_and_test_result == 1 )
		{
			// 成功
			gui_lcd_extern_set_current_back_color(GUI_GREEN, GUI_BLACK);
		}
		else if( system_run_ram.burn_and_test_result == 2 )
		{
			// 失败
			gui_lcd_extern_set_current_back_color(GUI_RED, GUI_BLACK);
		}
		else 
		{
			// 执行中
			gui_lcd_extern_set_current_back_color(GUI_WHITE, GUI_BLACK);
		}


		// 标签头部信息打印
		memset(buf,' ',30);
		len = sprintf( (char *)buf,VERSION);
		buf[len]= ' '; y = 0;
		x = (GUI_MAX_WIDTH - len*Font16x24.width)/2;
		gui_lcd_extern_draw_string(	x, y , buf);


		// 烧录信息
		gui_lcd_extern_set_fonts(Font8x16.width,Font8x16.height,Font8x16.table);

		//画线条
		x = 0;y = 16;		
		y = y+ Font8x16.height+4;
		gui_lcd_extern_draw_transverse_line(0,y,GUI_MAX_WIDTH,0);
		
		y = y+ 5;
		// Chip Burn Time
		memset(buf,' ',100);
		len = sprintf( (char *)buf,"Chip Burn Time : %6d(ms)", system_run_ram.chip_burn_time);
		gui_lcd_extern_draw_string(	x, y , buf);
		y = y+ Font8x16.height + 5;

		gui_lcd_extern_draw_transverse_line(0,y,GUI_MAX_WIDTH,0);
		y = y+ 5;


		// 烧录状态
		gui_lcd_extern_set_fonts(Font16x24.width,Font16x24.height,Font16x24.table);
		memset(buf,' ',100);
	

		// fw ver
		gui_lcd_extern_set_fonts(Font8x16.width,Font8x16.height,Font8x16.table);
		memset(buf,' ',100);
	
		if( system_run_ram.dev_work_state.read_pid_mac_sign  != 0 )
		{
			len = sprintf( (char *)buf,"Product_code:  %02x%02x%02x%02x%02x%02x%02x  ",system_run_ram.dev_work_state.read_pid[0],
																					system_run_ram.dev_work_state.read_pid[1],
																					system_run_ram.dev_work_state.read_pid[2],
																					system_run_ram.dev_work_state.read_pid[3],
																			system_run_ram.dev_work_state.read_pid[4],
																			system_run_ram.dev_work_state.read_pid[5],
																			system_run_ram.dev_work_state.read_pid[6]);
			system_run_ram.dev_work_state.get_pid_ok = 0;
		}
		else
		{
			len = sprintf( (char *)buf,"Product_code:                         ");
		}
		x = 0; 
		gui_lcd_extern_draw_string(	x, y , buf);
		y = y+ Font8x16.height*2+5;
	
		
	
				// fw ver
		gui_lcd_extern_set_fonts(Font8x16.width,Font8x16.height,Font8x16.table);
		memset(buf,' ',100);
	
		if( system_run_ram.dev_work_state.read_pid_mac_sign  != 0 ) 
		{
			len = sprintf( (char *)buf,"MAC:  %02x%02x%02x%02x%02x%02x%02x%02x  ",system_run_ram.dev_work_state.read_mac[7],
																				system_run_ram.dev_work_state.read_mac[6],
																				system_run_ram.dev_work_state.read_mac[5],
																				system_run_ram.dev_work_state.read_mac[4],
																				system_run_ram.dev_work_state.read_mac[3],
																				system_run_ram.dev_work_state.read_mac[2],
																				system_run_ram.dev_work_state.read_mac[1],
																				system_run_ram.dev_work_state.read_mac[0]);
		}	
		else
		{
			len = sprintf( (char *)buf,"MAC:                              ");
		}
		x = 0; 
		gui_lcd_extern_draw_string(	x, y , buf);
		y = y+ Font8x16.height*2+5;
		
		
		// wifi rssi
		memset(buf,' ',100);
		if(system_run_ram.dev_work_state.read_pid_mac_sign != 0)	
		{
			len = sprintf( (char *)buf,"WIFI Rssi: %d dB  min:%ddB     ",system_run_ram.dev_work_state.read_rssi,system_run_ram.usb_seng_rssi);
		}
		else
		{
			len = sprintf( (char *)buf,"WIFI Rssi:         min:%ddB     ",system_run_ram.usb_seng_rssi);
		}
		
		x = 0; 
		gui_lcd_extern_draw_string(	x, y , buf);
		y = y+ Font16x24.height+3;
	}
}


/************************************************************************************************
* 功能描述： 	ui 处理事件
* 输入参数：   	无
* 输出参数：	无
* 返回值     ： 	无
* 其它说明：
* 修改记录：   
* 修改日期		  版本号		       修改人		 修改内容
*  ----------------------------------------------------------------------------
* 20160316		    V1.0 			        Alex		    创建
**************************************************************************************************/
void ui_extern_main_handle( void )
{

	if(ui_sys_func_handle == NULL)
	{
		//TRACE_WP("ui_sys_func_handle NULL\r\n");
		//ui_extern_refresh();
		ui_extern_sys_test_menu_handle(NULL);
		
	}
	else
	{
		//TRACE_WP("ui_sys_func_handle run\r\n");
		ui_sys_func_handle( (void *)ui_sys_func_arg);	
	}
}

#endif

