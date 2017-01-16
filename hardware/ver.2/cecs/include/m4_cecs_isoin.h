////////////////////////////////////////////////////////////////////////////////
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Microcontroller		: Cortex M4 STM32F407ZET6
//  Revision          : 1.00
//  Project           : CECS_M4 2016
//  Create Date       : 2016-11-30
//  File              : M4_CECS_ISOIN.H
////////////////////////////////////////////////////////////////////////////////
#ifndef __M4_CECS_ISOIN_H__
#define __M4_CECS_ISOIN_H__

extern void InitISOINModule( void );
extern void ProcedureISOINModule( u16 nTypeID );
extern bool CheckInput( u16 nPort );

#endif // __M4_CECS_ISOIN_H__
