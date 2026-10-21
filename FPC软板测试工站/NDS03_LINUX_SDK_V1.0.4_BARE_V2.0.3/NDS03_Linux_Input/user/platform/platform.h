/*!
 @file platform.h
 @brief 
 @author lull
 @date 2025-06
 @copyright Copyright (c) 2025  Shenzhen Nephotonics  Semiconductor Technology Co., Ltd.
 @license BSD 3-Clause License
          This file is part of the Nephotonics sensor SDK.
          It is licensed under the BSD 3-Clause License.
          A copy of the license can be found in the project root directory, in the file named LICENSE.
 */
#ifndef __PLATFORM_H
#define __PLATFORM_H

#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define TOF_NDS03_DRV_NAME	"tof_nds03"
#define NDS03_TARGET_MAX_NUM             4

typedef struct{
    uint16_t    depth;              ///< 测量距离
    uint16_t    confi;              ///< 测量置信度
    uint16_t    count;              ///< 计数值
    uint16_t    crate;              ///< 计数率
}NDS03_RangingData_t;


typedef struct NDS03_Platform{
	/* Describe the device name */
	char name[64];
	/* I2C device address */
	uint16_t  i2c_dev_addr;
	/* File descriptor for the input event device. */
	int eventFd;
	
	NDS03_RangingData_t ranging_data[NDS03_TARGET_MAX_NUM]; 

}NDS03_Platform_t;


/*!
 @brief 设备初始化
 @param   pdev 
 @return int8_t 
 */
int8_t nds03_platform_init(NDS03_Platform_t *pdev);
/*!
 @brief 设备注销
 @param   pdev 
 @return int8_t 
 */
int8_t nds03_platform_uninit(NDS03_Platform_t *pdev);
/*!
 @brief 开始测量
 @param   pdev 
 @return int8_t 
 */
int8_t nds03_meas_start(NDS03_Platform_t *pdev);
/*!
 @brief 停止测量
 @param   pdev 
 @return int8_t 
 */
int8_t nds03_meas_stop(NDS03_Platform_t *pdev);
/*!
 @brief 设备重置
 @param   pdev 
 @return int8_t 
 */
int8_t nds03_tof_reset(NDS03_Platform_t *pdev);
/*!
 @brief 设备进行标定(串扰+offset)
 @param   pdev 
 @param   dist_mm offset标定距离，单位mm
 @return int8_t 
 */
int8_t nds03_tof_calib(NDS03_Platform_t *pdev, uint16_t dist_mm);
/*!
 @brief 设置轮询模式测据间隔
 @param   pdev 
 @param   ms 
 @return int8_t 
 */
int8_t nds03_set_polling_interval(NDS03_Platform_t *pdev, uint32_t ms);
/*!
 @brief 获取单次测据数据
 @param   pdev 
 @return int8_t 
 */
int8_t nds03_get_single_rang(NDS03_Platform_t *pdev);
/*!
 @brief 获取设备串扰值
 @param   pdev 
 @param   xtalk_value 
 @return int8_t 
 */
int8_t nds03_get_tof_xtalk(NDS03_Platform_t *pdev, uint16_t* xtalk_value);
/*!
 @brief 设置设备发光次数
 @param   pdev 
 @param   pulse_num 
 @return int8_t 
 */
int8_t nds03_set_tof_pulsenum(NDS03_Platform_t *pdev, uint32_t pulse_num);
#endif


