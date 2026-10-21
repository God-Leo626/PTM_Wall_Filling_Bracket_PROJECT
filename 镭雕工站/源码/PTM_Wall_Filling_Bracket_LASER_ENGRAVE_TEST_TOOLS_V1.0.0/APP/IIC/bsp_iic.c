#include "user.h"
#include "gpio.h"

void bsp_iic_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

//  RCC_AHB1PeriphClockCmd(IIC_SDA_RCC|IIC_SCL_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(IIC_SCL_RCC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(IIC_SDA_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(IIC_SCL_PORT, &GPIO_InitStructure);
	
	IIC_SDA_H;
	IIC_SCL_H;
}

void iic_delay(uint8_t tick)
{
	vTimeDelayUs(8 * tick);
}

void iic_start(void)
{
	IIC_SDA_H;
	IIC_SCL_H;
	iic_delay(4);
	IIC_SDA_L;
	iic_delay(4);
	IIC_SCL_L;
	iic_delay(1);	
}

void iic_stop(void)
{
	IIC_SDA_L;
	iic_delay(4);
	IIC_SCL_H;
	iic_delay(4);
	IIC_SDA_H;
	iic_delay(4);
}

uint8_t iic_wait_ack(void)
{
	uint8_t waittime = 0;
	uint8_t rack = 0;
	
	IIC_SDA_H;
	iic_delay(1);
	IIC_SCL_H;
	iic_delay(1);
	
	while(IIC_READ_SDA)
	{
		waittime++;
		if(waittime > 250)
		{
			iic_stop();
			rack = 1;
			break;
		}
	}
	IIC_SCL_L;
	iic_delay(1);
	return rack;
}

void iic_ack(void)
{
	IIC_SCL_L;
	IIC_SDA_L;
	iic_delay(1);
	IIC_SCL_H;
	iic_delay(1);
	IIC_SCL_L;
	iic_delay(1);
	IIC_SDA_H;
}

void iic_nack(void)
{
	IIC_SCL_L;
	IIC_SDA_H;
	iic_delay(1);
	IIC_SCL_H;
	iic_delay(1);
	IIC_SCL_L;
}

void iic_send_byte(uint8_t data)
{
	IIC_SCL_L;
	for(uint8_t i = 0; i < 8; i++)
	{
		if((data & 0x80) >> 7)
		{
			IIC_SDA_H;
		}
		else
		{
			IIC_SDA_L;
		}
		iic_delay(1);
		IIC_SCL_H;
		iic_delay(1);
		IIC_SCL_L;
		data <<= 1;
	}
	IIC_SDA_H;
}

uint8_t iic_read_byte(uint8_t ack)
{
	uint8_t receive = 0;
	IIC_SCL_L;
	for(uint8_t i = 0; i < 8; i++)
	{
		receive <<= 1;
		IIC_SCL_H;
		iic_delay(1);
		if(IIC_READ_SDA)
		{
			receive++;
		}
		IIC_SCL_L;
		iic_delay(1);
	}
	
	if(!ack)
	{
		iic_nack();
	}
	else
	{
		iic_ack();
	}
	return receive;
}

