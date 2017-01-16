////////////////////////////////////////////////////////////////////////////////
// File Name          : stm32f10x_it.c
// Author             : MCD Application Team
// Version            : V1.1.1
// Date               : 06/13/2008
// Description        : Main Interrupt Service Routines.
//                      This file provides template for all exceptions handler
//                      and peripherals interrupt service routine.
//
// THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
// WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
// AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
// INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
// CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
// INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
////////////////////////////////////////////////////////////////////////////////
#include  "def_application.h"

void NMIException( void ) 							{ while (1) {} }
void HardFaultException( void ) 				{ while (1) {} }
void MemManageException( void ) 				{ while (1) {} }
void BusFaultException( void ) 					{ while (1) {} }
void UsageFaultException( void ) 				{ while (1) {} }
void DebugMonitor( void ) 							{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void SVCHandler( void ) 								{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void PendSVC( void ) 										{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }

void SysTickHandler( void )
{
  ENTR_CRT_SECTION();
  DecrementTimeDelay();
  EXT_CRT_SECTION();
}

void WWDG_IRQHandler( void ) 						{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void PVD_IRQHandler( void )							{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void TAMPER_IRQHandler( void )					{ }

void RTC_IRQHandler( void )
{
  ENTR_CRT_SECTION();
  EXT_CRT_SECTION();
}

void FLASH_IRQHandler( void )						{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void RCC_IRQHandler( void )							{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void EXTI0_IRQHandler( void )           { ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void EXTI1_IRQHandler( void )           { ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void EXTI2_IRQHandler( void )						{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void EXTI3_IRQHandler( void )						{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void EXTI4_IRQHandler( void )						{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }

void DMA1_Channel1_IRQHandler( void )
{
	ENTR_CRT_SECTION();
	ISRADCDMA1Channel1();
	EXT_CRT_SECTION();
}

void DMA1_Channel2_IRQHandler( void ) 	{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void DMA1_Channel3_IRQHandler( void ) 	{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void DMA1_Channel4_IRQHandler( void ) 	{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void DMA1_Channel5_IRQHandler( void ) 	{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void DMA1_Channel6_IRQHandler( void )		{	ENTR_CRT_SECTION();	EXT_CRT_SECTION(); }
void DMA1_Channel7_IRQHandler( void ) 	{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void ADC1_2_IRQHandler( void )					{	ENTR_CRT_SECTION();	EXT_CRT_SECTION(); }
void USB_HP_CAN_TX_IRQHandler( void )		{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }

void USB_LP_CAN_RX0_IRQHandler( void )
{
  ENTR_CRT_SECTION();
  ISRCAN();
  EXT_CRT_SECTION();
}

void CAN_RX1_IRQHandler( void )					{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void CAN_SCE_IRQHandler( void )					{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void EXTI9_5_IRQHandler( void )					{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void TIM1_BRK_IRQHandler( void )				{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void TIM1_UP_IRQHandler( void )					{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void TIM1_TRG_COM_IRQHandler( void )		{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void TIM1_CC_IRQHandler( void )					{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void TIM2_IRQHandler( void )            { ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }

void TIM3_IRQHandler( void )
{
  ENTR_CRT_SECTION();
  TIM3ISR_CaptureCounter();
  EXT_CRT_SECTION();
}

void TIM4_IRQHandler( void )
{
  ENTR_CRT_SECTION();
  TIM4ISR_CaptureCounter();
  EXT_CRT_SECTION();
}

void I2C1_EV_IRQHandler( void )					{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void I2C1_ER_IRQHandler( void )					{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void I2C2_EV_IRQHandler( void )					{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void I2C2_ER_IRQHandler( void )					{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void SPI1_IRQHandler( void )						{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void SPI2_IRQHandler( void )						{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }

void USART1_IRQHandler( void )
{
  ENTR_CRT_SECTION();
  USART1_ISR();
  EXT_CRT_SECTION();
}

void USART2_IRQHandler( void )          { ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }

void USART3_IRQHandler( void )
{
  ENTR_CRT_SECTION();
  USART3_ISR();
  EXT_CRT_SECTION();
}

void EXTI15_10_IRQHandler( void )       { ENTR_CRT_SECTION();	EXT_CRT_SECTION(); }
void RTCAlarm_IRQHandler( void )				{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void USBWakeUp_IRQHandler( void )				{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void TIM8_BRK_IRQHandler( void )				{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void TIM8_UP_IRQHandler( void )					{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void TIM8_TRG_COM_IRQHandler( void )		{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void TIM8_CC_IRQHandler( void )					{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void ADC3_IRQHandler( void )						{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void FSMC_IRQHandler( void )						{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void SDIO_IRQHandler( void )						{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }

void TIM5_IRQHandler( void )
{
  ENTR_CRT_SECTION();
  ISRTIM5forADC();
  EXT_CRT_SECTION();
}

void SPI3_IRQHandler( void )						{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void UART4_IRQHandler( void )						{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void UART5_IRQHandler( void )						{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void TIM6_IRQHandler( void )						{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void TIM7_IRQHandler( void )						{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void DMA2_Channel1_IRQHandler( void )		{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void DMA2_Channel2_IRQHandler( void )		{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void DMA2_Channel3_IRQHandler( void )		{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }
void DMA2_Channel4_5_IRQHandler( void )	{ ENTR_CRT_SECTION(); EXT_CRT_SECTION(); }

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
