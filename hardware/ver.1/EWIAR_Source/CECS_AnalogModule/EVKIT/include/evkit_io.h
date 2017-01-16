////////////////////////////////////////////////////////////////////////////////
//  Company           : 
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Revision          : 1.00
//  Project           : EVKIT 2014
//  Editor            : Hyunju Lee
//  Create Date       : 2014-05-30
//  File              : EVKIT_IO.H
////////////////////////////////////////////////////////////////////////////////
#ifndef __EVKIT_IO__
#define __EVKIT_IO__

////////////////////////////////////////////////////////////////////////////////
// IO define
typedef struct __IO_PIN_CONFIG__ {
  GPIO_TypeDef*   nGPIO;
  u16             nPIN;
} IO_PIN_CONFIG, *PIO_PIN_CONFIG;

typedef enum __DEF_IO_MODE__   { MODE_INPUT = 0, MODE_OUTPUT = 1 } DEF_IO_MODE;
typedef enum __DEF_IO_ACTION__ { IO_LOW = 0, IO_HIGH  = 1 } DEF_IO_ACTION;

#define       nIO_COUNT   8
extern const  IO_PIN_CONFIG tagRelayOutput[nIO_COUNT];
extern const  IO_PIN_CONFIG tagIsolaionInput[nIO_COUNT];

extern void   SetMaskRelayOutput  ( u8 setmask );
extern void   ResetMaskRelayOutput( u8 rstmask );
extern u8     GetRelayOutput      ( void );
extern u8     GetIsolationInput   ( void );

////////////////////////////////////////////////////////////////////////////////
// i/o function define
extern void IOPinConfig ( IO_PIN_CONFIG nPort, DEF_IO_MODE   mode );
extern void SetOutput   ( IO_PIN_CONFIG nPort, DEF_IO_ACTION action );
extern DEF_IO_ACTION CheckInput  ( IO_PIN_CONFIG nPort );
extern DEF_IO_ACTION CheckOutput ( IO_PIN_CONFIG nPort );

#endif // __EVKIT_IO__
