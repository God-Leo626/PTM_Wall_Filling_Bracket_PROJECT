/*!
 @file NDS03_Ranging_Example.c
 @brief 单次测量示例
 @author lull
 @date 2025-06
 @copyright Copyright (c) 2025  Shenzhen Nephotonics  Semiconductor Technology Co., Ltd.
 @license BSD 3-Clause License
          This file is part of the Nephotonics sensor SDK.
          It is licensed under the BSD 3-Clause License.
          A copy of the license can be found in the project root directory, in the file named LICENSE.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "platform.h"

NDS03_Platform_t platform;

void printf_ranging_data(NDS03_Platform_t *pNxDevice)
{
    printf("ranging data start:\r\n");

    printf("dist start:\r\n");
    printf(
        "%d %d\r\n"
        "%d %d\r\n",
        pNxDevice->ranging_data[0].depth, pNxDevice->ranging_data[1].depth,
        pNxDevice->ranging_data[2].depth, pNxDevice->ranging_data[3].depth);
    printf("dist end\r\n");

    printf("confi start:\r\n");
    printf(
        "%d %d\r\n"
        "%d %d\r\n",
        pNxDevice->ranging_data[0].confi, pNxDevice->ranging_data[1].confi,
        pNxDevice->ranging_data[2].confi, pNxDevice->ranging_data[3].confi);
    printf("confi end\r\n");

    printf("count start:\r\n");
    printf(
        "%d %d\r\n"
        "%d %d\r\n",
        pNxDevice->ranging_data[0].count, pNxDevice->ranging_data[1].count,
        pNxDevice->ranging_data[2].count, pNxDevice->ranging_data[3].count);
    printf("count end\r\n");

    printf("crate start:\r\n");
    printf(
        "%d %d\r\n"
        "%d %d\r\n",
        pNxDevice->ranging_data[0].crate, pNxDevice->ranging_data[1].crate,
        pNxDevice->ranging_data[2].crate, pNxDevice->ranging_data[3].crate);
    printf("crate end\r\n");

    printf("ranging data end\r\n");
    fflush(stdout);
}

int32_t main(int argc, char **argv)
{
    NDS03_Platform_t *pDev = &platform;

    nds03_platform_init(pDev);
    // nds03_set_polling_interval(pDev, 1000);  //使用中断模式注释掉
    /** 由于input驱动绝对位置事件机制问题，如果数据不不变化则不会上传数据，因此为了防止
	 * 因只有一个目标导致其他数据为0值，故初始化为深度65300*/
	for (int32_t i = 0; i < sizeof(pDev->ranging_data)/sizeof(pDev->ranging_data[0]); i++) {
		pDev->ranging_data[i].depth = 65300;
	}
    nds03_meas_start(pDev);

    while (1) {
        nds03_get_single_rang(pDev);
        printf_ranging_data(pDev);
    }
    nds03_meas_stop(pDev);
    return 0;
}
