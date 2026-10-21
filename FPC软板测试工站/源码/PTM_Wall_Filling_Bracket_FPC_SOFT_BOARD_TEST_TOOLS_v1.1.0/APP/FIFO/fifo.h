 #ifndef __FIFO__H__
 #define __FIFO__H__
 
 #include <stdint.h>

#define UART_FIFO_RX_BUFFER_LEN					5120


typedef struct _UART_FIFO_RX_BUF_
{
	int wpos;
	int rpos;
	int count;
	int r_count;
	uint8_t buf[UART_FIFO_RX_BUFFER_LEN];
} UART_FIFO_RX;




void vFifoUartRxIsr( uint8_t rxd );
void vFifoUatrClearFifo( void );
int vFifoUatrGetFifoCount( void );
uint8_t vFifoUatrGetFifoDat( void );




void vFifoUsbRxIsr( uint8_t rxd );
void vFifoUsbClearFifo( void );
int vFifoUsbGetFifoCount( void );
uint8_t xFifoUsbGetFifoDat( void );



 #endif