#ifndef __SHT4XX_H
#define __SHT4XX_H

#include "stdint.h"
#include "sht4xx.h"

#define SHT4_SLAVE_ADDRESS      0x44

// SHT40命令定义
#define SHT4_MEASURE_HIGH_PRECISION    0xFD  // 高精度测量
#define SHT4_MEASURE_MEDIUM_PRECISION  0xF6  // 中精度测量  
#define SHT4_MEASURE_LOW_PRECISION     0xE0  // 低精度测量
#define SHT4_READ_SERIAL               0x89  // 读取序列号
#define SHT4_SOFT_RESET                0x94  // 软复位

void sht4xx_init(void);
uint8_t sht4xx_soft_reset(void);
uint8_t sht4xx_write_bytes(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data_buf, uint8_t len);
uint8_t sht4xx_read_bytes(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data_buf, uint8_t len);
uint8_t sht4x_read_temp_and_hum(float *temperature, float *humidity);

#endif
