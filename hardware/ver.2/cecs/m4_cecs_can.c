////////////////////////////////////////////////////////////////////////////////
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Microcontroller		: Cortex M4 STM32F407ZET6
//  Revision          : 1.00
//  Project           : CECS_M4 2016
//  Create Date       : 2016-11-30
//  File              : M4_CECS_CAN_H.C
////////////////////////////////////////////////////////////////////////////////
#include "m4_evkit_define.h"
#include "m4_cecs_io_base.h"

#include "m4_cecs_can.h"

////////////////////////////////////////////////////////////////////////////////
// CAN_ID function
u32   nAPPLICATION_CAN_ID = 0;

////////////////////////////////////////////////////////////////////////////////
// CAN function
CanTxMsg    CanTxMessage;
CanRxMsg    CanRxMessage;

void Init_CAN1( void )
{
  GPIO_InitTypeDef          GPIO_InitStructure;
  CAN_InitTypeDef           CAN_InitStructure;
  CAN_FilterInitTypeDef     CAN_FilterInitStructure;
  TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
  NVIC_InitTypeDef          NVIC_InitStructure;

  RCC_APB1PeriphClockCmd( RCC_APB1Periph_CAN1, ENABLE );
  CAN_DeInit( CAN1 );

  GPIO_PinAFConfig( GPIOB, GPIO_PinSource8, GPIO_AF_CAN1 );  
  GPIO_PinAFConfig( GPIOB, GPIO_PinSource9, GPIO_AF_CAN1 );  

  // Configure CAN pin : RX
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
  GPIO_Init( GPIOB, &GPIO_InitStructure );

  //  Configure CAN pin : TX
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
  GPIO_Init( GPIOB, &GPIO_InitStructure );

  // CAN cell init
  CAN_StructInit( &CAN_InitStructure );
  CAN_InitStructure.CAN_TTCM      = DISABLE;          // Initialize the time triggered communication mode
  CAN_InitStructure.CAN_ABOM      = DISABLE;          // Initialize the automatic bus-off management
  CAN_InitStructure.CAN_AWUM      = DISABLE;          // Initialize the automatic wake-up mode
  CAN_InitStructure.CAN_NART      = DISABLE;          // Initialize the no automatic retransmission
  CAN_InitStructure.CAN_RFLM      = DISABLE;          // Initialize the receive FIFO locked mode
  CAN_InitStructure.CAN_TXFP      = DISABLE;          // Initialize the transmit FIFO priority
  CAN_InitStructure.CAN_Mode      = CAN_Mode_Normal;  // Initialize the CAN_Mode member
  //CAN_InitStructure.CAN_Mode      = CAN_Mode_LoopBack;
  CAN_InitStructure.CAN_SJW       = CAN_SJW_1tq;      // Initialize the CAN_SJW member
  CAN_InitStructure.CAN_BS1       = CAN_BS1_6tq;      // Initialize the CAN_BS1 member
  CAN_InitStructure.CAN_BS2       = CAN_BS2_8tq;      // Initialize the CAN_BS2 member
  CAN_InitStructure.CAN_Prescaler = 4;                // Initialize the CAN_Prescaler member. PCLK1(4MHz)/4=1000KHz/(1+4+5)=100KHz
  CAN_Init( CAN1, &CAN_InitStructure );

  // CAN filter init
  CAN_FilterInitStructure.CAN_FilterNumber          = 0;
  CAN_FilterInitStructure.CAN_FilterMode            = CAN_FilterMode_IdMask; // 0
  CAN_FilterInitStructure.CAN_FilterScale           = CAN_FilterScale_32bit; // 1
  CAN_FilterInitStructure.CAN_FilterIdHigh          = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow           = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh      = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow       = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment  = CAN_FIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation      = ENABLE;
  CAN_FilterInit( &CAN_FilterInitStructure );

  NVIC_InitStructure.NVIC_IRQChannel                    = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 7;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_Init( &NVIC_InitStructure );
  
  CAN_ITConfig( CAN1, CAN_IT_FMP0, ENABLE );

 	// TIM2 initialize. for CAN timeout
	// PCLK1 __________ 36Mhz[ If(APB1_prescaler=1) x1 else x2 ]
  // RCC_PCLK1Config( RCC_HCLK_Div2 ); >> APB1_prescaler=2
	// prescaler  _____ 10,000hz
	TIM_TimeBaseStructInit( &TIM_TimeBaseStructure );
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );
	TIM_DeInit( TIM2 );
	TIM_TimeBaseStructure.TIM_Period        = 2500;   // Autoreload value ARR set 250msec
  TIM_TimeBaseStructure.TIM_Prescaler     = (u16)((SYSTEM_CORE_CLOCK>>1) / 10000uL) - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseInit( TIM2, &TIM_TimeBaseStructure );
  TIM_ClearITPendingBit( TIM2, TIM_IT_Update );

  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannel                    = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
  NVIC_Init( &NVIC_InitStructure );
}

void ISR_CAN1_Reception( void )
{
  MESSAGE msg;
  
  CAN_Receive( CAN1, CAN_FIFO0, &CanRxMessage );

  if( (CanRxMessage.IDE == CAN_ID_STD) && (CanRxMessage.DLC == 8) ) {
    ClearCANError();
    msg.nMsgType    = ACK_PACKET_HEADER;
    msg.nMsgSubType = CanRxMessage.StdId;
    for( u8 i=0; i<8; i++ ) { msg.nData[i] = CanRxMessage.Data[i]; }
    PushMessage( msg );
    SetCANEvent();
  } else {
    SetCANError();
  }
}

// init transmit message
// CanTxMessage.StdId = ?;
// CanTxMessage.ExtId = 0x00000000;
// CanTxMessage.IDE   = CAN_ID_STD;
// CanTxMessage.RTR   = CAN_RTR_DATA;
// CanTxMessage.DLC   = 8;
bool CANTrasmitMessage( LPCOMMPACKET pPacket )
{
  // init transmit message
  CanTxMessage.StdId  = pPacket->wCanID;
  CanTxMessage.ExtId  = 0x00000000;
  CanTxMessage.IDE    = CAN_ID_STD;
  CanTxMessage.RTR    = CAN_RTR_DATA;
  CanTxMessage.DLC    = 8;
  for( u16 i=0; i<8; i++ ) { CanTxMessage.Data[i] = pPacket->nData[i]; }

  ClearCANEvent();
  if ( CAN_Transmit( CAN1, &CanTxMessage ) == CAN_NO_MB ) { return false; }
  StartCanTimeOut( 250 );

  do{
    if( CheckCanTimeOut() ) { return false; }
  } while( !CheckCANEvent() );
 
  return true;
}

////////////////////////////////////////////////////////////////////////////////
void StartCanTimeOut( u16 timeout )   // timeout = msec
{
  ClearCanTimeOut();

  TIM_SetCounter( TIM2, 0 );
  TIM2->ARR = timeout * 10;
  TIM_ClearITPendingBit( TIM2, TIM_IT_Update );
	TIM_ITConfig( TIM2, TIM_IT_Update, ENABLE );
	TIM_Cmd( TIM2, ENABLE );
}

void StopCANTimer( void )
{
	TIM_Cmd( TIM2, DISABLE );
	TIM_ITConfig( TIM2, TIM_IT_Update, DISABLE );
  TIM_ClearITPendingBit( TIM2, TIM_IT_Update );
}

void ISR_TIM2forCANTimeOut( void )
{ // TIM2_IRQChannel
  SetCanTimeOut();
  StopCANTimer();
  
  TIM_ClearITPendingBit( TIM2, TIM_IT_Update );
}
