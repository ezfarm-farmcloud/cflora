////////////////////////////////////////////////////////////////////////////////
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Microcontroller		: Cortex M4 STM32F407ZET6
//  Revision          : V1.00
//  Project           : CECS_M4 2016
//  Create Date       : 2016-11-30
//  File              : M4_CECS_ANALOG.C
////////////////////////////////////////////////////////////////////////////////
#include "m4_evkit_define.h"

#include "m4_cecs_io_base.h"
#include "m4_cecs_uart1.h"
#include "m4_cecs_uart3.h"
#include "m4_cecs_uart6.h"
#include "m4_cecs_can.h"

#include "m4_cecs_analog.h"

const u8 nADCChannel[DEF_ADC_CHANNEL_CNT] = {
  ADC_Channel_4,
  ADC_Channel_5,
  ADC_Channel_6,
  ADC_Channel_7,
  ADC_Channel_4,
  ADC_Channel_5,
  ADC_Channel_6,
  ADC_Channel_7,
  ADC_Channel_8,
  ADC_Channel_9,
  ADC_Channel_14,
  ADC_Channel_15,
  ADC_Channel_8,
  ADC_Channel_9,
  ADC_Channel_14,
  ADC_Channel_15,
};

u16 nADCConvertedValue[DEF_ADC_CHANNEL_CNT];
u16 nTmpADCConvertedValue[DEF_ADC_BUFFER_CNT];
u16 nADCChannelCounter  = 0;
u16 nADCBufferCounter   = 0;

void InitAnalogModule( void )
{
  GPIO_InitTypeDef          GPIO_InitStructure;
  ADC_CommonInitTypeDef			ADC_CommonInitStructure;
  ADC_InitTypeDef           ADC_InitStructure;
  TIM_TimeBaseInitTypeDef   timer;
  NVIC_InitTypeDef          NVIC_InitStructure;

  GPIOF->ODR &= ~DEF_CHANNEL_SELECT_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL ;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init( GPIOF, &GPIO_InitStructure );

  // ADC input_channel initialize
	// PF03 : ADC3_channel9
	// PF04 : ADC3_channel14
	// PF05 : ADC3_channel15
	// PF06 : ADC3_channel4
	// PF07 : ADC3_channel5
	// PF08 : ADC3_channel6
	// PF09 : ADC3_channel7
	// PF10 : ADC3_channel8
  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6
                                | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_Init( GPIOF, &GPIO_InitStructure );

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC3, ENABLE );
  ADC_CommonInitStructure.ADC_Mode							= ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler					= ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode			= ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay	= ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit( &ADC_CommonInitStructure );

  ADC_InitStructure.ADC_Resolution            = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode          = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode    = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge  = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign             = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion       = 1;
  ADC_Init( ADC3, &ADC_InitStructure );

  NVIC_InitStructure.NVIC_IRQChannel										= ADC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority					= 5;
  NVIC_InitStructure.NVIC_IRQChannelCmd									= ENABLE;
  NVIC_Init( &NVIC_InitStructure );     

	// Timer_7 Initialize for AtoD converter 
	// TIM7CLK = 2 * PCLK1  
  // PCLK1 = HCLK / 4 => TIM4CLK = HCLK / 2 = SYSTEM_CORE_CLOCK / 2
	// tick_time : 1msec
	TIM_TimeBaseStructInit( &timer );
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM7, ENABLE );
	TIM_DeInit( TIM7 );
	timer.TIM_Period        = 78;
  timer.TIM_Prescaler     = (u16)((SYSTEM_CORE_CLOCK>>1) / 10000uL) - 1;
  timer.TIM_ClockDivision = TIM_CKD_DIV1;
  timer.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseInit( TIM7, &timer );
  TIM_ClearITPendingBit( TIM7, TIM_IT_Update );

  NVIC_InitStructure.NVIC_IRQChannel                    = TIM7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority					= 6;
  NVIC_InitStructure.NVIC_IRQChannelCmd									= ENABLE;
  NVIC_Init( &NVIC_InitStructure );
  
  ADC_ITConfig(ADC3,ADC_IT_EOC,ENABLE ) ; 
  ADC_Cmd( ADC3, ENABLE );
}

void TIM7_StartAtoDConverter( void )
{
  u16 tmp;

  nADCChannelCounter = 0;
  memset( nADCConvertedValue, 0, sizeof(u16)*DEF_ADC_CHANNEL_CNT );
  nADCBufferCounter  = 0;
  memset( nTmpADCConvertedValue, 0, sizeof(u16)*DEF_ADC_BUFFER_CNT );

  tmp  = GPIOF->ODR;
  tmp &= ~DEF_CHANNEL_SELECT_MASK;
  tmp |= DEF_CHANNEL_SELECT_1ST;
  GPIOF->ODR = tmp;
  ADC_RegularChannelConfig( ADC3, nADCChannel[nADCChannelCounter], 1, ADC_SampleTime_3Cycles );

	TIM7->ARR	 = 78;       // 7.8msec__~128sps
	TIM_SetCounter( TIM7, 0 );
  TIM_ClearITPendingBit( TIM7, TIM_IT_Update );
	TIM_ITConfig( TIM7, TIM_IT_Update, ENABLE );
	TIM_Cmd( TIM7, ENABLE );
}

void TIM7_StopAtoDConverter( void )
{
	TIM_Cmd( TIM7, DISABLE );
	TIM_ITConfig( TIM7, TIM_IT_Update, DISABLE );
  TIM_ClearITPendingBit( TIM7, TIM_IT_Update );

  GPIOF->ODR &= ~DEF_CHANNEL_SELECT_MASK;
}

void ISR_TIM7forAtoDConverter( void )
{
  ADC_SoftwareStartConv( ADC3 );
  
  TIM_ClearITPendingBit( TIM7, TIM_IT_Update );
}

void ISR_AtoDConverter( void )
{
  u16 tmp;
  
  
  for( u16 i=0; i<(DEF_ADC_BUFFER_CNT-1); i++ ) {
    nTmpADCConvertedValue[i] = nTmpADCConvertedValue[i+1];
  }
  nTmpADCConvertedValue[DEF_ADC_BUFFER_CNT-1] = ADC3->DR;

  nADCBufferCounter++;
  nADCBufferCounter %= DEF_ADC_BUFFER_CNT;
  
  if( nADCBufferCounter == 0 ) {
    u32 adTEMP ;
    adTEMP = (u32)((nTmpADCConvertedValue[1] + nTmpADCConvertedValue[2]) >> 1);
    adTEMP *= 5000 ;
    adTEMP /= 4095 ;
    
    nADCConvertedValue[nADCChannelCounter] =  (u16)adTEMP;
    nADCChannelCounter++;
    nADCChannelCounter %= DEF_ADC_CHANNEL_CNT;
  }
  
  tmp  = GPIOF->ODR;
  tmp &= ~DEF_CHANNEL_SELECT_MASK;
  if( nADCChannelCounter < 4 ) {
    tmp |= DEF_CHANNEL_SELECT_1ST;
  } else if( (nADCChannelCounter >= 4) && (nADCChannelCounter < 8) ) {
    tmp |= DEF_CHANNEL_SELECT_2ND;
  } else if( (nADCChannelCounter >= 8) && (nADCChannelCounter < 12) ) {
    tmp |= DEF_CHANNEL_SELECT_1ST;
  } else {
    tmp |= DEF_CHANNEL_SELECT_2ND;
  }
  GPIOF->ODR = tmp;
  ADC_RegularChannelConfig( ADC3, nADCChannel[nADCChannelCounter], 1, ADC_SampleTime_3Cycles );

  ADC_ClearFlag( ADC3, ADC_FLAG_EOC );
}

void ProcedureAnalogModule( u16 nTypeID )
{
  OnBuzzer();   Delay( 100 );
  OffBuzzer();  Delay( 400 );

  InitAnalogModule();
  TIM7_StartAtoDConverter();

	while( 1 ) {
    /*      
    if( USART1_PacketStreaming( &RxPacket ) ) {
      if( (RxPacket.wCanID & 0x00ff) == nTypeID ) {
        switch( RxPacket.wCanID & 0xff00 ) {
          case tagCAN_GET_COUNTER :
            TxPacket.wCanID    = RxPacket.wCanID;
            TxPacket.dwData[0] = TIM3_GetCaptureCFreq();
            TxPacket.dwData[1] = TIM4_GetCaptureCFreq();
            USART1_SendPacket( &TxPacket, true );
            break;
            
          case tagCAN_GET_ATOD_1ST :
            TxPacket.wCanID   = RxPacket.wCanID;
            TxPacket.wData[0] = 10;//nADCConvertedValue[0];
            TxPacket.wData[1] = 20;//nADCConvertedValue[1];
            TxPacket.wData[2] = 30;//nADCConvertedValue[2];
            TxPacket.wData[3] = 40;//nADCConvertedValue[3];
            USART1_SendPacket( &TxPacket, true );
            break;

          case tagCAN_GET_ATOD_2ND :
            TxPacket.wCanID   = RxPacket.wCanID;
            TxPacket.wData[0] = 50;//nADCConvertedValue[4];
            TxPacket.wData[1] = 60;//nADCConvertedValue[5];
            TxPacket.wData[2] = 70;//nADCConvertedValue[6];
            TxPacket.wData[3] = 80;//ADCConvertedValue[7];
            USART1_SendPacket( &TxPacket, true );
            break;

          case tagCAN_GET_ATOD_3RD :
            TxPacket.wCanID   = RxPacket.wCanID;
            TxPacket.wData[0] = 90;//nADCConvertedValue[8];
            TxPacket.wData[1] = 100;//nADCConvertedValue[9];
            TxPacket.wData[2] = 110;//nADCConvertedValue[10];
            TxPacket.wData[3] = 120;//nADCConvertedValue[11];
            USART1_SendPacket( &TxPacket, true );
            break;

          case tagCAN_GET_ATOD_4TH :
            TxPacket.wCanID   = RxPacket.wCanID;
            TxPacket.wData[0] = 130;//nADCConvertedValue[12];
            TxPacket.wData[1] = 140;//nADCConvertedValue[13];
            TxPacket.wData[2] = 150;//nADCConvertedValue[14];
            TxPacket.wData[3] = 160;//nADCConvertedValue[15];
            USART1_SendPacket( &TxPacket, true );
            break;

          case tagCAN_ID_SCANNING :
            TxPacket.wCanID  = RxPacket.wCanID;
            USART1_SendPacket( &TxPacket, true );
            break;
            
          default :
            TxPacket.wCanID  = RxPacket.wCanID;
            USART1_SendPacket( &TxPacket, false );
            break;
        }
      } else {
        if( CANTrasmitMessage( &RxPacket ) ) {
          if( GetMessage((LPMESSAGE)&TxPacket) ) {
            if(  (TxPacket.nHeader==ACK_PACKET_HEADER)
              && (TxPacket.wCanID==RxPacket.wCanID) ) {
              USART1_SendPacket( &TxPacket, true );
            }
          }
        } else {
          TxPacket.wCanID = RxPacket.wCanID;
          USART1_SendPacket( &TxPacket, false );
        }
      }
    }
*/
    if( GetMessage((LPMESSAGE)&RxPacket) ) {
      if( (RxPacket.wCanID & 0x00ff) == nTypeID ) {
        switch( RxPacket.wCanID & 0x0f00 ) {
          case tagCAN_GET_COUNTER :
            TxPacket.wCanID    = RxPacket.wCanID;
            TxPacket.dwData[0] = TIM3_GetCaptureCFreq();
            TxPacket.dwData[1] = TIM4_GetCaptureCFreq();
            CANTrasmitMessage( &TxPacket );
            break;
            
          case tagCAN_GET_ATOD_1ST :
            TxPacket.wCanID   = RxPacket.wCanID;
            TxPacket.wData[0] = nADCConvertedValue[0];
            TxPacket.wData[1] = nADCConvertedValue[1];
            TxPacket.wData[2] = nADCConvertedValue[2];
            TxPacket.wData[3] = nADCConvertedValue[3];
            CANTrasmitMessage( &TxPacket );
            break;

          case tagCAN_GET_ATOD_2ND :
            TxPacket.wCanID   = RxPacket.wCanID;
            TxPacket.wData[0] = nADCConvertedValue[4];
            TxPacket.wData[1] = nADCConvertedValue[5];
            TxPacket.wData[2] = nADCConvertedValue[6];
            TxPacket.wData[3] = nADCConvertedValue[7];
            CANTrasmitMessage( &TxPacket );
            break;

          case tagCAN_GET_ATOD_3RD :
            TxPacket.wCanID   = RxPacket.wCanID;
            TxPacket.wData[0] = nADCConvertedValue[8];
            TxPacket.wData[1] = nADCConvertedValue[9];
            TxPacket.wData[2] = nADCConvertedValue[10];
            TxPacket.wData[3] = nADCConvertedValue[11];
            CANTrasmitMessage( &TxPacket );
            break;

          case tagCAN_GET_ATOD_4TH :
            TxPacket.wCanID   = RxPacket.wCanID;
            TxPacket.wData[0] = nADCConvertedValue[12];
            TxPacket.wData[1] = nADCConvertedValue[13];
            TxPacket.wData[2] = nADCConvertedValue[14];
            TxPacket.wData[3] = nADCConvertedValue[15];
            CANTrasmitMessage( &TxPacket );
            break;

          case tagCAN_ID_SCANNING :
            TxPacket.wCanID  = RxPacket.wCanID;
            CANTrasmitMessage( &TxPacket );
            break;
            
          default :   break;
        }
      }
    }
  }
}
