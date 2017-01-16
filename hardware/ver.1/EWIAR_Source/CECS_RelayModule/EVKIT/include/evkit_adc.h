////////////////////////////////////////////////////////////////////////////////
//  Company           : 
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Revision          : 1.00
//  Project           : EVKIT 2014
//  Editor            : Hyunju Lee
//  Create Date       : 2014-05-30
//  File              : EVKIT_ADC.H
////////////////////////////////////////////////////////////////////////////////
#ifndef __EVKIT_ADC_H__
#define __EVKIT_ADC_H__

#define   ADC1_DR_ADDR			    ((u32)0x4001244C)

#define 	ADC_12b_BIT_CNT		    12
#define 	ADC_12b_MAX				    4095
#define 	ADC_VREF					    2500    // 2.5V referance
#define 	ADC_VREFx2				    5000

////////////////////////////////////////////////////////////////////////////////
// 10sps/channel x 8 x 10samples/sps = 800sps
#define 	DEF_CHANNEL_COUNT     8
#define 	DEF_ADC_DMA_COUNT     10

#define   nADC_TICK_TIME        100     // 1.25usec * 100 = 125usec

extern u16  tagAnalogInputValue[DEF_CHANNEL_COUNT];

typedef union __tagADC_DUAL_DATA__ {
	struct {
		u16	nADC_DR1;
		u16	nADC_DR2;
	};
	u32	dwADC_DR12;
} ADC_DUAL_DATA, *LPADC_DUAL_DATA;

extern void ConfigADC( void );
extern void ADCTimerStart( u8 nChannel );
extern void ADCTimerStop( void );

extern void ISRTIM5forADC( void );
extern void ISRADCDMA1Channel1( void );

#endif // __EVKIT_ADC_H__
