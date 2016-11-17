////////////////////////////////////////////////////////////////////////////////
//  Company           : 
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Revision          : 1.00
//  Project           : EVKIT 2014
//  Editor            : Hyunju Lee
//  Create Date       : 2014-05-30
//  File              : EVKIT_COUNTER3_4.C
//  *** using timer_3/4 external counter
////////////////////////////////////////////////////////////////////////////////
#include  "def_application.h"

#define REF_CAPTURE_CLOCK   10000uL

////////////////////////////////////////////////////////////////////////////////
// TIM3
void TIM3Config_CaptureCounter( void )
{
  GPIO_InitTypeDef          GPIO_InitStructure;
  TIM_ICInitTypeDef         TIM_ICInitStructure;
  NVIC_InitTypeDef					NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef   timer;
  
  // TIM3 channel_2 pin(PB.05_remap) configuration
  // no_remap PA.07
  GPIO_PinRemapConfig( GPIO_PartialRemap_TIM3, ENABLE );
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
 	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );

  // base timer configuration
  TIM_TimeBaseStructInit ( &timer );
  timer.TIM_Prescaler = (7200 - 1);   // 10Khz
  timer.TIM_Period    = 40000;
  TIM_TimeBaseInit( TIM3, &timer );

  TIM_ICInitStructure.TIM_Channel      = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity   = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection  = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler  = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter     = 0x08;
  TIM_ICInit( TIM3, &TIM_ICInitStructure );
 
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannel                    = TIM3_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 7;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
  NVIC_Init( &NVIC_InitStructure );

  TIM_ITConfig( TIM3, TIM_IT_Update | TIM_IT_CC2, ENABLE );
  TIM_Cmd( TIM3, ENABLE );
}

u16 nTIM3CaptureNumber  = 0;
u16 nIC3ReadValue1      = 0;
u16 nIC3ReadValue2      = 0;
u32 nTIM3CaptureCounter = 0uL;
u32 nTIM3CaptureFreq    = 0uL;
void TIM3ISR_CaptureCounter( void )
{
  if( TIM3->SR & TIM_IT_Update ) {
    nTIM3CaptureNumber = 0;
    nIC3ReadValue2 = nIC3ReadValue1 = 0;
    nTIM3CaptureFreq = 0uL;
  } else {
    if( nTIM3CaptureNumber == 0 ) {
      //nIC3ReadValue1 = TIM_GetCapture2( TIM3 );
      TIM_SetCounter( TIM3, 0 );
      nIC3ReadValue1 = 0;
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

u32 TIM3GetCaptureCounter( void ) { return nTIM3CaptureCounter; }
u32 TIM3GetCaptureCFreq( void )   { return nTIM3CaptureFreq; }

////////////////////////////////////////////////////////////////////////////////
// TIM4
void TIM4Config_CaptureCounter( void )
{
  GPIO_InitTypeDef          GPIO_InitStructure;
  TIM_ICInitTypeDef         TIM_ICInitStructure;
  NVIC_InitTypeDef					NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef   timer;
  
  // TIM4 channel_2 pin(PB.07) configuration
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
 	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE );

  // base timer configuration
  TIM_TimeBaseStructInit ( &timer );
  timer.TIM_Prescaler = (7200 - 1);   // 10Khz
  timer.TIM_Period    = 40000;
  TIM_TimeBaseInit( TIM4, &timer );

  TIM_ICInitStructure.TIM_Channel      = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity   = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection  = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler  = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter     = 0x08;
  TIM_ICInit( TIM4, &TIM_ICInitStructure );
 
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannel                    = TIM4_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 7;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
  NVIC_Init( &NVIC_InitStructure );

  TIM_ITConfig( TIM4, TIM_IT_CC2 | TIM_IT_Update, ENABLE );
  TIM_Cmd( TIM4, ENABLE );
}

u16 nTIM4CaptureNumber  = 0;
u16 nIC4ReadValue1      = 0;
u16 nIC4ReadValue2      = 0;
u32 nTIM4CaptureCounter = 0uL;
u32 nTIM4CaptureFreq    = 0uL;
void TIM4ISR_CaptureCounter( void )
{
  if( TIM4->SR & TIM_IT_Update ) {
    nTIM4CaptureNumber = 0;
    nIC4ReadValue2 = nIC4ReadValue1 = 0;
    nTIM4CaptureFreq = 0uL;
  } else {
    if( nTIM4CaptureNumber == 0 ) {
      //nIC4ReadValue1 = TIM_GetCapture2( TIM4 );
      TIM_SetCounter( TIM4, 0 );
      nIC4ReadValue1 = 0;
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

u32 TIM4GetCaptureCounter( void ) { return nTIM4CaptureCounter; }
u32 TIM4GetCaptureCFreq( void )   { return nTIM4CaptureFreq; }
