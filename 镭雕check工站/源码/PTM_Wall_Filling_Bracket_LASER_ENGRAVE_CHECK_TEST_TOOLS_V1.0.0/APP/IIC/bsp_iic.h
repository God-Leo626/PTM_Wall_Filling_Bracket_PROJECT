#ifndef __BSP_IIC_H
#define __BSP_IIC_H

#include "bsp_iic.h"
#include "stdint.h"


void bsp_iic_init(void);
void iic_start(void);
void iic_stop(void);
uint8_t iic_wait_ack(void);
void iic_ack(void);
void iic_nack(void);
void iic_send_byte(uint8_t data);
uint8_t iic_read_byte(uint8_t ack);


#endif
