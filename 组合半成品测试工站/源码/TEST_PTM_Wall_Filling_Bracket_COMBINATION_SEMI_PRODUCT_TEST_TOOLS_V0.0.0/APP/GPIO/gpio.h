/**********************************************************************************
*Copyright (c) 2019, Sonoff.ltd
*All rights reserved.
*
* File name			:	gpio
* Summary Content		:	
* Document			:	None
* Notes				:	

* Version				:	V1.0
*
* Modification date		Ver		       	Name		 		Modify 
*  ----------------------------------------------------------------------------
* 20190703		       V1.0 			 Alex		       	Establish
************************************************************************************/

#ifndef __GPIO_DRV_H__
#define __GPIO_DRV_H__

#include "user.h"


/****************************A�˿ڵ�����**********************************/
#define PINS_40PIN_STM32_IO_PA0					{ GPIO_Pin_0, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}
#define PINS_40PIN_STM32_IO_PA1					{ GPIO_Pin_1, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}
#define PINS_40PIN_UART2_TXD						{ GPIO_Pin_2, GPIO_Speed_50MHz, GPIO_Mode_AF_PP}
#define PINS_40PIN_UART2_RXD						{ GPIO_Pin_3, GPIO_Speed_50MHz, GPIO_Mode_AF_PP}

#define PINS_FLASH_1_SPI1_NSS						{ GPIO_Pin_4, GPIO_Speed_50MHz, GPIO_Mode_Out_PP }	
#define PINS_FLASH_1_SPI1_SCLK						{ GPIO_Pin_5, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
#define PINS_FLASH_1_SPI1_MISO					{ GPIO_Pin_6, GPIO_Speed_50MHz, GPIO_Mode_IPU }
#define PINS_FLASH_1_SPI1_MOSI					{ GPIO_Pin_7, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }	

#define PINS_40PIN_STM32_IO_PA8					{ GPIO_Pin_8, GPIO_Speed_50MHz, GPIO_Mode_Out_PP}
#define PINS_40PIN_UART1_TXD						{ GPIO_Pin_9, GPIO_Speed_50MHz, GPIO_Mode_AF_PP}
#define PINS_40PIN_UART1_RXD						{ GPIO_Pin_10, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}

#define PINS_TRANSMISSION_USB_DM					{ GPIO_Pin_11, GPIO_Speed_50MHz, GPIO_Mode_AF_OD}
#define PINS_TRANSMISSION_USB_DP					{ GPIO_Pin_12, GPIO_Speed_50MHz, GPIO_Mode_Out_PP}

#define PINS_TRANSMISSION_USB_SPEED				{ GPIO_Pin_15, GPIO_Speed_50MHz, GPIO_Mode_AF_OD}
#define PINS_TRANSMISSION_USB_SPEED_H			{ GPIO_Pin_15, GPIO_Speed_50MHz, GPIO_Mode_Out_PP}


#define GPIO_CFG_A		\
			PINS_40PIN_STM32_IO_PA0,\
			PINS_40PIN_STM32_IO_PA1,\
			PINS_40PIN_UART2_TXD,\
			PINS_40PIN_UART2_RXD,\
			PINS_FLASH_1_SPI1_NSS,\
			PINS_FLASH_1_SPI1_SCLK,\
			PINS_FLASH_1_SPI1_MISO,\
			PINS_FLASH_1_SPI1_MOSI,\
			PINS_40PIN_STM32_IO_PA8,\
			PINS_40PIN_UART1_TXD,\
			PINS_40PIN_UART1_RXD,\
			PINS_TRANSMISSION_USB_DM,\
			PINS_TRANSMISSION_USB_DP,\
			PINS_TRANSMISSION_USB_SPEED,\
		
			


/****************************B�˿ڵ�����**********************************/
#define PINS_40PIN_STM32_IO_PB0					{ GPIO_Pin_0, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING }
#define PINS_FLASH_2_SPI2_CS2						{ GPIO_Pin_1, GPIO_Speed_50MHz, GPIO_Mode_Out_PP }

#define PINS_40PIN_STM32_IO_PB3					{ GPIO_Pin_3, GPIO_Speed_50MHz, GPIO_Mode_IPU }
#define PINS_40PIN_STM32_IO_PB4					{ GPIO_Pin_4, GPIO_Speed_50MHz, GPIO_Mode_Out_PP }
#define PINS_40PIN_STM32_IO_PB5					{ GPIO_Pin_5, GPIO_Speed_50MHz, GPIO_Mode_Out_PP }
#define PINS_40PIN_STM32_IO_PB6					{ GPIO_Pin_6, GPIO_Speed_50MHz, GPIO_Mode_Out_PP }

#define PINS_SYS_LED_PB7							{ GPIO_Pin_7, GPIO_Speed_50MHz, GPIO_Mode_Out_PP }

#define PINS_40PIN_STM32_IO_PB8					{ GPIO_Pin_8, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}
#define PINS_40PIN_STM32_IO_PB9					{ GPIO_Pin_9, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}

#define PINS_DEBUG_UART3_TXD						{ GPIO_Pin_10, GPIO_Speed_50MHz, GPIO_Mode_AF_PP}
#define PINS_DEBUG_UART3_RXD						{ GPIO_Pin_11, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}

#define PINS_FLASH_2_SPI2_CS1						{ GPIO_Pin_12, GPIO_Speed_50MHz, GPIO_Mode_Out_PP }	
#define PINS_FLASH_2_SPI2_SCLK						{ GPIO_Pin_13, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
#define PINS_FLASH_2_SPI2_MISO					{ GPIO_Pin_14, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
#define PINS_FLASH_2_SPI2_MOSI					{ GPIO_Pin_15, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }


#define GPIO_CFG_B  \
			PINS_40PIN_STM32_IO_PB0,\
			PINS_FLASH_2_SPI2_CS2,\
			PINS_40PIN_STM32_IO_PB3,\
			PINS_40PIN_STM32_IO_PB4,\
			PINS_40PIN_STM32_IO_PB5,\
			PINS_40PIN_STM32_IO_PB6,\
			PINS_SYS_LED_PB7,\
			PINS_40PIN_STM32_IO_PB8,\
			PINS_40PIN_STM32_IO_PB9,\
			PINS_DEBUG_UART3_TXD,\
			PINS_DEBUG_UART3_RXD,\
			PINS_FLASH_2_SPI2_CS1,\
			PINS_FLASH_2_SPI2_SCLK,\
			PINS_FLASH_2_SPI2_MISO,\
			PINS_FLASH_2_SPI2_MOSI,\


			
			
			

			
/**************************** C �˿ڵ�����**********************************/
#define PINS_40PIN_STM32_IO_PC0					{ GPIO_Pin_0, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}
#define PINS_EXTERN_POWER_CTR_PC1				{ GPIO_Pin_1, GPIO_Speed_50MHz, GPIO_Mode_Out_PP}
#define PINS_EXTERN_BUZZER_CTR_PC2				{ GPIO_Pin_2, GPIO_Speed_50MHz, GPIO_Mode_Out_PP}

#define PINS_40PIN_STM32_IO_PC3					{ GPIO_Pin_3, GPIO_Speed_50MHz, GPIO_Mode_IPU}
#define PINS_40PIN_STM32_IO_PC4					{ GPIO_Pin_4, GPIO_Speed_50MHz, GPIO_Mode_IPU}
#define PINS_40PIN_STM32_IO_PC5					{ GPIO_Pin_5, GPIO_Speed_50MHz, GPIO_Mode_IPU}
#define PINS_40PIN_STM32_IO_PC6					{ GPIO_Pin_6, GPIO_Speed_50MHz, GPIO_Mode_IPU}
#define PINS_40PIN_STM32_IO_PC7					{ GPIO_Pin_7, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING} 

#define PINS_40PIN_STM32_SLED_PC8					{ GPIO_Pin_8, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}
#define PINS_40PIN_STM32_RST_PC9					{ GPIO_Pin_9, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}

 
#define PINS_40PIN_UART4_TXD						{ GPIO_Pin_10, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}
#define PINS_40PIN_UART4_RXD						{ GPIO_Pin_11, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}

#define PINS_40PIN_UART5_TXD						{ GPIO_Pin_12, GPIO_Speed_50MHz, GPIO_Mode_AF_PP}


#define GPIO_CFG_C  \
			PINS_40PIN_STM32_IO_PC0,\
			PINS_EXTERN_POWER_CTR_PC1,\
			PINS_EXTERN_BUZZER_CTR_PC2,\
			PINS_40PIN_STM32_IO_PC3,\
			PINS_40PIN_STM32_IO_PC4,\
			PINS_40PIN_STM32_IO_PC5,\
			PINS_40PIN_STM32_IO_PC6,\
			PINS_40PIN_STM32_IO_PC7,\
			PINS_40PIN_STM32_SLED_PC8,\
			PINS_40PIN_STM32_RST_PC9,\
			PINS_40PIN_UART4_TXD,\
			PINS_40PIN_UART4_RXD,\
			PINS_40PIN_UART5_TXD,\

			


/****************************D�˿ڵ�����**********************************/

#define PINS_LCD_FSMC_GUI_FSMC_D2				{ GPIO_Pin_0, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
#define PINS_LCD_FSMC_GUI_FSMC_D3				{ GPIO_Pin_1, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }

#define PINS_40PIN_UART5_RXD						{ GPIO_Pin_2, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}

#define PINS_40PIN_STM32_IO_PD3					{ GPIO_Pin_3, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}

#define PINS_LCD_FSMC_GUI_FSMC_NOE_RD			{ GPIO_Pin_4, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
#define PINS_LCD_FSMC_GUI_FSMC_NWE_WR			{ GPIO_Pin_5, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }

#define PINS_KEY_LCD_RETURN_PD6					{ GPIO_Pin_6, GPIO_Speed_50MHz, GPIO_Mode_IPU}

#define PINS_LCD_FSMC_GUI_FSMC_NCE2_CS			{ GPIO_Pin_7, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
#define PINS_LCD_FSMC_GUI_FSMC_D13				{ GPIO_Pin_8, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
#define PINS_LCD_FSMC_GUI_FSMC_D14				{ GPIO_Pin_9, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
#define PINS_LCD_FSMC_GUI_FSMC_D15				{ GPIO_Pin_10, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
#define PINS_LCD_FSMC_GUI_FSMC_NALE_RS			{ GPIO_Pin_11, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }

#define PINS_LCD_LEDA_CTR_PD12					{ GPIO_Pin_12, GPIO_Speed_50MHz, GPIO_Mode_Out_PP}
#define PINS_LCD_RST_CTR_PD13						{ GPIO_Pin_13, GPIO_Speed_50MHz, GPIO_Mode_Out_PP}

#define PINS_LCD_FSMC_GUI_FSMC_D0				{ GPIO_Pin_14, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
#define PINS_LCD_FSMC_GUI_FSMC_D1				{ GPIO_Pin_15, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }


#define GPIO_CFG_D  	\
			PINS_LCD_FSMC_GUI_FSMC_D2,\
			PINS_LCD_FSMC_GUI_FSMC_D3,\
			PINS_40PIN_UART5_RXD,\
			PINS_40PIN_STM32_IO_PD3,\
			PINS_LCD_FSMC_GUI_FSMC_NOE_RD,\
			PINS_LCD_FSMC_GUI_FSMC_NWE_WR,\
			PINS_KEY_LCD_RETURN_PD6,\
			PINS_LCD_FSMC_GUI_FSMC_NCE2_CS,\
			PINS_LCD_FSMC_GUI_FSMC_D13,\
			PINS_LCD_FSMC_GUI_FSMC_D14,\
			PINS_LCD_FSMC_GUI_FSMC_D15,\
			PINS_LCD_FSMC_GUI_FSMC_NALE_RS,\
			PINS_LCD_LEDA_CTR_PD12,\
			PINS_LCD_RST_CTR_PD13,\
			PINS_LCD_FSMC_GUI_FSMC_D0,\
			PINS_LCD_FSMC_GUI_FSMC_D1,\


	
			


/****************************E�˿ڵ�����**********************************/

#define PINS_40PIN_STM32_IO_PE0					{ GPIO_Pin_0, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}
#define PINS_40PIN_STM32_IO_PE1					{ GPIO_Pin_1, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}
#define PINS_40PIN_STM32_IO_PE2					{ GPIO_Pin_2, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}
#define PINS_40PIN_STM32_IO_PE3					{ GPIO_Pin_3, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}
#define PINS_40PIN_STM32_IO_PE4					{ GPIO_Pin_4, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}
#define PINS_40PIN_STM32_IO_PE5					{ GPIO_Pin_5, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}
#define PINS_40PIN_STM32_IO_PE6					{ GPIO_Pin_6, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING}


#define PINS_LCD_FSMC_GUI_FSMC_D4				{ GPIO_Pin_7, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
#define PINS_LCD_FSMC_GUI_FSMC_D5				{ GPIO_Pin_8, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
#define PINS_LCD_FSMC_GUI_FSMC_D6				{ GPIO_Pin_9, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
#define PINS_LCD_FSMC_GUI_FSMC_D7				{ GPIO_Pin_10, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
#define PINS_LCD_FSMC_GUI_FSMC_D8				{ GPIO_Pin_11, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
#define PINS_LCD_FSMC_GUI_FSMC_D9				{ GPIO_Pin_12, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
#define PINS_LCD_FSMC_GUI_FSMC_D10				{ GPIO_Pin_13, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
#define PINS_LCD_FSMC_GUI_FSMC_D11				{ GPIO_Pin_14, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
#define PINS_LCD_FSMC_GUI_FSMC_D12				{ GPIO_Pin_15, GPIO_Speed_50MHz, GPIO_Mode_AF_PP }
	

#define GPIO_CFG_E  	\
			PINS_40PIN_STM32_IO_PE0,\
			PINS_40PIN_STM32_IO_PE1,\
			PINS_40PIN_STM32_IO_PE2,\
			PINS_40PIN_STM32_IO_PE3,\
			PINS_40PIN_STM32_IO_PE4,\
			PINS_40PIN_STM32_IO_PE5,\
			PINS_40PIN_STM32_IO_PE6,\
			PINS_LCD_FSMC_GUI_FSMC_D4,\
			PINS_LCD_FSMC_GUI_FSMC_D5,\
			PINS_LCD_FSMC_GUI_FSMC_D6,\
			PINS_LCD_FSMC_GUI_FSMC_D7,\
			PINS_LCD_FSMC_GUI_FSMC_D8,\
			PINS_LCD_FSMC_GUI_FSMC_D9,\
			PINS_LCD_FSMC_GUI_FSMC_D10,\
			PINS_LCD_FSMC_GUI_FSMC_D11,\
			PINS_LCD_FSMC_GUI_FSMC_D12,\



// GPIO �������Ŷ���

// ����
// �������ذ���
#define IN_PINS_KEY_LCD_ENTER						GPIOC,GPIO_Pin_3
#define IN_PINS_KEY_LCD_UP							GPIOC,GPIO_Pin_4
#define IN_PINS_KEY_LCD_DOWN						GPIOC,GPIO_Pin_5
#define DEVICE_KEY_GPIO_CFG  						IN_PINS_KEY_LCD_ENTER
			



// ����ģ���ӦIO 
#define MODE_IN_IO1									GPIOB,GPIO_Pin_3
#define MODE_IN_IO2									GPIOB,GPIO_Pin_4
#define MODE_IN_IO3									GPIOB,GPIO_Pin_5
#define MODE_IN_IO4									GPIOB,GPIO_Pin_6
#define MODE_IN_IO5									GPIOB,GPIO_Pin_7
#define MODE_IN_IO6									GPIOB,GPIO_Pin_9
//#define MODE_IN_IO7									GPIOC,GPIO_Pin_9
//#define MODE_IN_IO8									GPIOC,GPIO_Pin_10
//#define MODE_IN_IO9									GPIOA,GPIO_Pin_0
//#define MODE_IN_IO10								GPIOE,GPIO_Pin_3
//#define MODE_IN_IO11								GPIOE,GPIO_Pin_4






// 	
// IN
#define IN_KEY_GPIO_CFG \
				MODE_IN_IO1,   \
				MODE_IN_IO2,   \
				MODE_IN_IO3,   \
				MODE_IN_IO4,   \
				MODE_IN_IO5,   \
				MODE_IN_IO6,   \



#define LED_R_PIN					GPIOB,GPIO_Pin_13	
#define LED_G_PIN					GPIOC,GPIO_Pin_6	
#define LED_B_PIN					GPIOC,GPIO_Pin_8


#define LED_R_ON    \
			GPIO_ResetBits(LED_R_PIN);   \
			GPIO_SetBits(LED_B_PIN);\
			GPIO_SetBits(LED_G_PIN); \

#define LED_G_ON    \
			GPIO_ResetBits(LED_G_PIN);   \
			GPIO_SetBits(LED_R_PIN); \
			GPIO_SetBits(LED_B_PIN); \

#define LED_B_ON    \
			GPIO_ResetBits(LED_B_PIN);   \
			GPIO_SetBits(LED_R_PIN); \
			GPIO_SetBits(LED_G_PIN); \

#define LED_OFF   \
			GPIO_SetBits(LED_B_PIN); \
			GPIO_SetBits(LED_R_PIN); \
			GPIO_SetBits(LED_G_PIN); \

#define LED_ON   \
			GPIO_ResetBits(LED_B_PIN); \
			GPIO_ResetBits(LED_R_PIN); \
			GPIO_ResetBits(LED_G_PIN); \

////power1_ctrl
//#define TEST_POWER1_CTL_PIN						GPIOB,GPIO_Pin_3
//#define TEST_POWER1_ON							GPIO_SetBits( TEST_POWER1_CTL_PIN )	
//#define TEST_POWER1_OFF							GPIO_ResetBits( TEST_POWER1_CTL_PIN )  
////power3_ctrl
//#define TEST_POWER3_CTL_PIN						GPIOB,GPIO_Pin_5
//#define TEST_POWER3_ON							GPIO_SetBits( TEST_POWER3_CTL_PIN )	
//#define TEST_POWER3_OFF							GPIO_ResetBits( TEST_POWER3_CTL_PIN )
////power2_ctrl
//#define TEST_POWER2_CTL_PIN						GPIOB,GPIO_Pin_4
//#define TEST_POWER2_ON							GPIO_SetBits( TEST_POWER2_CTL_PIN )	
//#define TEST_POWER2_OFF							GPIO_ResetBits( TEST_POWER2_CTL_PIN )
////usart1_ctrl
//#define TEST_usart1_CTL_PIN						GPIOB,GPIO_Pin_7
//#define TEST_usart1_ON							GPIO_SetBits( TEST_usart1_CTL_PIN )	
//#define TEST_usart1_OFF							GPIO_ResetBits( TEST_usart1_CTL_PIN )
////power4_ctrl
//#define TEST_POWER4_CTL_PIN						GPIOB,GPIO_Pin_6
//#define TEST_POWER4_ON							GPIO_SetBits( TEST_POWER4_CTL_PIN )	
//#define TEST_POWER4_OFF							GPIO_ResetBits( TEST_POWER4_CTL_PIN )
////usart3_ctrl
//#define TEST_usart3_CTL_PIN						GPIOB,GPIO_Pin_9
//#define TEST_usart3_ON							GPIO_SetBits( TEST_usart3_CTL_PIN )	
//#define TEST_usart3_OFF							GPIO_ResetBits( TEST_usart3_CTL_PIN )
////usart2_ctrl
//#define TEST_usart2_CTL_PIN						GPIOB,GPIO_Pin_8
//#define TEST_usart2_ON							GPIO_SetBits( TEST_usart2_CTL_PIN )	
//#define TEST_usart2_OFF							GPIO_ResetBits( TEST_usart2_CTL_PIN )
////usart4_ctrl
//#define TEST_usart4_CTL_PIN						GPIOE,GPIO_Pin_1
//#define TEST_usart4_ON							GPIO_SetBits( TEST_usart4_CTL_PIN )	
//#define TEST_usart4_OFF							GPIO_ResetBits( TEST_usart4_CTL_PIN )

#define TEST_POWER_CTL_PIN						GPIOE,GPIO_Pin_1
#define TEST_POWER_ON							GPIO_SetBits( TEST_POWER_CTL_PIN )	
#define TEST_POWER_OFF							GPIO_ResetBits( TEST_POWER_CTL_PIN )

#define TEST_usart_CTL_PIN						GPIOE,GPIO_Pin_0
#define TEST_usart_ON							GPIO_SetBits( TEST_usart_CTL_PIN )	
#define TEST_usart_OFF							GPIO_ResetBits( TEST_usart_CTL_PIN )

// ϵͳָʾ��
#define EXTERNAL_SYS_LEN						GPIOB,GPIO_Pin_7




// 3V3 PD6
// 5V  PC12
#define EXTERNAL_POWER5V_CTL_PIN					GPIOC,GPIO_Pin_12
#define EXTERNAL_POWER5V_ON						GPIO_SetBits( EXTERNAL_POWER5V_CTL_PIN )	
#define EXTERNAL_POWER5V_OFF						GPIO_ResetBits( EXTERNAL_POWER5V_CTL_PIN )

#define EXTERNAL_POWER3V3_CTL_PIN					GPIOD,GPIO_Pin_6
#define EXTERNAL_POWER3V3_ON						GPIO_SetBits( EXTERNAL_POWER3V3_CTL_PIN )	
#define EXTERNAL_POWER3V3_OFF						GPIO_ResetBits( EXTERNAL_POWER3V3_CTL_PIN )

//WIFI POWER 3V3 PC13
#define WIFI_POWER_CTL_PIN					GPIOC,GPIO_Pin_13
#define WIFI_POWER_ON							GPIO_SetBits( WIFI_POWER_CTL_PIN )	
#define WIFI_POWER_OFF						GPIO_ResetBits( WIFI_POWER_CTL_PIN )

//DONGLE/NRF POW 3V3 PE3
#define DONGLE_POWER_CTL_PIN					GPIOE,GPIO_Pin_3
#define DONGLE_POWER_ON							GPIO_SetBits( DONGLE_POWER_CTL_PIN )	
#define DONGLE_POWER_OFF						GPIO_ResetBits( DONGLE_POWER_CTL_PIN )

#define CHENGKONG_KEY_PIN						GPIO_Pin_1
#define CHENGKONG_KEY_GPIO						GPIOB
#define IN_PINS_KEY_MODE_DOWN					CHENGKONG_KEY_GPIO,CHENGKONG_KEY_PIN

#define 	ACCOMPANYING_TEST_SWITCH 	GPIOC,GPIO_Pin_7

#define EXTERNAL_UART_SWITCH_PIN					GPIOE,GPIO_Pin_6
#define EXTERNAL_UART_SET_MCU						GPIO_SetBits( EXTERNAL_UART_SWITCH_PIN )	
#define EXTERNAL_UART_SET_DUT						GPIO_ResetBits( EXTERNAL_UART_SWITCH_PIN )

#define EXTERNAL_RELAY_CTRL_PIN					GPIOB,GPIO_Pin_3
#define EXTERNAL_RELAY_POWER_ON					GPIO_SetBits( EXTERNAL_RELAY_CTRL_PIN )	
#define EXTERNAL_RELAY_POWER_OFF				GPIO_ResetBits( EXTERNAL_RELAY_CTRL_PIN )

#define 	PTM_TYPEC_DM 	GPIOA,GPIO_Pin_0
#define 	PTM_TYPEC_DP 	GPIOA,GPIO_Pin_1

#define PTM_TYPEC_CC1_CTR					GPIOC,GPIO_Pin_2
#define EXTERNAL_CC1_CTR_UP						GPIO_SetBits( PTM_TYPEC_CC1_CTR )	
#define EXTERNAL_CC1_CTR_DOWN						GPIO_ResetBits( PTM_TYPEC_CC1_CTR )

#define PTM_TYPEC_CC2_CTR					GPIOC,GPIO_Pin_3
#define EXTERNAL_CC2_CTR_UP						GPIO_SetBits( PTM_TYPEC_CC2_CTR )	
#define EXTERNAL_CC2_CTR_DOWN						GPIO_ResetBits( PTM_TYPEC_CC2_CTR )

#define PTM_TYPEC_DM_SHORT_IO					GPIOE,GPIO_Pin_4
#define EXTERNAL_DM_SHORT_IO_UP						GPIO_SetBits( PTM_TYPEC_DM_SHORT_IO )	
#define EXTERNAL_DM_SHORT_IO_DOWN						GPIO_ResetBits( PTM_TYPEC_DM_SHORT_IO )

#define PTM_TYPEC_DP_SHORT_IO					GPIOE,GPIO_Pin_5
#define EXTERNAL_DP_SHORT_IO_UP						GPIO_SetBits( PTM_TYPEC_DP_SHORT_IO )	
#define EXTERNAL_DP_SHORT_IO_DOWN						GPIO_ResetBits( PTM_TYPEC_DP_SHORT_IO )

// External support function describe
void gpio_extern_system_driver_init(void);
void gpio_extern_led_system_flash(void);




#endif


