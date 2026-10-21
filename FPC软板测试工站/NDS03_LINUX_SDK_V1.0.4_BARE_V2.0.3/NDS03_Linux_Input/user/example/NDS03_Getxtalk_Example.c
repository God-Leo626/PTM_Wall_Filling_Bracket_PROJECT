/*!
 @file NDS03_Getxtalk_Example.c
 @brief 获取串扰值示例
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
    uint16_t          xtalk_value;
    int8_t            retval = 0;
    NDS03_Platform_t *pDev   = &platform;

    retval = nds03_platform_init(pDev);
    if (retval < 0) {
        printf("nds03 platform init error\n");
        return -1;
    }
    retval = nds03_get_tof_xtalk(pDev, &xtalk_value);
    if (retval < 0) {
        printf("nds03 get xtalk error\n");
        return -2;
    }
    printf("nds03 xtalk value %d\n", xtalk_value);
    return 0;
}
