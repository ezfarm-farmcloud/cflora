////////////////////////////////////////////////////////////////////////////////
//  Company           : 
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Revision          : 1.00
//  Project           : EVKIT 2014
//  Editor            : Hyunju Lee
//  Create Date       : 2014-05-30
//  File              : EVKIT_IO.C
////////////////////////////////////////////////////////////////////////////////
#include  "def_application.h"

////////////////////////////////////////////////////////////////////////////////
// i/o function define
const IO_PIN_CONFIG tagRelayOutput[nIO_COUNT] = {
  {(GPIO_TypeDef*)GPIOD_BASE, GPIO_Pin_0 }, // 0
  {(GPIO_TypeDef*)GPIOD_BASE, GPIO_Pin_1 }, // 1 
  {(GPIO_TypeDef*)GPIOD_BASE, GPIO_Pin_2 }, // 2
  {(GPIO_TypeDef*)GPIOD_BASE, GPIO_Pin_3 }, // 3
  {(GPIO_TypeDef*)GPIOD_BASE, GPIO_Pin_4 }, // 4
  {(GPIO_TypeDef*)GPIOD_BASE, GPIO_Pin_5 }, // 5
  {(GPIO_TypeDef*)GPIOD_BASE, GPIO_Pin_6 }, // 6
  {(GPIO_TypeDef*)GPIOD_BASE, GPIO_Pin_7 }  // 7
};

const IO_PIN_CONFIG tagIsolaionInput[nIO_COUNT] = {
  {(GPIO_TypeDef*)GPIOE_BASE, GPIO_Pin_8  }, // 0
  {(GPIO_TypeDef*)GPIOE_BASE, GPIO_Pin_9  }, // 1 
  {(GPIO_TypeDef*)GPIOE_BASE, GPIO_Pin_10 }, // 2
  {(GPIO_TypeDef*)GPIOE_BASE, GPIO_Pin_11 }, // 3
  {(GPIO_TypeDef*)GPIOE_BASE, GPIO_Pin_12 }, // 4
  {(GPIO_TypeDef*)GPIOE_BASE, GPIO_Pin_13 }, // 5
  {(GPIO_TypeDef*)GPIOE_BASE, GPIO_Pin_14 }, // 6
  {(GPIO_TypeDef*)GPIOE_BASE, GPIO_Pin_15 }  // 7
};

void  SetMaskRelayOutput  ( u8 setmask ) { GPIOD->BSRR = (u16)setmask & 0x00ff; }
void  ResetMaskRelayOutput( u8 rstmask ) { GPIOD->BRR  = (u16)rstmask & 0x00ff; }
u8    GetRelayOutput( void )             { return (u8)(GPIOD->ODR & 0x00ff); }
u8    GetIsolationInput( void )          { return (u8)((GPIOE->IDR & 0xff00) >> 8); }

void IOPinConfig( IO_PIN_CONFIG nPort, DEF_IO_MODE mode )
{
  GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin		= nPort.nPIN;
  if( mode == MODE_INPUT ) {
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  } else {
    GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
  }
  nPort.nGPIO->ODR &=  ~nPort.nPIN;   // port reset "low output"
  
  GPIO_Init( nPort.nGPIO, &GPIO_InitStructure );
}

void SetOutput( IO_PIN_CONFIG nPort, DEF_IO_ACTION action )
{
  if( action == IO_LOW ) { nPort.nGPIO->BRR  = nPort.nPIN; }
  else { nPort.nGPIO->BSRR = nPort.nPIN; }
}

DEF_IO_ACTION CheckInput( IO_PIN_CONFIG nPort )
{
  if( nPort.nGPIO->IDR & nPort.nPIN ) { return IO_HIGH; } else { return IO_LOW; }
}

DEF_IO_ACTION CheckOutput( IO_PIN_CONFIG nPort )
{
  if( nPort.nGPIO->ODR & nPort.nPIN ) { return IO_HIGH; } else { return IO_LOW; }
}
