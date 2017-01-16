////////////////////////////////////////////////////////////////////////////////
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Microcontroller		: Cortex M4 STM32F407ZET6
//  Revision          : V1.00
//  Project           : CECS_M4 2016
//  Create Date       : 2016-11-30
//  File              : M4_CECS_UART1.C
////////////////////////////////////////////////////////////////////////////////
#include "m4_evkit_define.h"
#include "m4_cecs_uart1.h"

UART_DATAQUE			USART1_RxDataQue;       // Que for the Receive
UART_DATAQUE			USART1_TxDataQue;    	  // Que for the Transmission
UART_LINE_EVENT   USART1_LineEvents;		  // UART Evens (PE, FE, OE, ORE)
COMMPACKET        USART1_StreamingBuffer;

void Init_UART1( u32 baudrate )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef UART_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;

  // Init Que & Buffer
  USART1_RxDataQue.nPopIndex = USART1_RxDataQue.nPushIndex = 0;
  USART1_TxDataQue.nPopIndex = USART1_TxDataQue.nPushIndex = 0;
  USART1_LineEvents.dwData   = 0uL;
  memset( (void*)&USART1_StreamingBuffer, 0, sizeof(COMMPACKET) ); 
  
  // configure usart1 pin
  USART_DeInit( USART1 );
  GPIO_PinAFConfig( GPIOA,GPIO_PinSource9,  GPIO_AF_USART1 );  
  GPIO_PinAFConfig( GPIOA,GPIO_PinSource10, GPIO_AF_USART1 );  
  
	// UART1_TX
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
  GPIO_Init( GPIOA, &GPIO_InitStructure );

	// UART1_RX
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
  GPIO_Init( GPIOA, &GPIO_InitStructure );

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE );

  USART_StructInit( &UART_InitStructure );
  UART_InitStructure.USART_BaudRate							= baudrate;
  UART_InitStructure.USART_WordLength						= USART_WordLength_8b;
  UART_InitStructure.USART_StopBits							= USART_StopBits_1;
  UART_InitStructure.USART_Parity								= USART_Parity_No ;
  UART_InitStructure.USART_Mode									= USART_Mode_Rx | USART_Mode_Tx;
  UART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;  
  USART_Init( USART1, &UART_InitStructure );

  // Enable and configure the priority of the UART6 Update IRQ Channel
  NVIC_InitStructure.NVIC_IRQChannel                    = USART1_IRQn;
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

  USART_Cmd( USART1, ENABLE );
}

void ISR_USART1( void )
{
  u8 data;
  
  // Overrun Error
  if( USART_GetFlagStatus(USART1, USART_FLAG_ORE) ) {
    USART_ReceiveData( USART1 );
    USART1_LineEvents.bOE = 1;
  }
  
  // Noise or Framing Error
  if(  USART_GetFlagStatus(USART1, USART_FLAG_NE)
    || USART_GetFlagStatus(USART1, USART_FLAG_FE) ) {
    USART_ReceiveData( USART1 );
    USART1_LineEvents.bFE = 1;
  }
  
  // used Parity Error
  if( USART_GetFlagStatus(USART1, USART_FLAG_PE) ) {
    USART_ReceiveData( USART1 );
    USART1_LineEvents.bPE = 1;
  }
  
  // *** RxD : used  Push a new data into the receiver buffer
  // the FIFO is full
  if( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) ) {
    if( !USART1_RxQuePush((u8)USART_ReceiveData(USART1)) ) {
      USART1_LineEvents.bOE = 1;
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
void USART1_SendPacket( LPCOMMPACKET pPacket, bool bAck )
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
  USART1_Write( (u8*)pPacket, COMMPACKET_SIZE );
}

u16 USART1_Write( u8 *buffer, u16 buffersize )
{
  if( buffersize == 0 ) { return 0; }
  
  u16 i = 0;

  ENTR_CRT_SECTION();
  if( USART1_TxDataQue.nPushIndex == USART1_TxDataQue.nPopIndex ) {
    USART_ITConfig( USART1, USART_IT_TXE, ENABLE );
    USART_SendData( USART1, buffer[i++] );         // 1'st data transmit
  }

  for ( ; i<buffersize; i++ ) {
    if( !USART1_TxQuePush(buffer[i]) ) { break; }
  }
  EXT_CRT_SECTION();
  
  return( i );
}

////////////////////////////////////////////////////////////////////////////////
// USART Receive Function Define
bool USART1_PacketStreaming( LPCOMMPACKET pPacket )
{
	u8					nch, i;
	u16					nchksum;
	u8					*buffer = (u8*)&USART1_StreamingBuffer;
	
  while( USART1_RxQuePop(&nch) ) {
		// packet streaming
		for( i=0; i<(COMMPACKET_SIZE-1); i++ ) { buffer[i] = buffer[i+1]; }
		buffer[COMMPACKET_SIZE-1] = nch;

		// check packet_header & checksum
		if( USART1_StreamingBuffer.nHeader == ACK_PACKET_HEADER ) {
      nchksum = USART1_StreamingBuffer.wCanID
              + USART1_StreamingBuffer.wData[0]
              + USART1_StreamingBuffer.wData[1]
              + USART1_StreamingBuffer.wData[2]
              + USART1_StreamingBuffer.wData[3];
			if( nchksum == USART1_StreamingBuffer.nChksum ) {
        memcpy( pPacket, &USART1_StreamingBuffer, COMMPACKET_SIZE );
        return true;
      }
		}
  }
  
  return false;
}

bool USART1_TxQuePush( u8 data )
{
  u16 tmp = USART1_TxDataQue.nPushIndex + 1;
  tmp %= UART_DATAQUE_SIZE;
  if( tmp == USART1_TxDataQue.nPopIndex ) { return false; }  // overflow
  
  USART1_TxDataQue.nBuffer[USART1_TxDataQue.nPushIndex++] = data;
  USART1_TxDataQue.nPushIndex %= UART_DATAQUE_SIZE;

  return true;
}

bool USART1_TxQuePop( u8 *data )
{
  // is empty
  if( USART1_TxDataQue.nPushIndex == USART1_TxDataQue.nPopIndex ) {
    return false;
  }
  
  *data = USART1_TxDataQue.nBuffer[USART1_TxDataQue.nPopIndex++];
  USART1_TxDataQue.nPopIndex %= UART_DATAQUE_SIZE;

  return true;
}

bool USART1_RxQuePush( u8 data )
{
  u16 tmp = USART1_RxDataQue.nPushIndex + 1;
  tmp %= UART_DATAQUE_SIZE;
  if( tmp == USART1_RxDataQue.nPopIndex ) { return false; }  // overflow
  
  USART1_RxDataQue.nBuffer[USART1_RxDataQue.nPushIndex++] = data;
  USART1_RxDataQue.nPushIndex %= UART_DATAQUE_SIZE;

  return true;
}

bool USART1_RxQuePop( u8 *data )
{
  // is empty
  if( USART1_RxDataQue.nPushIndex == USART1_RxDataQue.nPopIndex ) {
    return false;
  }
  
  *data = USART1_RxDataQue.nBuffer[USART1_RxDataQue.nPopIndex++];
  USART1_RxDataQue.nPopIndex %= UART_DATAQUE_SIZE;

  return true;
}

