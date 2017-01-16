/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Company           : MesaMedical Co., Ltd.
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Microcontroller		: Cortex M4 STM32F407ZET6
//  Revision          : V1.00
//  Project           : Ultra Surgery 2013
//  Editor            : Hyunju Lee
//  Create Date       : 2013-07-30
//  File              : STM32F4xx_IT.C
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "m4_evkit_define.h"

#include "m4_cecs_io_base.h"
#include "m4_cecs_uart1.h"
#include "m4_cecs_uart3.h"
#include "m4_cecs_uart6.h"
#include "m4_cecs_can.h"
#include "m4_cecs_analog.h"

MESSAGE   iMSG;

// system interrupt handler
void NMI_Handler									( void )	{	while( 1 ) {} }
void HardFault_Handler						( void )	{	while( 1 ) {}	}
void MemManage_Handler						( void )	{	while( 1 ) {}	}
void BusFault_Handler							( void )	{	while( 1 ) {}	}
void UsageFault_Handler						( void )	{	while( 1 ) {}	}
void SVC_Handler									( void )	{	while( 1 ) {} }
void DebugMon_Handler							( void )	{	while( 1 ) {} }
void PendSV_Handler								( void )	{	while( 1 ) {} }

// system_tick_timer
void SysTick_Handler( void )
{
  ISR_SysTickHandler();
}

// external interrupt handler
void WWDG_IRQHandler              ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }
void PVD_IRQHandler               ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void TAMP_STAMP_IRQHandler        ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void RTC_WKUP_IRQHandler          ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void FLASH_IRQHandler             ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void RCC_IRQHandler               ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); } 
void EXTI0_IRQHandler             ( void )  {	ENTR_CRT_SECTION(); 	EXT_CRT_SECTION(); }
void EXTI1_IRQHandler             ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void EXTI2_IRQHandler             ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void EXTI3_IRQHandler             ( void )  { ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void EXTI4_IRQHandler             ( void )  { ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void DMA1_Stream0_IRQHandler			( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void DMA1_Stream1_IRQHandler      ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void DMA1_Stream2_IRQHandler      ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void DMA1_Stream3_IRQHandler      ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void DMA1_Stream4_IRQHandler      ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void DMA1_Stream5_IRQHandler      ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void DMA1_Stream6_IRQHandler      ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   

void ADC_IRQHandler( void )
{
	ENTR_CRT_SECTION();
  ISR_AtoDConverter();
	EXT_CRT_SECTION();
}   

void CAN1_TX_IRQHandler           ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }

void CAN1_RX0_IRQHandler( void )
{
  ENTR_CRT_SECTION();
  ISR_CAN1_Reception();
  EXT_CRT_SECTION();
}   

void CAN1_RX1_IRQHandler          ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void CAN1_SCE_IRQHandler          ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void EXTI9_5_IRQHandler           ( void )  { ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void TIM1_BRK_TIM9_IRQHandler     ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void TIM1_UP_TIM10_IRQHandler     ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void TIM1_TRG_COM_TIM11_IRQHandler( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void TIM1_CC_IRQHandler           ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }

void TIM2_IRQHandler( void )
{
  ENTR_CRT_SECTION();
  ISR_TIM2forCANTimeOut();
  EXT_CRT_SECTION();
}   

void TIM3_IRQHandler( void )
{
	ENTR_CRT_SECTION();
  ISR_TIM3_CaptureCounter();
  EXT_CRT_SECTION();
}

void TIM4_IRQHandler( void )
{
	ENTR_CRT_SECTION();
  ISR_TIM4_CaptureCounter();
	EXT_CRT_SECTION();
}

void I2C1_EV_IRQHandler           ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void I2C1_ER_IRQHandler           ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void I2C2_EV_IRQHandler           ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void I2C2_ER_IRQHandler           ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void SPI1_IRQHandler              ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void SPI2_IRQHandler              ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }

void USART1_IRQHandler( void )
{
  ENTR_CRT_SECTION();
  ISR_USART1();
  EXT_CRT_SECTION();
}

void USART2_IRQHandler            ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   

void USART3_IRQHandler( void )
{
  ENTR_CRT_SECTION();
  ISR_USART3();
  EXT_CRT_SECTION();
}   

void EXTI15_10_IRQHandler         ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void RTC_Alarm_IRQHandler         ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void OTG_FS_WKUP_IRQHandler       ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void TIM8_BRK_TIM12_IRQHandler    ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void TIM8_UP_TIM13_IRQHandler     ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void TIM8_TRG_COM_TIM14_IRQHandler( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void TIM8_CC_IRQHandler           ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void DMA1_Stream7_IRQHandler      ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void FSMC_IRQHandler              ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void SDIO_IRQHandler              ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void TIM5_IRQHandler							( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }
void SPI3_IRQHandler              ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void UART4_IRQHandler             ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void UART5_IRQHandler             ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void TIM6_DAC_IRQHandler          ( void )  {	ENTR_CRT_SECTION(); 	EXT_CRT_SECTION(); }   

void TIM7_IRQHandler( void )
{
	ENTR_CRT_SECTION();
  ISR_TIM7forAtoDConverter();
  EXT_CRT_SECTION();
}   

void DMA2_Stream0_IRQHandler			( void )	{	ENTR_CRT_SECTION();		EXT_CRT_SECTION(); } 
void DMA2_Stream1_IRQHandler      ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void DMA2_Stream2_IRQHandler      ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void DMA2_Stream3_IRQHandler      ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void DMA2_Stream4_IRQHandler      ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void ETH_IRQHandler               ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void ETH_WKUP_IRQHandler          ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void CAN2_TX_IRQHandler           ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void CAN2_RX0_IRQHandler          ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void CAN2_RX1_IRQHandler          ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void CAN2_SCE_IRQHandler          ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void OTG_FS_IRQHandler            ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void DMA2_Stream5_IRQHandler      ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void DMA2_Stream6_IRQHandler      ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void DMA2_Stream7_IRQHandler      ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   

void USART6_IRQHandler( void )
{
  ENTR_CRT_SECTION();
  ISR_USART6();
  EXT_CRT_SECTION();
}   

void I2C3_EV_IRQHandler           ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void I2C3_ER_IRQHandler           ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void OTG_HS_EP1_OUT_IRQHandler    ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void OTG_HS_EP1_IN_IRQHandler     ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void OTG_HS_WKUP_IRQHandler       ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void OTG_HS_IRQHandler            ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void DCMI_IRQHandler              ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void CRYP_IRQHandler              ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void HASH_RNG_IRQHandler          ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }   
void FPU_IRQHandler               ( void )	{ ENTR_CRT_SECTION();		EXT_CRT_SECTION(); }      
