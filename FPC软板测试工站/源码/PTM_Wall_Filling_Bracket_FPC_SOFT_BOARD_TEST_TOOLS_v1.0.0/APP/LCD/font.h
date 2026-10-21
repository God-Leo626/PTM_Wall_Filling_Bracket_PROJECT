/*
 * @Author: “HP “1357243695@qq.com”
 * @Date: 2023-07-20 17:11:02
 * @LastEditors: “HP “1357243695@qq.com”
 * @LastEditTime: 2023-08-08 18:06:18
 * @FilePath: \Guider\APP\LCD\font.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __FONT_H__
#define __FONT_H__
#include "stm32f4xx.h"


typedef struct _FONTS_ST_H_
{    

	const uint8_t  *table;
	uint16_t 	    width;
	uint16_t 	    height;
  
} FONTS_ST;

extern FONTS_ST Font8x16;
extern FONTS_ST Font16x24;



extern  u8 youyuan_6x12_L2R_H2L[];
extern  u8 youyuan_8x16_L2R_H2L[];
extern  u8 youyuan_12x24_L2R_H2L[];
extern  u8 youyuan_16x32_L2R_H2L[];
#endif


