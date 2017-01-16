////////////////////////////////////////////////////////////////////////////////
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Microcontroller		: Cortex M4 STM32F407ZET6
//  Revision          : 1.00
//  Project           : CECS_M4 2016
//  Create Date       : 2016-11-30
//  File              : M4_CECS_CAN.H
////////////////////////////////////////////////////////////////////////////////
#ifndef __M4_CECS_CAN_H__
#define __M4_CECS_CAN_H__

////////////////////////////////////////////////////////////////////////////////
// CAN_ID function
#define     TYPE_BASEBOARD        0x0000
#define     TYPE_ANALOG_MOD       0x0010
#define     TYPE_RELAY_MOD        0x0020
#define     TYPE_ISO_INPUT_MOD    0x0030

extern u32  nAPPLICATION_CAN_ID;

extern void Init_CAN1( void );
extern void ISR_CAN1_Reception( void );
extern bool CANTrasmitMessage( LPCOMMPACKET pPacket );

extern void StartCanTimeOut( u16 timeout );
extern void StopCANTimer( void );
extern void ISR_TIM2forCANTimeOut( void );

extern CanTxMsg CanTxMessage;
extern CanRxMsg CanRxMessage;

#endif // __M4_CECS_CAN_H__
