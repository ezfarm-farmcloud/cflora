////////////////////////////////////////////////////////////////////////////////
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Microcontroller		: Cortex M4 STM32F407ZET6
//  Revision          : 1.00
//  Project           : CECS_M4 2016
//  Create Date       : 2016-11-30
//  File              : M4_CECS_IO.H
////////////////////////////////////////////////////////////////////////////////
#ifndef __M4_CECS_IO_H__
#define __M4_CECS_IO_H__

///////////////////////////////////////////////////////////////////////////////
// i/o function define
#define     TYPE_MODULE_BASE     0x0000
#define     TYPE_MODULE_ANALOG   0x0010
#define     TYPE_MODULE_RELAY    0x0020
#define     TYPE_MODULE_ISOIN    0x0030

extern u16  Init_IOPort( void );

#define DEF_CANID_MASK        (u16)0x0f00
#define GetBoardID()          ((~GPIOE->IDR & DEF_CANID_MASK)>>8)

#define DEF_BOARD_TYPE_MASK   (u16)0x0007
#define GetBoardType()        (GPIOG->IDR & DEF_BOARD_TYPE_MASK)

#define OnBuzzer()            (GPIOG->ODR |= GPIO_Pin_3)
#define OffBuzzer()           (GPIOG->ODR &= ~GPIO_Pin_3)

#define CheckTestSwitch()     (GPIOB->IDR &= GPIO_Pin_11)

#define OnTestLED()           (GPIOB->ODR &= ~GPIO_Pin_12)
#define OffTestLED()          (GPIOB->ODR |= GPIO_Pin_12)
#define ToggleTestLED()       (GPIOB->ODR ^= GPIO_Pin_12)

#define REF_CAPTURE_CLOCK     10000uL
extern void ISR_TIM3_CaptureCounter( void );
extern u32 TIM3_GetCaptureCounter( void );
extern u32 TIM3_GetCaptureCFreq( void );

extern void ISR_TIM4_CaptureCounter( void );
extern u32 TIM4_GetCaptureCounter( void );
extern u32 TIM4_GetCaptureCFreq( void );

#endif // __M4_CECS_IO_H__
