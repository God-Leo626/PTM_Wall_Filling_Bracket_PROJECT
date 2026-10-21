/*
 * @Author: “HP “1357243695@qq.com”
 * @Date: 2023-08-10 10:57:36
 * @LastEditors: “HP “1357243695@qq.com”
 * @LastEditTime: 2023-08-11 10:19:40
 * @FilePath: \project\APP\USB\USB_APP\usbd_cdc_vcp.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __USBD_CDC_VCP_H
#define __USBD_CDC_VCP_H
#include "usbd_cdc_core.h"
#include "usbd_conf.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32������
//usb vcp��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/2/24
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#define USB_USART_REC_LEN	 	200				//USB���ڽ��ջ���������ֽ���

extern u8  USB_USART_RX_BUF[USB_USART_REC_LEN]; //���ջ���,���USB_USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USB_USART_RX_STA;   					//����״̬���	


//USB���⴮��������ò���
typedef struct
{
  uint32_t bitrate;
  uint8_t  format;
  uint8_t  paritytype;
  uint8_t  datatype;
}LINE_CODING;
 


uint16_t VCP_Init     (void);
uint16_t VCP_DeInit   (void);
uint16_t VCP_Ctrl     (uint32_t Cmd, uint8_t* Buf, uint32_t Len);
uint16_t VCP_DataTx   (uint8_t data);
uint16_t VCP_DataRx   (uint8_t* Buf, uint32_t Len);
void usb_printf(char* fmt,...); 

uint8_t usb_extern_txrx_send_buf(uint8_t *p_buf,uint16_t len);

#endif 
















