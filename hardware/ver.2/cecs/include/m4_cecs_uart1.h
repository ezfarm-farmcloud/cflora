////////////////////////////////////////////////////////////////////////////////
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Microcontroller		: Cortex M4 STM32F407ZET6
//  Revision          : V1.00
//  Project           : CECS_M4 2016
//  Create Date       : 2016-11-30
//  File              : M4_CECS_UART1.H
////////////////////////////////////////////////////////////////////////////////
#ifndef __M4_CECS_UART1_H__
#define __M4_CECS_UART1_H__

extern void Init_UART1( u32 baudrate );
extern void ISR_USART1            ( void );

extern void	USART1_SendPacket     ( LPCOMMPACKET pPacket, bool bAck );
extern u16 	USART1_Write          ( u8 *buffer, u16 buffersize );
extern bool	USART1_TxQuePush      ( u8 data );
extern bool USART1_TxQuePop       ( u8 *data );

extern bool	USART1_PacketStreaming( LPCOMMPACKET pPacket );
extern bool	USART1_RxQuePush      ( u8 data );
extern bool USART1_RxQuePop       ( u8 *data );

#endif // __M4_CECS_UART1_H__

