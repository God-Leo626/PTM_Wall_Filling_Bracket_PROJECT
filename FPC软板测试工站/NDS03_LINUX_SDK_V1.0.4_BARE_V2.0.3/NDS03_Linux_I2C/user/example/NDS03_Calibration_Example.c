/*!
 @file NDS03_Calibration_Example.c
 @brief 标定示例
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

/**
 * @brief NDS03标定函数示例
 *        按照NDS03标定手册进行标定，以手册为主
 *        注意：以下标定示例需要将设备处于500mm处
 * @return int32_t
 */
int32_t main(int argc, char **argv)
{
    NDS03_Dev_t *pNxDevice      = &g_nds03_device;
    uint32_t     calib_depth_mm = 500;
    uint16_t     xtalk_value    = 0;
    if (argc > 1) {
        calib_depth_mm = atoi(argv[1]);
    }
    int ret;
    /* 测距次数为20次 */
    int32_t i, cnt = 20;
    int32_t depth_sum, depth_aver;

    if (NDS03_ERROR_NONE != nds03_platform_init(&pNxDevice->platform, NULL)) {
        printf("nds03_platform init error\n");
    }

    /* 循环等待设备启动, 若模组或者IIC读写函数有问题则会报错 */
    if (NDS03_ERROR_NONE != NDS03_WaitDeviceBootUp(pNxDevice)) {
        printf("NDS03_WaitDeviceBootUp error\r\n");
        return -1;
    }
    /** 判断是否为NDS03设备 */
    if (NDS03_ERROR_NONE != NDS03_IsNDS03(pNxDevice)) {
        printf("The device is not NDS03, please change the device!\n");
        return -1;
    }
    /* 初始化模组设备 */
    if (NDS03_ERROR_NONE != NDS03_InitDevice(pNxDevice)) {
        printf("NDS03_InitDevice error!!\r\n");
        return -1;
    }

    /* 将设备处于500mm处 */
    /* 串扰标定 */
    ret = NDS03_XtalkCalibration(pNxDevice);
    if (NDS03_ERROR_NONE != ret) {
        printf("NDS03_XtalkCalibration error!! ret:%d\r\n", ret);
        return -1;
    }
    /** 获取串扰值 */
    ret = NDS03_GetXTalkValue(pNxDevice, &xtalk_value);
    if (NDS03_ERROR_NONE != ret) {
        printf("NDS03_GetXTalkValue error!! ret:%d\r\n", ret);
        return -1;
    }
    ret = NDS03_OffsetCalibrationAtDepth(pNxDevice, calib_depth_mm);
    /* Offset标定 */
    if (NDS03_ERROR_NONE != ret) {
        printf("NDS03_OffsetCalibrationAtDepth error!! ret: %d\r\n", ret);
        return -1;
    }

    /* 获取平均值 */
    depth_sum = 0;
    for (i = 0; i < cnt; i++) {
        if (NDS03_ERROR_NONE != NDS03_GetSingleRangingData(pNxDevice)) {
            printf("NDS03_GetSingleRangingData error!!\r\n");
            return -1;
        }
        depth_sum =
            depth_sum + (int32_t)(uint32_t)pNxDevice->ranging_data[0].depth;
    }
    depth_aver = depth_sum / cnt;
    if (depth_aver < (calib_depth_mm - 20) ||
        depth_aver > (calib_depth_mm + 20)) {
        printf("NDS03 calibration fail!!\r\n");
        return -1;
    }
    printf("NDS03 calibration success\r\n");
    nds03_platform_uninit(&pNxDevice->platform, NULL);
    return 0;
}