////////////////////////////////////////////////////////////////////////////////
//  Company           : 
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Revision          : 1.00
//  Project           : EVKIT 2014
//  Editor            : Hyunju Lee
//  Create Date       : 2014-05-30
//  File              : EVKIT_COUNTER3_4.H
////////////////////////////////////////////////////////////////////////////////
#ifndef __EVKIT_COUNTER3_4__
#define __EVKIT_COUNTER3_4__

// TIM3...COUNTER_3
extern void TIM3Config_CaptureCounter( void );
extern void TIM3ISR_CaptureCounter( void );
extern u32  TIM3GetCaptureCounter( void );
extern u32  TIM3GetCaptureCFreq( void );

// TIM4...COUNTER_4
extern void TIM4Config_CaptureCounter( void );
extern void TIM4ISR_CaptureCounter( void );
extern u32  TIM4GetCaptureCounter( void );
extern u32  TIM4GetCaptureCFreq( void );

#endif // __EVKIT_COUNTER3_4__
