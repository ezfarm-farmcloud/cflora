////////////////////////////////////////////////////////////////////////////////
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Microcontroller		: Cortex M4 STM32F407ZET6
//  Revision          : 1.00
//  Project           : CECS_M4 2016
//  Create Date       : 2016-11-30
//  File              : M4_CECS_ANALOG.H
////////////////////////////////////////////////////////////////////////////////
#ifndef __M4_CECS_ANALOG_H__
#define __M4_CECS_ANALOG_H__

#define   DEF_CHANNEL_SELECT_MASK   0xf000
#define   DEF_CHANNEL_SELECT_1ST    0x5000
#define   DEF_CHANNEL_SELECT_2ND    0xa000

#define   DEF_ADC_CHANNEL_CNT       16
#define   DEF_ADC_BUFFER_CNT        4

extern void InitAnalogModule( void );
extern void ProcedureAnalogModule( u16 nTypeID );

extern void TIM7_StartAtoDConverter( void );
extern void TIM7_StopAtoDConverter( void );
extern void ISR_TIM7forAtoDConverter( void );
extern void ISR_AtoDConverter( void );

#endif // __M4_CECS_ANALOG_H__
