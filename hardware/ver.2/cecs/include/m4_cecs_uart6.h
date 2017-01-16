////////////////////////////////////////////////////////////////////////////////
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Microcontroller		: Cortex M4 STM32F407ZET6
//  Revision          : V1.00
//  Project           : CECS_M4 2016
//  Create Date       : 2016-11-30
//  File              : M4_CECS_UART6.H
////////////////////////////////////////////////////////////////////////////////
#ifndef __M4_CECS_UART6_H__
#define __M4_CECS_UART6_H__

#define RS485_DirReception()      (GPIOG->ODR &= ~GPIO_Pin_8)
#define RS485_DirTransmission()   (GPIOG->ODR |= GPIO_Pin_8)

extern void Init_UART6            ( u32 baudrate );
extern void ISR_USART6            ( void );

extern void	USART6_SendPacket     ( LPCOMMPACKET pPacket, bool bAck );
extern u16 	USART6_Write          ( u8 *buffer, u16 buffersize );
extern bool	USART6_TxQuePush      ( u8 data );
extern bool USART6_TxQuePop       ( u8 *data );

extern bool	USART6_PacketStreaming( LPCOMMPACKET pPacket );
extern bool	USART6_RxQuePush      ( u8 data );
extern bool USART6_RxQuePop       ( u8 *data );

#endif // __M4_CECS_UART6_H__

