////////////////////////////////////////////////////////////////////////////////
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Microcontroller		: Cortex M4 STM32F407ZET6
//  Revision          : V1.00
//  Project           : CECS_M4 2016
//  Create Date       : 2016-11-30
//  File              : M4_CECS_MAIN.C
////////////////////////////////////////////////////////////////////////////////
#include "m4_evkit_define.h"

#include "m4_cecs_io_base.h"
#include "m4_cecs_uart1.h"
#include "m4_cecs_uart3.h"
#include "m4_cecs_uart6.h"
#include "m4_cecs_can.h"

#include "m4_cecs_analog.h"
#include "m4_cecs_isoin.h"
#include "m4_cecs_relay.h"

COMMPACKET  RxPacket, TxPacket;
u16         nModuleType = TYPE_MODULE_BASE;

///////////////////////////////////////////////////////////////////////////////
// Function Name	: main
void main(void)
{
  ENTR_CRT_SECTION();
  InitSysTick( 1000uL );   // system tick time 1msec
	ClearGlobalReg();
  EXT_CRT_SECTION();

  switch( Init_IOPort() ) {
    case 0x01 : nModuleType = TYPE_MODULE_ANALOG; break;  // 0x0100
    case 0x02 : nModuleType = TYPE_MODULE_ISOIN;  break;  // 0x0200
    case 0x04 : nModuleType = TYPE_MODULE_RELAY;  break;  // 0x0300
    default   : nModuleType = TYPE_MODULE_BASE;   break;  // 0x0000
  }
  nAPPLICATION_CAN_ID = GetBoardID() | nModuleType;
  
  if( GetBoardID() == 0x01 ) OnTestLED() ; 
  else OffTestLED() ;
  
  ClearMessageHandle();
  
  Init_UART1( 57600uL );
  Init_UART3( 57600uL );
  Init_UART6( 57600uL );
  Init_CAN1();
  
  
  OnBuzzer();   Delay( 200 );
  OffBuzzer();  Delay( 200 );

  switch( nModuleType ) {
    case TYPE_MODULE_ANALOG : ProcedureAnalogModule( nAPPLICATION_CAN_ID ); break;
    case TYPE_MODULE_ISOIN  : ProcedureISOINModule ( nAPPLICATION_CAN_ID ); break;
    case TYPE_MODULE_RELAY  : ProcedureRelayModule ( nAPPLICATION_CAN_ID ); break;
    default                 : ProcedureModuleBase  ( nAPPLICATION_CAN_ID ); break;
  }
}

void ProcedureModuleBase( u16 nTypeID )
{

 
  while( 1 ) {
 
    if( USART1_PacketStreaming( &RxPacket ) ) {
      if( (RxPacket.wCanID & 0x00FF) == nTypeID ) {
      OffTestLED() ;
      switch( RxPacket.wCanID & 0x0F00 ) {
          
          case tagCAN_GET_COUNTER :
           
            TxPacket.wCanID    = RxPacket.wCanID;
            TxPacket.dwData[0] = TIM3_GetCaptureCFreq();//TIM3_GetCaptureCounter();
            TxPacket.dwData[1] = TIM4_GetCaptureCFreq();//TIM4_GetCaptureCounter();
            USART1_SendPacket( &TxPacket, true );
            
            break;
            
          case tagCAN_ID_SCANNING :
      
            TxPacket.wCanID  = RxPacket.wCanID;
             USART1_SendPacket( &TxPacket, true );
       
            break;
            
          default :
            TxPacket.wCanID  = RxPacket.wCanID;
            USART1_SendPacket( &TxPacket, false );
            break;
        }
      } else {
        OnTestLED() ;  
        if( CANTrasmitMessage( &RxPacket ) ) {
          if( GetMessage((LPMESSAGE)&TxPacket) ) {
            if(  (TxPacket.nHeader==ACK_PACKET_HEADER)
              && (TxPacket.wCanID==RxPacket.wCanID) ) {
              USART1_SendPacket( &TxPacket, true );
            }
          }
        } else {
           TxPacket.wCanID = RxPacket.wCanID;
          USART1_SendPacket( &TxPacket, false );
        }
      }
    }
  
  } //while
}
