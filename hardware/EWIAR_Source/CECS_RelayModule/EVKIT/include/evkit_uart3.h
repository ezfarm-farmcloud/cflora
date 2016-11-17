////////////////////////////////////////////////////////////////////////////////
//  Company           : 
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Revision          : 1.00
//  Project           : EVKIT 2014
//  Editor            : Hyunju Lee
//  Create Date       : 2014-05-30
//  File              : EVKIT_UART3.H
////////////////////////////////////////////////////////////////////////////////
#ifndef __EVKIT_UART_3_H__
#define __EVKIT_UART_3_H__

// USART1 Function Define
extern void	USART3_Config         ( u32 nlbaudrate );
extern void USART3_ISR            ( void );

extern void	USART3_SendPacket     ( LPCOMMPACKET pPacket, BOOL bAck );
extern u16 	USART3_Write          ( u8 *buffer, u16 buffersize );
extern BOOL	USART3_TxQuePush      ( u8 data );
extern BOOL USART3_TxQuePop       ( u8 *data );

extern BOOL	USART3_PacketStreaming( void );
extern BOOL	USART3_RxQuePush      ( u8 data );
extern BOOL USART3_RxQuePop       ( u8 *data );

extern COMMPACKET   USART3StreamingBuffer;

#endif // __EVKIT_UART_3_H__