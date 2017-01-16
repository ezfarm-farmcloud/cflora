////////////////////////////////////////////////////////////////////////////////
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Microcontroller		: Cortex M4 STM32F407ZET6
//  Revision          : V1.00
//  Project           : CECS_M4 2016
//  Create Date       : 2016-11-30
//  File              : M4_CECS_UART6.C
////////////////////////////////////////////////////////////////////////////////
#include "m4_evkit_define.h"
#include "m4_cecs_uart6.h"

UART_DATAQUE			USART6_RxDataQue;       // Que for the Receive
UART_DATAQUE			USART6_TxDataQue;    	  // Que for the Transmission
UART_LINE_EVENT   USART6_LineEvents;		  // UART Evens (PE, FE, OE, ORE)
COMMPACKET        USART6_StreamingBuffer;

void Init_UART6( u32 baudrate )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef UART_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  
  // Init Que & Buffer
  USART6_RxDataQue.nPopIndex = USART6_RxDataQue.nPushIndex = 0;
  USART6_TxDataQue.nPopIndex = USART6_TxDataQue.nPushIndex = 0;
  USART6_LineEvents.dwData   = 0uL;
  memset( (void*)&USART6_StreamingBuffer, 0, sizeof(COMMPACKET) ); 

  // rs-485 direction control
  RS485_DirReception();

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_8;
  GPIO_Init( GPIOG, &GPIO_InitStructure );

  // configure usart6 pin
  USART_DeInit( USART6 );
  GPIO_PinAFConfig( GPIOC, GPIO_PinSource6, GPIO_AF_USART6 );  
  GPIO_PinAFConfig( GPIOC, GPIO_PinSource7, GPIO_AF_USART6 );  
  
	// UART6_TX
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
  GPIO_Init( GPIOC, &GPIO_InitStructure );

	// UART6_RX
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
  GPIO_Init( GPIOC, &GPIO_InitStructure );

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART6, ENABLE );

  USART_StructInit( &UART_InitStructure );
  UART_InitStructure.USART_BaudRate							= baudrate;
  UART_InitStructure.USART_WordLength						= USART_WordLength_8b;
  UART_InitStructure.USART_StopBits							= USART_StopBits_1;
  UART_InitStructure.USART_Parity								= USART_Parity_No ;
  UART_InitStructure.USART_Mode									= USART_Mode_Rx | USART_Mode_Tx;
  UART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;  
  USART_Init( USART6, &UART_InitStructure );

  // Enable and configure the priority of the UART6 Update IRQ Channel
  NVIC_InitStructure.NVIC_IRQChannel                    = USART6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 7;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_Init( &NVIC_InitStructure );

  // Enable USART interrupts
  USART_ITConfig( USART6, USART_IT_PE,   ENABLE   );
  USART_ITConfig( USART6, USART_IT_TXE,  ENABLE   );
  USART_ITConfig( USART6, USART_IT_RXNE, ENABLE   );
  USART_ITConfig( USART6, USART_IT_TC,   DISABLE  );
  USART_ITConfig( USART6, USART_IT_IDLE, DISABLE  );
  USART_ITConfig( USART6, USART_IT_LBD,  DISABLE  );
  USART_ITConfig( USART6, USART_IT_CTS,  DISABLE  );
  USART_ITConfig( USART6, USART_IT_ERR,  DISABLE  );
  
  USART_Cmd( USART6, ENABLE );
}

void ISR_USART6( void )
{
  u8 data;
  
  // Overrun Error
  if( USART_GetFlagStatus(USART6, USART_FLAG_ORE) ) {
    USART_ReceiveData( USART6 );
    USART6_LineEvents.bOE = 1;
  }
  
  // Noise or Framing Error
  if(  USART_GetFlagStatus(USART6, USART_FLAG_NE)
    || USART_GetFlagStatus(USART6, USART_FLAG_FE) ) {
    USART_ReceiveData( USART6 );
    USART6_LineEvents.bFE = 1;
  }
  
  // used Parity Error
  if( USART_GetFlagStatus(USART6, USART_FLAG_PE) ) {
    USART_ReceiveData( USART6 );
    USART6_LineEvents.bPE = 1;
  }
  
  // *** RxD : used  Push a new data into the receiver buffer
  // the FIFO is full
  if( USART_GetFlagStatus(USART6, USART_FLAG_RXNE) ) {
    if( !USART6_RxQuePush((u8)USART_ReceiveData(USART6)) ) {
      USART6_LineEvents.bOE = 1;
    }
  }

  // *** TxD : used
  if(  USART_GetFlagStatus(USART6, USART_FLAG_TXE)
    && USART_GetITStatus(USART6, USART_IT_TXE) ) {
    if( USART6_TxQuePop(&data) ) {
      USART_SendData( USART6, data );
    } else {
      USART_ITConfig( USART6, USART_IT_TXE, DISABLE );
      RS485_DirReception();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// USART Transmit Function Define
void USART6_SendPacket( LPCOMMPACKET pPacket, bool bAck )
{
  if( bAck ) {
    pPacket->nHeader = ACK_PACKET_HEADER;
  } else {
    pPacket->nHeader = NACK_PACKET_HEADER;
  }
  
  pPacket->nChksum  = pPacket->wCanID
                    + pPacket->wData[0]
                    + pPacket->wData[1]
                    + pPacket->wData[2]
                    + pPacket->wData[3];
  USART6_Write( (u8*)pPacket, COMMPACKET_SIZE );
}

u16 USART6_Write( u8 *buffer, u16 buffersize )
{
  if( buffersize == 0 ) { return 0; }
  
  u16 i = 0;

  ENTR_CRT_SECTION();
  if( USART6_TxDataQue.nPushIndex == USART6_TxDataQue.nPopIndex ) {
    RS485_DirTransmission();
    USART_ITConfig( USART6, USART_IT_TXE, ENABLE );
    USART_SendData( USART6, buffer[i++] );         // 1'st data transmit
  }

  for ( ; i<buffersize; i++ ) {
    if( !USART6_TxQuePush(buffer[i]) ) { break; }
  }
  EXT_CRT_SECTION();
  
  return( i );
}

////////////////////////////////////////////////////////////////////////////////
// USART Receive Function Define
bool USART6_PacketStreaming( LPCOMMPACKET pPacket )
{
	u8					nch, i;
	u16					nchksum;
	u8					*buffer = (u8*)&USART6_StreamingBuffer;
	
  while( USART6_RxQuePop(&nch) ) {
		// packet streaming
		for( i=0; i<(COMMPACKET_SIZE-1); i++ ) { buffer[i] = buffer[i+1]; }
		buffer[COMMPACKET_SIZE-1] = nch;

		// check packet_header & checksum
		if( USART6_StreamingBuffer.nHeader == ACK_PACKET_HEADER ) {
      nchksum = USART6_StreamingBuffer.wCanID
              + USART6_StreamingBuffer.wData[0]
              + USART6_StreamingBuffer.wData[1]
              + USART6_StreamingBuffer.wData[2]
              + USART6_StreamingBuffer.wData[3];
			if( nchksum == USART6_StreamingBuffer.nChksum ) {
        memcpy( pPacket, &USART6_StreamingBuffer, COMMPACKET_SIZE );
        return true;
      }
		}
  }
  
  return false;
}

bool USART6_TxQuePush( u8 data )
{
  u16 tmp = USART6_TxDataQue.nPushIndex + 1;
  tmp %= UART_DATAQUE_SIZE;
  if( tmp == USART6_TxDataQue.nPopIndex ) { return false; }  // overflow
  
  USART6_TxDataQue.nBuffer[USART6_TxDataQue.nPushIndex++] = data;
  USART6_TxDataQue.nPushIndex %= UART_DATAQUE_SIZE;

  return true;
}

bool USART6_TxQuePop( u8 *data )
{
  // is empty
  if( USART6_TxDataQue.nPushIndex == USART6_TxDataQue.nPopIndex ) {
    return false;
  }
  
  *data = USART6_TxDataQue.nBuffer[USART6_TxDataQue.nPopIndex++];
  USART6_TxDataQue.nPopIndex %= UART_DATAQUE_SIZE;

  return true;
}

bool USART6_RxQuePush( u8 data )
{
  u16 tmp = USART6_RxDataQue.nPushIndex + 1;
  tmp %= UART_DATAQUE_SIZE;
  if( tmp == USART6_RxDataQue.nPopIndex ) { return false; }  // overflow
  
  USART6_RxDataQue.nBuffer[USART6_RxDataQue.nPushIndex++] = data;
  USART6_RxDataQue.nPushIndex %= UART_DATAQUE_SIZE;

  return true;
}

bool USART6_RxQuePop( u8 *data )
{
  // is empty
  if( USART6_RxDataQue.nPushIndex == USART6_RxDataQue.nPopIndex ) {
    return false;
  }
  
  *data = USART6_RxDataQue.nBuffer[USART6_RxDataQue.nPopIndex++];
  USART6_RxDataQue.nPopIndex %= UART_DATAQUE_SIZE;

  return true;
}

