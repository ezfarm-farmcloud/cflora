////////////////////////////////////////////////////////////////////////////////
//  Company           : 
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Revision          : 1.00
//  Project           : EVKIT 2014
//  Editor            : Hyunju Lee
//  Create Date       : 2014-05-30
//  File              : EVKIT_ADC.C
//  *** timer_5 used
//  *** DMA1_Channel1 used
////////////////////////////////////////////////////////////////////////////////
#include  "def_application.h"

u16           tagAnalogInputValue[DEF_CHANNEL_COUNT];
ADC_DUAL_DATA nAData[DEF_ADC_DMA_COUNT];
static  u8    nADChannelIndex = ADC_Channel_0;

////////////////////////////////////////////////////////////////////////////////
// init AtoC
void ConfigADC( void )
{
  GPIO_InitTypeDef					GPIO_InitStructure;
  ADC_InitTypeDef  					ADC_InitStructure;
	DMA_InitTypeDef						DMA_InitStructure;
  TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
  NVIC_InitTypeDef					NVIC_InitStructure;

	// analog input port : 8channels
  GPIO_InitStructure.GPIO_Pin		= 0x00FF;   // PA_0~7
  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AIN;
  GPIO_Init( GPIOA, &GPIO_InitStructure );

  RCC_ADCCLKConfig( RCC_PCLK2_Div8 );   // ADC Clock 64/8 = 8MHz
  ADC_InitStructure.ADC_Mode                = ADC_Mode_RegSimult;
  ADC_InitStructure.ADC_ScanConvMode        = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode  = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv    = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign           = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel        = 1;
	
  // ADC1 Configuration & Calibration
	// ADC_SampleTime_239Cycles5 >> 1.5 + 239.5 = 241cycles(8MHz >> 30.125usec)
  ADC_DeInit( ADC1 );
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE );
  ADC_Init( ADC1, &ADC_InitStructure );
  ADC_ITConfig( ADC1, ADC_IT_EOC, ENABLE );
  ADC_Cmd( ADC1, ENABLE );

  // ADC2 Configuration & Calibration
	// ADC_SampleTime_239Cycles5 >> 1.5 + 239.5 = 241cycles(8MHz >> 30.125usec)
  ADC_DeInit( ADC2 );
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC2, ENABLE );
  ADC_Init( ADC2, &ADC_InitStructure );
  ADC_ExternalTrigConvCmd(ADC2, ENABLE);
  ADC_Cmd( ADC2, ENABLE );

  // ADC1 Channel_0 Setting & Calibration
  ADC_RegularChannelConfig( ADC1, nADChannelIndex, 1, ADC_SampleTime_239Cycles5 );
	ADC_ResetCalibration( ADC1 );
	while( ADC_GetResetCalibrationStatus(ADC1) );
  ADC_StartCalibration( ADC1 );
  while( ADC_GetCalibrationStatus(ADC1) );

  // ADC2 Channel_0 Setting & Calibration
  ADC_RegularChannelConfig( ADC2, nADChannelIndex, 1, ADC_SampleTime_239Cycles5 );
	ADC_ResetCalibration( ADC2 );
	while( ADC_GetResetCalibrationStatus(ADC2) );
  ADC_StartCalibration( ADC2 );
  while( ADC_GetCalibrationStatus(ADC2) );

 	// TIM5 initialize. for ADC
	// PCLK1 __________ 36Mhz[ If(APB1_prescaler=1) x1 else x2 ]
  // RCC_PCLK1Config( RCC_HCLK_Div2 ); >> APB1_prescaler=2
	// TIMx Clock _____ (PCLK1 x 2) / TIM_CKD_DIV1 = 72Mhz
  // 8channel/2 * 10sps * 10 = 400sps
  // 72,000,000 / 400 = 180,000
	// prescaler  _____ 180,000 / (1799+1) = 100
	// period _________ nADC_TICK_TIME = 100
	TIM_TimeBaseStructInit( &TIM_TimeBaseStructure );
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM5, ENABLE );
	TIM_DeInit( TIM5 );
	TIM_TimeBaseStructure.TIM_Period        = nADC_TICK_TIME;	  // Autoreload value ARR set
  TIM_TimeBaseStructure.TIM_Prescaler     = 1799; 	            // Prescaler value PSC set
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseInit( TIM5, &TIM_TimeBaseStructure );
  TIM_ClearITPendingBit( TIM5, TIM_IT_Update );

  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannel                    = TIM5_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 7;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
  NVIC_Init( &NVIC_InitStructure );

  // DMA1 channel1 configuration
  // DEF_ADC_DMA_COUNT = 8
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  DMA_DeInit( DMA1_Channel1 );
  DMA_InitStructure.DMA_PeripheralBaseAddr	= ADC1_DR_ADDR;
  DMA_InitStructure.DMA_MemoryBaseAddr			= (u32)nAData;        // adc data buffer base address
  DMA_InitStructure.DMA_DIR									= DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize					= DEF_ADC_DMA_COUNT;  // adc sample count
  DMA_InitStructure.DMA_PeripheralInc				= DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc						= DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize	= DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize			= DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode								= DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority						= DMA_Priority_High;
  DMA_InitStructure.DMA_M2M									= DMA_M2M_Disable;
  DMA_Init( DMA1_Channel1, &DMA_InitStructure );
  DMA_Cmd( DMA1_Channel1, ENABLE );
  DMA_ITConfig( DMA1_Channel1, DMA_IT_TC, ENABLE );
  ADC_DMACmd( ADC1, ENABLE );
  
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannel										= DMA1_Channel1_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
  NVIC_Init( &NVIC_InitStructure );

  memset( tagAnalogInputValue, 0, sizeof(u16)*DEF_CHANNEL_COUNT );
}

////////////////////////////////////////////////////////////////////////////////
// timer_3 function & ISR
void ADCTimerStart( u8 nChannel )
{
  if( nChannel >= 4 ) { return; }
  
  DMA1_Channel1->CNDTR  = 0;
  memset( nAData, 0, sizeof(ADC_DUAL_DATA)*DEF_ADC_DMA_COUNT );

  ADC_RegularChannelConfig( ADC1, nChannel,   1, ADC_SampleTime_239Cycles5 );
  ADC_RegularChannelConfig( ADC2, nChannel+4, 1, ADC_SampleTime_239Cycles5 );

	TIM_SetCounter( TIM5, 0 );
  TIM_ClearITPendingBit( TIM5, TIM_IT_Update );
	TIM_ITConfig( TIM5, TIM_IT_Update, ENABLE );
	TIM_Cmd( TIM5, ENABLE );
}

void ADCTimerStop( void )
{
	TIM_Cmd( TIM5, DISABLE );
	TIM_ITConfig( TIM5, TIM_IT_Update, DISABLE );
  TIM_ClearITPendingBit( TIM5, TIM_IT_Update );
}

void ISRTIM5forADC( void )
{ // TIM5_IRQChannel
  TIM_SetCounter( TIM5, 0 );
  ADC_SoftwareStartConvCmd( ADC1, ENABLE );

  TIM_ClearITPendingBit( TIM5, TIM_IT_Update );
}

////////////////////////////////////////////////////////////////////////////////
// DMA1_Channel1 ISR
void ISRADCDMA1Channel1( void )     // DMA1_Channel1_IRQChannel
{
  u32 ulDataL = 0uL;
  u32 ulDataH = 0uL;

  ADCTimerStop();
  
	// dma_adc_data averaging
	for( u16 i=0; i<DEF_ADC_DMA_COUNT; i++ ) {
		ulDataL += (u32)nAData[i].nADC_DR1;
		ulDataH += (u32)nAData[i].nADC_DR2;
	}

  // voltage caliblation
	ulDataL /= DEF_ADC_DMA_COUNT;
  ulDataL *= ADC_VREFx2;
  ulDataL /= ADC_12b_MAX;

	ulDataH /= DEF_ADC_DMA_COUNT;
  ulDataH *= ADC_VREFx2;
  ulDataH /= ADC_12b_MAX;

  if( nADChannelIndex < 4 ) {
    tagAnalogInputValue[nADChannelIndex]   = (u16)ulDataL;
    tagAnalogInputValue[nADChannelIndex+4] = (u16)ulDataH;
    if( ++nADChannelIndex >= 4 ) { nADChannelIndex = ADC_Channel_0; }
  } else {
    nADChannelIndex = ADC_Channel_0;
  }
  ADCTimerStart( nADChannelIndex );

	DMA_ClearITPendingBit( DMA1_IT_GL1 | DMA1_IT_TC1 );
}
