/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "user.h"
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}



/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{

}


// void USART2_IRQHandler(void)
// {
//   	uint8_t tmp;

//     if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)	
//     {
//       // 接收到数据
//       tmp = USART_ReceiveData( USART2 );
//       vFifoUartRxIsr(tmp); 
//       TRACE_WP("%02X ",tmp);
//       // 清除中断标志
//       USART_ClearITPendingBit( USART2, USART_IT_RXNE );				
//     }
//     // ORE缓冲溢出错误（必须处理，否则会使程序不停进入串口中断）
//     if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)		
//     {
//       // 先读SR，后读DR，清除ORE状态
//       tmp = USART2->SR;
//       tmp = USART2->DR;
//       TRACE_WP("ORE =%02x",tmp);
//     }

// }

void USART2_IRQHandler(void)
{
  	uint8_t tmp;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)	
    {
      // 接收到数据
      tmp = USART_ReceiveData( USART2 );
      vFifoUartRxIsr(tmp); 
      // 清除中断标志
      USART_ClearITPendingBit( USART2, USART_IT_RXNE );				
    }
    // ORE缓冲溢出错误（必须处理，否则会使程序不停进入串口中断）
    if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)		
    {
      // 先读SR，后读DR，清除ORE状态
      tmp = USART2->SR;
      tmp = USART2->DR;
      TRACE_WP("ORE =%02x",tmp);
    }
}



// void USART3_IRQHandler(void)
// {
//   	uint8_t tmp;

//     if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)	
//     {
//       // 接收到数据
//       tmp = USART_ReceiveData( USART3 );
//       vFifoUartRxIsr(tmp); 
//       // 清除中断标志
//       USART_ClearITPendingBit( USART3, USART_IT_RXNE );				
//     }
//     // ORE缓冲溢出错误（必须处理，否则会使程序不停进入串口中断）
//     if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)		
//     {
//       // 先读SR，后读DR，清除ORE状态
//       tmp = USART3->SR;
//       tmp = USART3->DR;
//       TRACE_WP("ORE =%02x",tmp);
//     }

// }




//void UART4_IRQHandler(void)
// {
//   	uint8_t tmp;

//     if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)	
//     {
//       // 接收到数据
//       tmp = USART_ReceiveData( UART4 );
//       vFifoUartRxIsr(tmp); 
//       TRACE_WP("%02X ",tmp);
//       // 清除中断标志
//       USART_ClearITPendingBit( UART4, USART_IT_RXNE );				
//     }
//     // ORE缓冲溢出错误（必须处理，否则会使程序不停进入串口中断）
//     if (USART_GetFlagStatus(UART4, USART_FLAG_ORE) != RESET)		
//     {
//       // 先读SR，后读DR，清除ORE状态
//       tmp = UART4->SR;
//       tmp = UART4->DR;
//       TRACE_WP("ORE =%02x",tmp);
//     }
// }



/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
