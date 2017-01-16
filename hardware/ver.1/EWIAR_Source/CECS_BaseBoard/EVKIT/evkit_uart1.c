////////////////////////////////////////////////////////////////////////////////
//  Company           :
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Revision          : 1.00
//  Project           : EVKIT 2014
//  Editor            : Hyunju Lee
//  Create Date       : 2014-05-30
//  File              : EVKIT_UART1.C
////////////////////////////////////////////////////////////////////////////////
#include  "def_application.h"

////////////////////////////////////////////////////////////////////////////////
// UART1 Function Define
// using application "packet streaming"
UART_DATAQUE			USART1RxDataQue;      // Que for the Receive
UART_DATAQUE			USART1TxDataQue;    	// Que for the Transmit
UART_LINE_EVENT   USART1LineEvents;		  // UART Evens (PE, FE, OE, ORE)
COMMPACKET        USART1StreamingBuffer;

void USART1_Config( u32 nlbaudrate )
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef UART_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  
  // Init Que & Buffer
  USART1RxDataQue.nPopIndex = USART1RxDataQue.nPushIndex = 0;
  USART1TxDataQue.nPopIndex = USART1TxDataQue.nPushIndex = 0;
  USART1LineEvents.dwData   = 0uL;
  memset( (void*)&USART1StreamingBuffer, 0, sizeof(COMMPACKET) ); 

	// UART1_TX
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
  GPIO_Init( GPIOA, &GPIO_InitStructure );

	// UART1_RX
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
  GPIO_Init( GPIOA, &GPIO_InitStructure );

  // Release reset and enable clock
  USART_DeInit( USART1 );
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE );

  USART_StructInit( &UART_InitStructure );
  UART_InitStructure.USART_BaudRate							= nlbaudrate;
  UART_InitStructure.USART_WordLength						= USART_WordLength_8b;
  UART_InitStructure.USART_StopBits							= USART_StopBits_1;
  UART_InitStructure.USART_Parity								= USART_Parity_No ;
  UART_InitStructure.USART_Mode									= USART_Mode_Rx | USART_Mode_Tx;
  UART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;  
  USART_Init( USART1, &UART_InitStructure );

  // Enable and configure the priority of the UART2 Update IRQ Channel
  NVIC_InitStructure.NVIC_IRQChannel                    = USART1_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 7;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_Init( &NVIC_InitStructure );

  // Enable USART interrupts
  USART_ITConfig( USART1, USART_IT_PE,   ENABLE   );
  USART_ITConfig( USART1, USART_IT_TXE,  ENABLE   );
  USART_ITConfig( USART1, USART_IT_RXNE, ENABLE   );
  USART_ITConfig( USART1, USART_IT_TC,   DISABLE  );
  USART_ITConfig( USART1, USART_IT_IDLE, DISABLE  );
  USART_ITConfig( USART1, USART_IT_LBD,  DISABLE  );
  USART_ITConfig( USART1, USART_IT_CTS,  DISABLE  );
  USART_ITConfig( USART1, USART_IT_ERR,  DISABLE  );

  USART_Cmd( USART1, ENABLE );   // Enable the USART
}

void USART1_ISR( void )
{
  u8 data;
  
  // Overrun Error
  if( USART_GetFlagStatus(USART1, USART_FLAG_ORE) ) {
    USART_ReceiveData( USART1 );
    USART1LineEvents.bOE = 1;
  }
  
  // Noise or Framing Error
  if(  USART_GetFlagStatus(USART1, USART_FLAG_NE)
    || USART_GetFlagStatus(USART1, USART_FLAG_FE) ) {
    USART_ReceiveData( USART1 );
    USART1LineEvents.bFE = 1;
  }
  
  // used Parity Error
  if( USART_GetFlagStatus(USART1, USART_FLAG_PE) ) {
    USART_ReceiveData( USART1 );
    USART1LineEvents.bPE = 1;
  }
  
  // *** RxD : used  Push a new data into the receiver buffer
  // the FIFO is full
  if( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) ) {
    if( !USART1_RxQuePush((u8)USART_ReceiveData(USART1)) ) {
      USART1LineEvents.bOE = 1;
    }
  }

  // *** TxD : used
  if(  USART_GetFlagStatus(USART1, USART_FLAG_TXE)
    && USART_GetITStatus(USART1, USART_IT_TXE) ) {
    if( USART1_TxQuePop(&data) ) {
      USART_SendData( USART1, data );
    } else {
      USART_ITConfig( USART1, USART_IT_TXE, DISABLE );
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// USART Transmit Function Define
void USART1_SendPacket( LPCOMMPACKET pPacket, BOOL bAck )
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
  USART1_Write( (u8*)pPacket, COMMPACKET_SIZE );
}

u16 USART1_Write( u8 *buffer, u16 buffersize )
{
  if( buffersize == 0 ) { return 0; }
  
  u16 i = 0;

  ENTR_CRT_SECTION();
  if( USART1TxDataQue.nPushIndex == USART1TxDataQue.nPopIndex ) {
    USART_ITConfig( USART1, USART_IT_TXE, ENABLE );
    USART_SendData( USART1, buffer[i++] );         // 1'st data transmit
  }

  for ( ; i<buffersize; i++ ) {
    if( !USART1_TxQuePush(buffer[i]) ) { break; }
  }
  EXT_CRT_SECTION();
  
  return( i );
}

BOOL USART1_TxQuePush( u8 data )
{
  u16 tmp = USART1TxDataQue.nPushIndex + 1;
  tmp %= UART_DATAQUE_SIZE;
  if( tmp == USART1TxDataQue.nPopIndex ) { return FALSE; }  // overflow
  
  USART1TxDataQue.nBuffer[USART1TxDataQue.nPushIndex++] = data;
  USART1TxDataQue.nPushIndex %= UART_DATAQUE_SIZE;

  return TRUE;
}

BOOL USART1_TxQuePop( u8 *data )
{
  // is empty
  if( USART1TxDataQue.nPushIndex == USART1TxDataQue.nPopIndex ) {
    return FALSE;
  }
  
  *data = USART1TxDataQue.nBuffer[USART1TxDataQue.nPopIndex++];
  USART1TxDataQue.nPopIndex %= UART_DATAQUE_SIZE;

  return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// USART Receive Function Define
BOOL USART1_PacketStreaming( LPCOMMPACKET pPacket )
{
	u8					nch, i;
	u16					nchksum;
	u8					*buffer = (u8*)&USART1StreamingBuffer;
	
  while( USART1_RxQuePop(&nch) ) {
		// packet streaming
		for( i=0; i<(COMMPACKET_SIZE-1); i++ ) { buffer[i] = buffer[i+1]; }
		buffer[COMMPACKET_SIZE-1] = nch;

		// check packet_header & checksum
		if( USART1StreamingBuffer.nHeader == PACKET_HEADER ) {
      nchksum = USART1StreamingBuffer.wCanID
              + USART1StreamingBuffer.wData[0]
              + USART1StreamingBuffer.wData[1]
              + USART1StreamingBuffer.wData[2]
              + USART1StreamingBuffer.wData[3];
			if( nchksum == USART1StreamingBuffer.nChksum ) {
        memcpy( pPacket, &USART1StreamingBuffer, COMMPACKET_SIZE );
        return TRUE;
      }
		}
  }
  
  return FALSE;
}

BOOL USART1_RxQuePush( u8 data )
{
  u16 tmp = USART1RxDataQue.nPushIndex + 1;
  tmp %= UART_DATAQUE_SIZE;
  if( tmp == USART1RxDataQue.nPopIndex ) { return FALSE; }  // overflow
  
  USART1RxDataQue.nBuffer[USART1RxDataQue.nPushIndex++] = data;
  USART1RxDataQue.nPushIndex %= UART_DATAQUE_SIZE;

  return TRUE;
}

BOOL USART1_RxQuePop( u8 *data )
{
  // is empty
  if( USART1RxDataQue.nPushIndex == USART1RxDataQue.nPopIndex ) {
    return FALSE;
  }
  
  *data = USART1RxDataQue.nBuffer[USART1RxDataQue.nPopIndex++];
  USART1RxDataQue.nPopIndex %= UART_DATAQUE_SIZE;

  return TRUE;
}
