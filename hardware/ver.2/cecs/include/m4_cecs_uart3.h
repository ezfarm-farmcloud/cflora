////////////////////////////////////////////////////////////////////////////////
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Microcontroller		: Cortex M4 STM32F407ZET6
//  Revision          : V1.00
//  Project           : CECS_M4 2016
//  Create Date       : 2016-11-30
//  File              : M4_CECS_UART3.H
////////////////////////////////////////////////////////////////////////////////
#ifndef __M4_CECS_UART3_H__
#define __M4_CECS_UART3_H__

extern void Init_UART3( u32 baudrate );
extern void ISR_USART3            ( void );

extern void	USART3_SendPacket     ( LPCOMMPACKET pPacket, bool bAck );
extern u16 	USART3_Write          ( u8 *buffer, u16 buffersize );
extern bool	USART3_TxQuePush      ( u8 data );
extern bool USART3_TxQuePop       ( u8 *data );

extern bool	USART3_PacketStreaming( LPCOMMPACKET pPacket );
extern bool	USART3_RxQuePush      ( u8 data );
extern bool USART3_RxQuePop       ( u8 *data );

#endif // __M4_CECS_UART3_H__

