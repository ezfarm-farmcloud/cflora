////////////////////////////////////////////////////////////////////////////////
//  Company           : 
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Revision          : 1.00
//  Project           : EVKIT 2014
//  Editor            : Hyunju Lee
//  Create Date       : 2014-05-30
//  File              : EVKIT_UART1.H
////////////////////////////////////////////////////////////////////////////////
#ifndef __EVKIT_UART_1_H__
#define __EVKIT_UART_1_H__

// USART1 Function Define
extern void	USART1_Config         ( u32 nlbaudrate );
extern void USART1_ISR            ( void );

extern void	USART1_SendPacket     ( LPCOMMPACKET pPacket, BOOL bAck );
extern u16 	USART1_Write          ( u8 *buffer, u16 buffersize );
extern BOOL	USART1_TxQuePush      ( u8 data );
extern BOOL USART1_TxQuePop       ( u8 *data );

extern BOOL	USART1_PacketStreaming( LPCOMMPACKET pPacket );
extern BOOL	USART1_RxQuePush      ( u8 data );
extern BOOL USART1_RxQuePop       ( u8 *data );

extern COMMPACKET   USART1StreamingBuffer;

#endif // __EVKIT_UART_1_H__