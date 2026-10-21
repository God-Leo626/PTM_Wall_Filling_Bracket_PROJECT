/*
 * @Author: “HP “1357243695@qq.com”
 * @Date: 2023-07-20 13:40:11
 * @LastEditors: “HP “1357243695@qq.com”
 * @LastEditTime: 2023-11-13 16:38:20
 * @FilePath: \Guider\APP\usart\usart.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include "usart.h"

void usart1_init(u32 bound)
{
		//初始化io管脚类型的结构体变量
	GPIO_InitTypeDef  gpio_init_type_struct;
	
	//初始化串口的结构体变量（波特率，校验位，停止位，传输字长）
	USART_InitTypeDef  usart_init_type_strcut;
	
	//初始化中断优先级的结构体变量（抢占优先级，响应优先级）
	NVIC_InitTypeDef  nvic_init_type_strcut;
	
	
	//使能时钟 GPIOA,因为UART1 ,用到PA9  PA10 两个管脚
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	//使能uart1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
	
	//复位串口1
	USART_DeInit(USART1);
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);  //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
 
	
	
	//初始化io口，使其成为TX  RX
	// PA9 = TX    PA10 = RX
	gpio_init_type_struct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	gpio_init_type_struct.GPIO_Mode = GPIO_Mode_AF;           //复用功能
	gpio_init_type_struct.GPIO_Speed = GPIO_Speed_100MHz;	   //速度50MHz
	gpio_init_type_struct.GPIO_OType = GPIO_OType_PP;         //推挽复用输出
	gpio_init_type_struct.GPIO_PuPd = GPIO_PuPd_UP;           //上拉
	GPIO_Init(GPIOA,&gpio_init_type_struct);
	
	
	//串口参数初始化 115200 8N1  TX/RX  NC
	usart_init_type_strcut.USART_BaudRate = bound; //波特率
	usart_init_type_strcut.USART_HardwareFlowControl =USART_HardwareFlowControl_None; //无硬件数据流控制
	usart_init_type_strcut.USART_Mode = USART_Mode_Rx | USART_Mode_Tx ;                //串口收，发模式
	usart_init_type_strcut.USART_Parity = USART_Parity_No;                             //无校验
	usart_init_type_strcut.USART_StopBits = USART_StopBits_1;                          //停止位1bit
	usart_init_type_strcut.USART_WordLength = USART_WordLength_8b;                     //数据位8bit
	USART_Init(USART1,&usart_init_type_strcut);
	
	USART_Cmd(USART1,ENABLE);	//使能串口1 
	
}

void vUsar1tDisable()
{
	uint8_t tmp;
	NVIC_InitTypeDef NVIC_InitStructure;

	// Enable the USART1 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;			
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);


	// 关闭发送和接收中断
	USART_ITConfig(USART1, USART_IT_RXNE|USART_IT_TXE, DISABLE);
	USART_Cmd(USART1,DISABLE);									
		
	
	// 空读两次，清除串口接收缓冲
	tmp = USART_ReceiveData(USART1);
	tmp += USART_ReceiveData(USART1);
	USART_DeInit( USART1 );				

	USART_Cmd(USART1,DISABLE);
}



void usart2_init(u32 bound)
{
		//初始化io管脚类型的结构体变量
	GPIO_InitTypeDef  gpio_init_type_struct;
	
	//初始化串口的结构体变量（波特率，校验位，停止位，传输字长）
	USART_InitTypeDef  usart_init_type_strcut;
	
	//初始化中断优先级的结构体变量（抢占优先级，响应优先级）
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	
	//使能时钟 GPIOA,因为UART1 ,用到PA9  PA10 两个管脚
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	//使能uart1时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
	
	//复位串口1
	USART_DeInit(USART2);
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);  
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); 
 
	
	//初始化io口，使其成为TX  RX
	// PA9 = TX    PA10 = RX
	gpio_init_type_struct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA9与GPIOA10
	gpio_init_type_struct.GPIO_Mode = GPIO_Mode_AF;           //复用功能
	gpio_init_type_struct.GPIO_Speed = GPIO_Speed_50MHz;	   //速度50MHz
	gpio_init_type_struct.GPIO_OType = GPIO_OType_PP;         //推挽复用输出
	gpio_init_type_struct.GPIO_PuPd = GPIO_PuPd_UP;           //上拉
	GPIO_Init(GPIOA,&gpio_init_type_struct);
	
	
	//串口参数初始化 115200 8N1  TX/RX  NC
	usart_init_type_strcut.USART_BaudRate = bound; //波特率
	usart_init_type_strcut.USART_HardwareFlowControl =USART_HardwareFlowControl_None; //无硬件数据流控制
	usart_init_type_strcut.USART_Mode = USART_Mode_Rx | USART_Mode_Tx ;                //串口收，发模式
	usart_init_type_strcut.USART_Parity = USART_Parity_No;                             //无校验
	usart_init_type_strcut.USART_StopBits = USART_StopBits_1;                          //停止位1bit
	usart_init_type_strcut.USART_WordLength = USART_WordLength_8b;                     //数据位8bit
	USART_Init(USART2,&usart_init_type_strcut);
	
	USART_Cmd(USART2,ENABLE);


	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断
	//Usart1 NVIC 配置
 	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
}



void usart3_init(u32 bound)
{
	//初始化io管脚类型的结构体变量
	GPIO_InitTypeDef  gpio_init_type_struct;
	
	//初始化串口的结构体变量（波特率，校验位，停止位，传输字长）
	USART_InitTypeDef  usart_init_type_strcut;
	
	//初始化中断优先级的结构体变量（抢占优先级，响应优先级）
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	
	//使能时钟 GPIOA,因为UART1 ,用到PA9  PA10 两个管脚
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOA时钟
	//使能uart1时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);
	
	//复位串口1
	USART_DeInit(USART3);
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);  
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); 
 
	
	//初始化io口，使其成为TX  RX
	// PA9 = TX    PA10 = RX
	gpio_init_type_struct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9与GPIOA10
	gpio_init_type_struct.GPIO_Mode = GPIO_Mode_AF;           //复用功能
	gpio_init_type_struct.GPIO_Speed = GPIO_Speed_50MHz;	   //速度50MHz
	gpio_init_type_struct.GPIO_OType = GPIO_OType_PP;         //推挽复用输出
	gpio_init_type_struct.GPIO_PuPd = GPIO_PuPd_UP;           //上拉
	GPIO_Init(GPIOB,&gpio_init_type_struct);
	
	
	//串口参数初始化 115200 8N1  TX/RX  NC
	usart_init_type_strcut.USART_BaudRate = bound; //波特率
	usart_init_type_strcut.USART_HardwareFlowControl =USART_HardwareFlowControl_None; //无硬件数据流控制
	usart_init_type_strcut.USART_Mode = USART_Mode_Rx | USART_Mode_Tx ;                //串口收，发模式
	usart_init_type_strcut.USART_Parity = USART_Parity_No;                             //无校验
	usart_init_type_strcut.USART_StopBits = USART_StopBits_1;                          //停止位1bit
	usart_init_type_strcut.USART_WordLength = USART_WordLength_8b;                     //数据位8bit
	USART_Init(USART3,&usart_init_type_strcut);
	
	USART_Cmd(USART3,ENABLE);


	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断
	//Usart1 NVIC 配置
 	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
}





void vUsar2tDisable(void)
{
	uint8_t tmp;
	NVIC_InitTypeDef NVIC_InitStructure;

	// Enable the USART1 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;			
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);


	// 关闭发送和接收中断
	USART_ITConfig(USART2, USART_IT_RXNE|USART_IT_TXE, DISABLE);
	USART_Cmd(USART2,DISABLE);									
		
	
	// 空读两次，清除串口接收缓冲
	tmp = USART_ReceiveData(USART2);
	tmp += USART_ReceiveData(USART2);
	USART_DeInit( USART2 );				

	USART_Cmd(USART2,DISABLE);
}







void usart4_init(u32 bound)
{
	//初始化io管脚类型的结构体变量
	GPIO_InitTypeDef  gpio_init_type_struct;
	
	//初始化串口的结构体变量（波特率，校验位，停止位，传输字长）
	USART_InitTypeDef  usart_init_type_strcut;
	
	//初始化中断优先级的结构体变量（抢占优先级，响应优先级）
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	
	//使能时钟 GPIOA,因为UART1 ,用到PA9  PA10 两个管脚
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOA时钟
	//使能uart1时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4 , ENABLE);
	
	//复位串口1
	USART_DeInit(UART4);
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4);  
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); 
 
	
	//初始化io口，使其成为TX  RX
	// PA9 = TX    PA10 = RX
	gpio_init_type_struct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9与GPIOA10
	gpio_init_type_struct.GPIO_Mode = GPIO_Mode_AF;           //复用功能
	gpio_init_type_struct.GPIO_Speed = GPIO_Speed_50MHz;	   //速度50MHz
	gpio_init_type_struct.GPIO_OType = GPIO_OType_PP;         //推挽复用输出
	gpio_init_type_struct.GPIO_PuPd = GPIO_PuPd_UP;           //上拉
	GPIO_Init(GPIOC,&gpio_init_type_struct);
	
	
	//串口参数初始化 115200 8N1  TX/RX  NC
	usart_init_type_strcut.USART_BaudRate = bound; //波特率
	usart_init_type_strcut.USART_HardwareFlowControl =USART_HardwareFlowControl_None; //无硬件数据流控制
	usart_init_type_strcut.USART_Mode = USART_Mode_Rx | USART_Mode_Tx ;                //串口收，发模式
	usart_init_type_strcut.USART_Parity = USART_Parity_No;                             //无校验
	usart_init_type_strcut.USART_StopBits = USART_StopBits_1;                          //停止位1bit
	usart_init_type_strcut.USART_WordLength = USART_WordLength_8b;                     //数据位8bit
	USART_Init(UART4,&usart_init_type_strcut);
	
	USART_Cmd(UART4,ENABLE);

	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启相关中断
	//Usart1 NVIC 配置
 	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
}


void uart3_disable(void)
{
	uint8_t tmp;
	NVIC_InitTypeDef NVIC_InitStructure;

	// Enable the USART1 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;			
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);

	// 关闭发送和接收中断
	USART_ITConfig(USART3, USART_IT_RXNE|USART_IT_TXE, DISABLE);
	USART_Cmd(USART3,DISABLE);									
		
	// 空读两次，清除串口接收缓冲
	tmp = USART_ReceiveData(USART3);
	tmp += USART_ReceiveData(USART3);
	USART_DeInit( USART3 );				

	USART_Cmd(USART3,DISABLE);
}




void uart4_disable(void)
{
	uint8_t tmp;
	NVIC_InitTypeDef NVIC_InitStructure;

	// Enable the USART1 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;			
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);

	// 关闭发送和接收中断
	USART_ITConfig(UART4, USART_IT_RXNE|USART_IT_TXE, DISABLE);
	USART_Cmd(UART4,DISABLE);									
		
	// 空读两次，清除串口接收缓冲
	tmp = USART_ReceiveData(UART4);
	tmp += USART_ReceiveData(UART4);
	USART_DeInit( UART4 );				

	USART_Cmd(UART4,DISABLE);
}


void uart_extern_burn_and_at_uart_tx_buf(uint8_t * p_tx_buf, uint16_t len )
{
	uint16_t count = len;

	while ( count-- )
	{
		// 数据写入串口发送缓冲
		USART_SendData( USART2, *p_tx_buf++ );	
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	
	}
}






#if NO_USE_MICRO_LIB   // 使用标准库时编译的代码（不使用micro lib）
	#include <stdio.h> 
	/* 告知连接器不从C库链接使用半主机的函数 */
	#pragma import(__use_no_semihosting) 
	/* 定义 _sys_exit() 以避免使用半主机模式 */
	void _sys_exit(int x)
	{
		x = x;
	} 
	/* 标准库需要的支持类型 */
	struct __FILE
	{
		int handle;
	};
	FILE __stdout;
	int fputc(int ch, FILE *stream)
	{
		//堵塞判断串口是否发送完成 
		while((USART1->SR & 0X40) == 0);
		//串口发送完成，将该字符发送 
		USART1->DR = (uint8_t) ch;
		return ch;
	}	
#else  //使用micro lib 时候编译的代码
	#include <stdio.h>
	int fputc(int ch, FILE *stream)
	{
		/* 堵塞判断串口是否发送完成 */
		while((USART1->SR & 0X40) == 0);
		/* 串口发送完成，将该字符发送 */
		USART1->DR = (uint8_t) ch;
		return ch;
	}

#endif

