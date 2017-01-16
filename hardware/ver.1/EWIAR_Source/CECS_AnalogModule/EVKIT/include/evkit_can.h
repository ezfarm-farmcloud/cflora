////////////////////////////////////////////////////////////////////////////////
//  Company           : 
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Revision          : 1.00
//  Project           : EVKIT 2014
//  Editor            : Hyunju Lee
//  Create Date       : 2014-05-30
//  File              : EVKIT_CAN.H
////////////////////////////////////////////////////////////////////////////////
#ifndef __EVKIT_CAN_H__
#define __EVKIT_CAN_H__

////////////////////////////////////////////////////////////////////////////////
// CAN_ID function
#define     TYPE_BASEBOARD        0x0000
#define     TYPE_ANALOG_MOD       0x0010
#define     TYPE_RELAY_MOD        0x0020
#define     TYPE_ISO_INPUT_MOD    0x0030

#define     GetCanID()    (u32)((GPIOE->IDR & 0x000F) ^ 0x000F) // ~input 
extern u32  nAPPLICATION_CAN_ID;

extern u32  ConfigCanID( void );

extern void InitCanRxMessage( CanRxMsg *pRxMessage );
extern void ConfigCAN( void );
extern void ISRCAN( void );
extern BOOL CANTrasmitMessage( LPCOMMPACKET pPacket );

extern CanTxMsg CanTxMessage;
extern CanRxMsg CanRxMessage;

#endif // __EVKIT_CAN_H__
