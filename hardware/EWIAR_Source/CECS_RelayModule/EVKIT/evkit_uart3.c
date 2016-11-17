////////////////////////////////////////////////////////////////////////////////
//  Company           :
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Revision          : 1.00
//  Project           : EVKIT 2014
//  Editor            : Hyunju Lee
//  Create Date       : 2014-05-30
//  File              : EVKIT_UART3.C
////////////////////////////////////////////////////////////////////////////////
#include  "def_application.h"

////////////////////////////////////////////////////////////////////////////////
// UART1 Function Define
// using application "packet streaming"
UART_DATAQUE			USART3RxDataQue;      // Que for the Receive
UART_DATAQUE			USART3TxDataQue;    	// Que for the Transmit
UART_LINE_EVENT   USART3LineEvents;		  // UART Evens (PE, FE, OE, ORE)
COMMPACKET        USART3StreamingBuffer;

void USART3_Config( u32 nlbaudrate )
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef UART_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  
  // Init Que & Buffer
  USART3RxDataQue.nPopIndex = USART3RxDataQue.nPushIndex = 0;
  USART3TxDataQue.nPopIndex = USART3TxDataQue.nPushIndex = 0;
  USART3LineEvents.dwData   = 0uL;
  memset( (void*)&USART3StreamingBuffer, 0, sizeof(COMMPACKET) ); 

	// UART3_TX
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
  GPIO_Init( GPIOB, &GPIO_InitStructure );

	// UART3_RX
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
  GPIO_Init( GPIOB, &GPIO_InitStructure );

  // Release reset and enable clock
  USART_DeInit( USART3 );
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE );

  USART_StructInit( &UART_InitStructure );
  UART_InitStructure.USART_BaudRate							= nlbaudrate;
  UART_InitStructure.USART_WordLength						= USART_WordLength_8b;
  UART_InitStructure.USART_StopBits							= USART_StopBits_1;
  UART_InitStructure.USART_Parity								= USART_Parity_No ;
  UART_InitStructure.USART_Mode									= USART_Mode_Rx | USART_Mode_Tx;
  UART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;  
  USART_Init( USART3, &UART_InitStructure );

  // Enable and configure the priority of the UART2 Update IRQ Channel
  NVIC_InitStructure.NVIC_IRQChannel                    = USART3_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 7;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_Init( &NVIC_InitStructure );

  // Enable USART interrupts
  USART_ITConfig( USART3, USART_IT_PE,   ENABLE   );
  USART_ITConfig( USART3, USART_IT_TXE,  ENABLE   );
  USART_ITConfig( USART3, USART_IT_RXNE, ENABLE   );
  USART_ITConfig( USART3, USART_IT_TC,   DISABLE  );
  USART_ITConfig( USART3, USART_IT_IDLE, DISABLE  );
  USART_ITConfig( USART3, USART_IT_LBD,  DISABLE  );
  USART_ITConfig( USART3, USART_IT_CTS,  DISABLE  );
  USART_ITConfig( USART3, USART_IT_ERR,  DISABLE  );

  USART_Cmd( USART3, ENABLE );   // Enable the USART
}

void USART3_ISR( void )
{
  u8 data;
  
  // Overrun Error
  if( USART_GetFlagStatus(USART3, USART_FLAG_ORE) ) {
    USART_ReceiveData( USART3 );
    USART3LineEvents.bOE = 1;
  }
  
  // Noise or Framing Error
  if(  USART_GetFlagStatus(USART3, USART_FLAG_NE)
    || USART_GetFlagStatus(USART3, USART_FLAG_FE) ) {
    USART_ReceiveData( USART3 );
    USART3LineEvents.bFE = 1;
  }
  
  // used Parity Error
  if( USART_GetFlagStatus(USART3, USART_FLAG_PE) ) {
    USART_ReceiveData( USART3 );
    USART3LineEvents.bPE = 1;
  }
  
  // *** RxD : used  Push a new data into the receiver buffer
  // the FIFO is full
  if( USART_GetFlagStatus(USART3, USART_FLAG_RXNE) ) {
    if( !USART3_RxQuePush((u8)USART_ReceiveData(USART3)) ) {
      USART3LineEvents.bOE = 1;
    }
  }

  // *** TxD : used
  if(  USART_GetFlagStatus(USART3, USART_FLAG_TXE)
    && USART_GetITStatus(USART3, USART_IT_TXE) ) {
    if( USART3_TxQuePop(&data) ) {
      USART_SendData( USART3, data );
    } else {
      USART_ITConfig( USART3, USART_IT_TXE, DISABLE );
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// USART Transmit Function Define
void USART3_SendPacket( LPCOMMPACKET pPacket, BOOL bAck )
{
  if( bAck ) {
    pPacket->nHeader = PACKET_HEADER;
  } else {
    pPacket->nHeader = NACK_PACKET_HEADER;
  }
  
  pPacket->nChksum  = pPacket->wCanID
                    + pPacket->wData[0]
                    + pPacket->wData[1]
                    + pPacket->wData[2]
                    + pPacket->wData[3];
  USART3_Write( (u8*)pPacket, COMMPACKET_SIZE );
}

u16 USART3_Write( u8 *buffer, u16 buffersize )
{
  if( buffersize == 0 ) { return 0; }
  
  u16 i = 0;

  ENTR_CRT_SECTION();
  if( USART3TxDataQue.nPushIndex == USART3TxDataQue.nPopIndex ) {
    USART_ITConfig( USART3, USART_IT_TXE, ENABLE );
    USART_SendData( USART3, buffer[i++] );         // 1'st data transmit
  }

  for ( ; i<buffersize; i++ ) {
    if( !USART3_TxQuePush(buffer[i]) ) { break; }
  }
  EXT_CRT_SECTION();
  
  return( i );
}

BOOL USART3_TxQuePush( u8 data )
{
  u16 tmp = USART3TxDataQue.nPushIndex + 1;
  tmp %= UART_DATAQUE_SIZE;
  if( tmp == USART3TxDataQue.nPopIndex ) { return FALSE; }  // overflow
  
  USART3TxDataQue.nBuffer[USART3TxDataQue.nPushIndex++] = data;
  USART3TxDataQue.nPushIndex %= UART_DATAQUE_SIZE;

  return TRUE;
}

BOOL USART3_TxQuePop( u8 *data )
{
  // is empty
  if( USART3TxDataQue.nPushIndex == USART3TxDataQue.nPopIndex ) {
    return FALSE;
  }
  
  *data = USART3TxDataQue.nBuffer[USART3TxDataQue.nPopIndex++];
  USART3TxDataQue.nPopIndex %= UART_DATAQUE_SIZE;

  return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// USART Receive Function Define
BOOL USART3_PacketStreaming( void )
{
	u8					nch, i;
	u16					nchksum;
	u8					*buffer = (u8*)&USART3StreamingBuffer;
	
  while( USART3_RxQuePop(&nch) ) {
		// packet streaming
		for( i=0; i<(COMMPACKET_SIZE-1); i++ ) { buffer[i] = buffer[i+1]; }
		buffer[COMMPACKET_SIZE-1] = nch;

		// check packet_header & checksum
		if( USART3StreamingBuffer.nHeader == PACKET_HEADER ) {
      nchksum = USART3StreamingBuffer.wCanID
              + USART3StreamingBuffer.wData[0]
              + USART3StreamingBuffer.wData[1]
              + USART3StreamingBuffer.wData[2]
              + USART3StreamingBuffer.wData[3];
			if( nchksum == USART3StreamingBuffer.nChksum ) {
        return PushMessage( *((LPMESSAGE)(&USART3StreamingBuffer)) );
      }
		}
  }
  
  return FALSE;
}

BOOL USART3_RxQuePush( u8 data )
{
  u16 tmp = USART3RxDataQue.nPushIndex + 1;
  tmp %= UART_DATAQUE_SIZE;
  if( tmp == USART3RxDataQue.nPopIndex ) { return FALSE; }  // overflow
  
  USART3RxDataQue.nBuffer[USART3RxDataQue.nPushIndex++] = data;
  USART3RxDataQue.nPushIndex %= UART_DATAQUE_SIZE;

  return TRUE;
}

BOOL USART3_RxQuePop( u8 *data )
{
  // is empty
  if( USART3RxDataQue.nPushIndex == USART3RxDataQue.nPopIndex ) {
    return FALSE;
  }
  
  *data = USART3RxDataQue.nBuffer[USART3RxDataQue.nPopIndex++];
  USART3RxDataQue.nPopIndex %= UART_DATAQUE_SIZE;

  return TRUE;
}
