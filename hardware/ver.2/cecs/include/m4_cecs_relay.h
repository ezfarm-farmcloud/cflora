////////////////////////////////////////////////////////////////////////////////
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Microcontroller		: Cortex M4 STM32F407ZET6
//  Revision          : 1.00
//  Project           : CECS_M4 2016
//  Create Date       : 2016-11-30
//  File              : M4_CECS_RELAY.H
////////////////////////////////////////////////////////////////////////////////
#ifndef __M4_CECS_RELAY_H__
#define __M4_CECS_RELAY_H__


extern void InitRelayModule( void );
extern void ProcedureRelayModule( u16 nTypeID );

extern void SetMaskRelayOutput  ( u16 setmask );
extern void ResetMaskRelayOutput( u16 rstmask );
extern void SetRelay( u16 nPort, bool b );
extern bool CheckRelay( u16 nPort );

#endif // __M4_CECS_RELAY_H__
