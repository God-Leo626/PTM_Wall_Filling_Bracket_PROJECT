#include "fifo.h"


static volatile UART_FIFO_RX s_fifo_rx;
static volatile UART_FIFO_RX usb_fifo_rx;

/************************************************************************************************
* 功能描述： 	串口接收中断处理函数，将收到的数据保存到FIFO缓冲中
* 输入参数：   	
* 输出参数：	无
* 返回值     ： 	无
* 其它说明：
* 修改记录：   
* 修改日期		  版本号		       修改人		 修改内容
*  ----------------------------------------------------------------------------
* 20160316		    V1.0 			        Alex		    创建
**************************************************************************************************/
void vFifoUartRxIsr( uint8_t rxd )
{
	uint32_t cnt;

	// 长度计算
	if ( s_fifo_rx.count<s_fifo_rx.r_count )
	{
		cnt = s_fifo_rx.count+UART_FIFO_RX_BUFFER_LEN-s_fifo_rx.r_count;
	}
	else
	{
		cnt = s_fifo_rx.count-s_fifo_rx.r_count;
	}

	// 缓冲区满，丢弃数据
	if ( cnt>=UART_FIFO_RX_BUFFER_LEN )		
	{
		return;
	}

	// 将数据保存到接收缓冲，同时将写入指针后移
	s_fifo_rx.buf[s_fifo_rx.wpos++] = rxd;	
	s_fifo_rx.count++;					
	// 写入指针环回检测
	if ( s_fifo_rx.wpos>=UART_FIFO_RX_BUFFER_LEN )		
	{
		s_fifo_rx.wpos = 0;
	}

	// 写入指针环回检测
	if ( s_fifo_rx.count>=UART_FIFO_RX_BUFFER_LEN )		
	{
		s_fifo_rx.count = 0;
	}
	
}

/************************************************************************************************
* 功能描述： 	清除串口接收FIFO
* 输入参数：   	
* 输出参数：	无
* 返回值     ： 	无
* 其它说明：
* 修改记录：   
* 修改日期		  版本号		       修改人		 修改内容
*  ----------------------------------------------------------------------------
* 20160316		    V1.0 			        Alex		    创建
**************************************************************************************************/
void vFifoUatrClearFifo( void )
{

	s_fifo_rx.wpos = 0;
	s_fifo_rx.rpos = 0;
	s_fifo_rx.count = 0;
	s_fifo_rx.r_count = 0;
	
}

/************************************************************************************************
* 功能描述： 	读取串口FIFO缓冲中的数据个数
* 输入参数：   	
* 输出参数：	无
* 返回值     ： 	无
* 其它说明：
* 修改记录：   
* 修改日期		  版本号		       修改人		 修改内容
*  ----------------------------------------------------------------------------
* 20160316		    V1.0 			        Alex		    创建
**************************************************************************************************/
int vFifoUatrGetFifoCount( void )
{
	if ( s_fifo_rx.count<s_fifo_rx.r_count )
	{
		return s_fifo_rx.count+UART_FIFO_RX_BUFFER_LEN-s_fifo_rx.r_count;
	}
	else
	{
		return s_fifo_rx.count-s_fifo_rx.r_count;
	}
}


/************************************************************************************************
* 功能描述： 	从接收FIFO读取一字节数据
* 输入参数：   	
* 输出参数：	无
* 返回值     ： 	无
* 其它说明：	仅在FIFO中有数据时调用此函数才有意义，
					若FIFO为空则会直接返回0 
* 修改记录：   
* 修改日期		  版本号		       修改人		 修改内容
*  ----------------------------------------------------------------------------
* 20160316		    V1.0 			        Alex		    创建
**************************************************************************************************/
uint8_t vFifoUatrGetFifoDat( void )
{
	uint8_t ret;
	
	if ( s_fifo_rx.r_count == s_fifo_rx.count )
	{
		return 0;
	}

	// 从缓冲读出数据
	ret = s_fifo_rx.buf[s_fifo_rx.rpos++];
	s_fifo_rx.r_count++;
	// 读出指针环回检测
	if ( s_fifo_rx.rpos>=UART_FIFO_RX_BUFFER_LEN)		
	{
		s_fifo_rx.rpos = 0;
	}

	// 写入指针环回检测
	if ( s_fifo_rx.r_count>=UART_FIFO_RX_BUFFER_LEN )		
	{
		s_fifo_rx.r_count = 0;
	}
	
	return ret;
}
















/************************************************************************************************
* 功能描述： 	串口接收中断处理函数，将收到的数据保存到FIFO缓冲中
* 输入参数：   	
* 输出参数：	无
* 返回值     ： 	无
* 其它说明：
* 修改记录：   
* 修改日期		  版本号		       修改人		 修改内容
*  ----------------------------------------------------------------------------
* 20160316		    V1.0 			        Alex		    创建
**************************************************************************************************/
void vFifoUsbRxIsr( uint8_t rxd )
{
	uint32_t cnt;

	// 长度计算
	if ( usb_fifo_rx.count<usb_fifo_rx.r_count )
	{
		cnt = usb_fifo_rx.count+UART_FIFO_RX_BUFFER_LEN-usb_fifo_rx.r_count;
	}
	else
	{
		cnt = usb_fifo_rx.count-usb_fifo_rx.r_count;
	}

	// 缓冲区满，丢弃数据
	if ( cnt>=UART_FIFO_RX_BUFFER_LEN )		
	{
		return;
	}

	// 将数据保存到接收缓冲，同时将写入指针后移
	usb_fifo_rx.buf[usb_fifo_rx.wpos++] = rxd;	
	usb_fifo_rx.count++;					
	// 写入指针环回检测
	if ( usb_fifo_rx.wpos>=UART_FIFO_RX_BUFFER_LEN )		
	{
		usb_fifo_rx.wpos = 0;
	}

	// 写入指针环回检测
	if ( usb_fifo_rx.count>=UART_FIFO_RX_BUFFER_LEN )		
	{
		usb_fifo_rx.count = 0;
	}

}

/************************************************************************************************
* 功能描述： 	清除串口接收FIFO
* 输入参数：   	
* 输出参数：	无
* 返回值     ： 	无
* 其它说明：
* 修改记录：   
* 修改日期		  版本号		       修改人		 修改内容
*  ----------------------------------------------------------------------------
* 20160316		    V1.0 			        Alex		    创建
**************************************************************************************************/
void vFifoUsbClearFifo( void )
{

	usb_fifo_rx.wpos = 0;
	usb_fifo_rx.rpos = 0;
	usb_fifo_rx.count = 0;
	usb_fifo_rx.r_count = 0;
	
	
}

/************************************************************************************************
* 功能描述： 	读取串口FIFO缓冲中的数据个数
* 输入参数：   	
* 输出参数：	无
* 返回值     ： 	无
* 其它说明：
* 修改记录：   
* 修改日期		  版本号		       修改人		 修改内容
*  ----------------------------------------------------------------------------
* 20160316		    V1.0 			        Alex		    创建
**************************************************************************************************/
int vFifoUsbGetFifoCount( void )
{
	if ( usb_fifo_rx.count<usb_fifo_rx.r_count )
	{
		return usb_fifo_rx.count+UART_FIFO_RX_BUFFER_LEN-usb_fifo_rx.r_count;
	}
	else
	{
		return usb_fifo_rx.count-usb_fifo_rx.r_count;
	}
}


/************************************************************************************************
* 功能描述： 	从接收FIFO读取一字节数据
* 输入参数：   	
* 输出参数：	无
* 返回值     ： 	无
* 其它说明：	仅在FIFO中有数据时调用此函数才有意义，
					若FIFO为空则会直接返回0 
* 修改记录：   
* 修改日期		  版本号		       修改人		 修改内容
*  ----------------------------------------------------------------------------
* 20160316		    V1.0 			        Alex		    创建
**************************************************************************************************/
uint8_t xFifoUsbGetFifoDat( void )
{
	uint8_t ret;
	
	if ( usb_fifo_rx.r_count == usb_fifo_rx.count )
	{
		return 0;
	}

	// 从缓冲读出数据
	ret = usb_fifo_rx.buf[usb_fifo_rx.rpos++];
	usb_fifo_rx.r_count++;
	// 读出指针环回检测
	if ( usb_fifo_rx.rpos>=UART_FIFO_RX_BUFFER_LEN)		
	{
		usb_fifo_rx.rpos = 0;
	}

	// 写入指针环回检测
	if ( usb_fifo_rx.r_count>=UART_FIFO_RX_BUFFER_LEN )		
	{
		usb_fifo_rx.r_count = 0;
	}
	
	return ret;
}


