////////////////////////////////////////////////////////////////////////////////
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Microcontroller		: Cortex M4 STM32F407ZET6
//  Revision          : 1.00
//  Project           : CECS_M4 2016
//  Create Date       : 2016-11-30
//  File              : M4_IO.C
////////////////////////////////////////////////////////////////////////////////
#include "m4_evkit_define.h"
#include "m4_cecs_io_base.h"

u16 Init_IOPort( void )
{
  GPIO_InitTypeDef          GPIO_InitStructure;
  TIM_ICInitTypeDef         TIM_ICInitStructure;
  NVIC_InitTypeDef          NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef   timer;

  //////////////////////////////////////////////////////////////////////////////
  // config can_id input pin
  GPIOE->ODR &= ~DEF_CANID_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_Init( GPIOE, &GPIO_InitStructure );
  
  // config board_type input pin
  GPIOG->ODR &= ~DEF_BOARD_TYPE_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_Init( GPIOG, &GPIO_InitStructure );

  // config buzzer output pin
  OffBuzzer();
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3;
  GPIO_Init( GPIOG, &GPIO_InitStructure );

  // config test_switch input pin
  GPIOB->ODR |= GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_11;
  GPIO_Init( GPIOB, &GPIO_InitStructure );
  
  // config test_led output pin
  OffTestLED();
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_12;
  GPIO_Init( GPIOB, &GPIO_InitStructure );

  //////////////////////////////////////////////////////////////////////////////
  // config timer3_ch2
 	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );
  GPIOB->ODR &= ~GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_5;
  GPIO_Init( GPIOB, &GPIO_InitStructure );
  GPIO_PinAFConfig( GPIOB, GPIO_PinSource5, GPIO_AF_TIM3 );

  TIM_TimeBaseStructInit ( &timer );
  timer.TIM_Prescaler = (u16)((SYSTEM_CORE_CLOCK>>1) / 10000uL) - 1;  // 10Khz
  timer.TIM_Period    = 40000;                                        // 4sec  
  TIM_TimeBaseInit( TIM3, &timer );

  TIM_ICInitStructure.TIM_Channel      = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity   = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection  = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler  = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter     = 0x08;
  TIM_ICInit( TIM3, &TIM_ICInitStructure );

  NVIC_InitStructure.NVIC_IRQChannel                    = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_Init( &NVIC_InitStructure );
  TIM_ITConfig( TIM3, TIM_IT_Update | TIM_IT_CC2, ENABLE );

  //////////////////////////////////////////////////////////////////////////////
  // config timer4_ch2
 	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE );
  GPIOB->ODR &= ~GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_7;
  GPIO_Init( GPIOB, &GPIO_InitStructure );
  GPIO_PinAFConfig( GPIOB, GPIO_PinSource7, GPIO_AF_TIM4 );

  TIM_TimeBaseStructInit ( &timer );
  timer.TIM_Prescaler = (u16)((SYSTEM_CORE_CLOCK>>1) / 10000uL) - 1;  // 10Khz
  timer.TIM_Period    = 40000;                                        // 4sec  
  TIM_TimeBaseInit( TIM4, &timer );

  TIM_ICInitStructure.TIM_Channel      = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity   = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection  = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler  = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter     = 0x08;
  TIM_ICInit( TIM4, &TIM_ICInitStructure );

  NVIC_InitStructure.NVIC_IRQChannel                    = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_Init( &NVIC_InitStructure );
  TIM_ITConfig( TIM4, TIM_IT_Update | TIM_IT_CC2, ENABLE );

  TIM_Cmd( TIM3, ENABLE );
  TIM_Cmd( TIM4, ENABLE );

  return GetBoardType();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
u16 nTIM3CaptureNumber  = 0;
u16 nIC3ReadValue1      = 0;
u16 nIC3ReadValue2      = 0;
u32 nTIM3CaptureCounter = 0uL;
u32 nTIM3CaptureFreq    = 0uL;
void ISR_TIM3_CaptureCounter( void )
{
  if( TIM3->SR & TIM_IT_Update ) {
    nTIM3CaptureNumber = 0;
    nIC3ReadValue2 = nIC3ReadValue1 = 0;
    nTIM3CaptureFreq = 0uL;
  } else {
    if( nTIM3CaptureNumber == 0 ) {
      TIM_SetCounter( TIM3, 0 );
      nIC3ReadValue1 = TIM_GetCapture2( TIM3 );
      nTIM3CaptureNumber = 1;
    } else {
      nIC3ReadValue2 = TIM_GetCapture2( TIM3 );
      if( nIC3ReadValue1 > nIC3ReadValue2 ) {
        nTIM3CaptureCounter = (u32)((65535 - nIC3ReadValue1) + nIC3ReadValue2);
      } else {
        nTIM3CaptureCounter = (u32)(nIC3ReadValue2 - nIC3ReadValue1);
      }
      
      // Frequency computation
      nTIM3CaptureFreq = (REF_CAPTURE_CLOCK * 10uL) / (u32)nTIM3CaptureCounter;
      nTIM3CaptureNumber = 0;
    }
  }
  
  TIM_ClearITPendingBit( TIM3, TIM_IT_CC2 | TIM_IT_Update );
}

u32 TIM3_GetCaptureCounter( void ) { return nTIM3CaptureCounter; }
u32 TIM3_GetCaptureCFreq( void )   { return nTIM3CaptureFreq; }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
u16 nTIM4CaptureNumber  = 0;
u16 nIC4ReadValue1      = 0;
u16 nIC4ReadValue2      = 0;
u32 nTIM4CaptureCounter = 0uL;
u32 nTIM4CaptureFreq    = 0uL;
void ISR_TIM4_CaptureCounter( void )
{
  if( TIM4->SR & TIM_IT_Update ) {
    nTIM4CaptureNumber = 0;
    nIC4ReadValue2 = nIC4ReadValue1 = 0;
    nTIM4CaptureFreq = 0uL;
  } else {
    if( nTIM4CaptureNumber == 0 ) {
      TIM_SetCounter( TIM4, 0 );
      nIC4ReadValue1 = TIM_GetCapture2( TIM4 );
      nTIM4CaptureNumber = 1;
    } else {
      nIC4ReadValue2 = TIM_GetCapture2( TIM4 ); 
      if( nIC4ReadValue1 > nIC4ReadValue2 ) {
        nTIM4CaptureCounter = (u32)((65535 - nIC4ReadValue1) + nIC4ReadValue2);
      } else {
        nTIM4CaptureCounter = (u32)(nIC4ReadValue2 - nIC4ReadValue1);
      }
      
      // Frequency computation
      nTIM4CaptureFreq = (REF_CAPTURE_CLOCK * 10uL) / (u32)nTIM4CaptureCounter;
      nTIM4CaptureNumber = 0;
    }
  }
  
  TIM_ClearITPendingBit( TIM4, TIM_IT_CC2 | TIM_IT_Update );
}

u32 TIM4_GetCaptureCounter( void ) { return nTIM4CaptureCounter; }
u32 TIM4_GetCaptureCFreq( void )   { return nTIM4CaptureFreq; }
