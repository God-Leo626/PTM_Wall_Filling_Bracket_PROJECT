/*!
 @file NDS03_Calibration_Example.c
 @brief 标定(串扰+offset)示例
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

int32_t main(int argc, char **argv)
{
    int8_t            retval         = 0;
    NDS03_Platform_t *pDev           = &platform;
    uint16_t          calib_depth_mm = 500;  ///< offset标定距离，单位mm
    if (argc > 1) {
        calib_depth_mm = atoi(argv[1]);
    }
    retval = nds03_platform_init(pDev);
    if (retval < 0) {
        printf("nds03 platform init error\n");
        return -1;
    }
    retval = nds03_tof_calib(pDev, calib_depth_mm);
    if (retval < 0) {
        printf("nds03 calibration error\n");
        return -2;
    }
    printf("nds03 calibration successful\n");
    return 0;
}
