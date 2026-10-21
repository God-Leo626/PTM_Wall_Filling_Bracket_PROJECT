/*!
 @file NDS03_SingleRangingInterrupt_Example.c
 @brief 单次测量中断示例
 @author lull
 @date 2025-06
 @copyright Copyright (c) 2025  Shenzhen Nephotonics  Semiconductor Technology Co., Ltd.
 @license BSD 3-Clause License
          This file is part of the Nephotonics sensor SDK.
          It is licensed under the BSD 3-Clause License.
          A copy of the license can be found in the project root directory, in the file named LICENSE.
 */

#include <stdio.h>

#include "nds03_calib.h"
#include "nds03_comm.h"
#include "nds03_data.h"
#include "nds03_def.h"
#include "nds03_dev.h"
#include "nds03_platform.h"

/** 模组设备 */
NDS03_Dev_t g_nds03_device = {
    .platform.i2c_dev_addr = NDS03_DEFAULT_SLAVE_ADDR,
};

void printf_ranging_data(NDS03_Dev_t *pNxDevice)
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
/**
 * @brief 单次测量接口
 *
 * @param pNxDevice 模组设备
 */
void SG_RangingTest(NDS03_Dev_t *pNxDevice)
{
    /* 获取测量数据 */
    NDS03_GetInterruptRangingData(pNxDevice);
    printf_ranging_data(pNxDevice);
    NDS03_StartSingleMeasurement(pNxDevice);
}

/**
 * @brief NDS03单次中断测量主函数示例
 *
 * @return int32_t
 */
int32_t main(void)
{
    NDS03_Dev_t *pNxDevice = &g_nds03_device;
    /* 函数指针结构体 */

    if (NDS03_ERROR_NONE != nds03_platform_init(&pNxDevice->platform, NULL)) {
        printf("nds03_platform init error\n");
        return -1;
    }
    /* 循环等待设备启动, 若模组或者IIC读写函数有问题则会报错 */
    if (NDS03_ERROR_NONE != NDS03_WaitDeviceBootUp(pNxDevice)) {
        printf("NDS03_WaitDeviceBootUp error\r\n");
        return -1;
    }
    /** 判断是否为NDS03设备 */
    if(NDS03_ERROR_NONE != NDS03_IsNDS03(pNxDevice))
	{
		printf("The device is not NDS03, please change the device!\n");
		return -1;
	}
    /* 初始化模组设备 */
    if (NDS03_ERROR_NONE != NDS03_InitDevice(pNxDevice)) {
        printf("NDS03_InitDevice error!!\r\n");
        return -1;
    }
    /* 设置使用有新数据时触发中断,且低电平产生中断 */
    NDS03_SetGpio1Config(pNxDevice, NDS03_GPIO1_NEW_MEASURE_READY,
                         NDS03_GPIO1_POLARITY_LOW);

    /* 需要先测量一次 */
    NDS03_StartSingleMeasurement(pNxDevice);
    /* 循环测量 */
    while (1) {
        NDS03_Delay1ms(pNxDevice, 10);
        WaitForNDS03_Interrupt(&pNxDevice->platform);
        SG_RangingTest(pNxDevice);
    }
    /* 关闭测距中断 */
    NDS03_SetGpio1Config(pNxDevice, NDS03_GPIO1_FUNCTIONALITY_OFF,
                         NDS03_GPIO1_POLARITY_LOW);
    nds03_platform_uninit(&pNxDevice->platform, NULL);
    return 0;
}
