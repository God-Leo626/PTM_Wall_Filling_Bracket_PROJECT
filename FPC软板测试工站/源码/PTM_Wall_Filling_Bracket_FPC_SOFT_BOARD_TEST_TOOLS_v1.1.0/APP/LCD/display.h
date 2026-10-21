/*
 * @Author: “HP “1357243695@qq.com”
 * @Date: 2023-11-10 14:48:30
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2026-09-18 11:36:39
 * @FilePath: \镭雕\APP\LCD\display.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __UI_DISPLAY_APP_H__
#define __UI_DISPLAY_APP_H__

#include "user.h"


#define LOOK_DIANLIU          0


//发布版本

//1.1版本改为随时接收模式
//#define VERSION  "s60_zb_LEIDIAO_NO_LOOK_DIANLIU_V1.2_2025_3_13"

//1.3版本改为随时接收模式
//#define VERSION  "s60_zb_LEIDIAO_NO_LOOK_DIANLIU_V1.3_2025_3_17"

// //晨阳优化dongle 这边优化设置SKU
// #define VERSION  "s60_zb_LEIDIAO_NO_LOOK_DIANLIU_V2.1_2025_3_18"

//这版本的镭雕是老的镭雕格式  后面可能回替换的
#define VERSION  "PTM_Wall_Filling_Bracket_FPC_SOFT_BOARD_TEST_TOOLS_v1.1.0_2026_9_20"

#define	DIS_MARK_NO_SELECT		0

#define	DIS_MARK_SELECT			1

#define	DIS_MARK_CLEAN			2


#ifndef	TEST_WITH_OUT_UI


#define UI_INFO_FLAG_REFRESH_EVENT			(1<<0)		// ǿ��ˢ���¼�



typedef void (*P_UI_MEUI_FUNC_HANLE)( void * arg);
typedef P_UI_MEUI_FUNC_HANLE 	LP_UI_MEUI_FUNC_HANLE;


typedef struct  _UI_STRING_LOCATION_ST_T_
{

	uint16_t		x_pos;
	uint16_t		y_pos;
										
}UI_STRING_LOCATION;





typedef struct  _INFO_TEST_VAL_ST_T_
{
	uint32_t power_on_num[20];
	uint32_t restart_success_num[20];
	uint32_t restart_timeout_num[20];
}INFO_TEST_VAL;





extern volatile LP_UI_MEUI_FUNC_HANLE ui_sys_func_handle;
extern volatile void * ui_sys_func_arg;
extern uint32_t	ui_sys_info;



void ui_extern_refresh(void);
void ui_extern_main_init( void );
void ui_extern_sys_test_menu_init( void );
void ui_extern_sys_test_menu_handle( void * arg);
void ui_extern_main_handle( void );


#endif

#endif


