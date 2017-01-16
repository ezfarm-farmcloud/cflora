////////////////////////////////////////////////////////////////////////////////
//  Company           : 
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Revision          : V1.00
//  Project           : Complex Enviroment Control System 2015
//  Editor            : Hyunju Lee
//  Create Date       : 2015-06-10
//  File              : DEF_APPLICATION.H
////////////////////////////////////////////////////////////////////////////////
#ifndef __DEF_APPLICATION_H__
#define __DEF_APPLICATION_H__

#include  <assert.h>
#include  <math.h>
#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <string.h>
#include  <intrinsics.h>

#include  "stm32f10x_lib.h"
#include  "arm_comm.h"

////////////////////////////////////////////////////////////////////////////////
// application version define
#define   DEF_FW_VERSION  	    1.00f
#define   DEF_FW_DATE		  	    150610
#define   SYSTEM_CORE_CLOCK     72000000uL  // 72MHz
#define 	PI()  						    3.1415925f

////////////////////////////////////////////////////////////////////////////////
// BOOL define
#define		ON								TRUE
#define		OFF								FALSE

#define		HIGH							TRUE
#define		LOW								FALSE

#define		OK								TRUE
#define		NO								FALSE

#define		ACK						    TRUE
#define		NACK    					FALSE

#define		ACTIVE						TRUE
#define		NOACTIVE					FALSE

////////////////////////////////////////////////////////////////////////////////
// macro define
#define 	min(a, b)					(((a)>=(b))?(b):(a))
#define 	max(a, b)					(((a)>=(b))?(a):(b))
#define 	chkbit(reg, no)		((reg) & (0x0001<<(no)))
#define 	setbit(reg, no)		((reg) | (0x0001<<(no)))
#define 	clrbit(reg, no)		((reg) & ~(0x0001<<(no)))

////////////////////////////////////////////////////////////////////////////////
// global Regist Define
typedef union __tagGLOBAL_USER_REG__ {
	struct {
		u8		REGA;		// system status
		u8		REGB;
		u8		REGC;
		u8		REGD;		// screen info.
	};
	u32 uLREG;
} GLOBAL_UREG, *LPGLOBAL_UREG;
extern GLOBAL_UREG   m_gUserReg;
#define ClearGlobalUReg()			(m_gUserReg.uLREG = (u32)0x00000000)

// user registor define - system status
typedef union __tagSYSTEM_STATUS__ {
	struct {
			u8 bCanErr          : 1;
			u8 bCanEvent    	  : 1;
			u8 nreg             : 6;
	};
	u8	nReg;
} SYSTEM_STATUS, *LPSYSTEM_STATUS;
#define regSystemStatus				  ((LPSYSTEM_STATUS)&m_gUserReg.REGA)

#define	ClearCANError()         (regSystemStatus->bCanErr = 0)
#define	SetCANError()				    (regSystemStatus->bCanErr = 1)
#define	ChkCANError()				    (regSystemStatus->bCanErr)

#define	ClearCANEvent()         (regSystemStatus->bCanEvent = 0)
#define	SetCANEvent()				    (regSystemStatus->bCanEvent = 1)
#define	ChkCANEvent()				    (regSystemStatus->bCanEvent)


////////////////////////////////////////////////////////////////////////////////
// usart communication define
#define tagCOMM_TIMEOUT     250     // 250msec

#define tagCAN_GET_ATOD_LO  0x0000
#define tagCAN_GET_ATOD_HI  0x0100
#define tagCAN_OUTPUT_RELAY 0x0200
#define tagCAN_IN_ISOLATION 0x0300
#define tagCAN_GET_COUNTER  0x0400
#define tagCAN_ID_SCANNING  0x0700

// communication packet structure
#define	COMMPACKET_SIZE		    14
#define PACKET_HEADER			    (u16)0xaa55
#define NACK_PACKET_HEADER		(u16)0x55aa
typedef struct __tagCOMMPACKET__ {
	u16		nHeader;	// 0xaa55
	u16		wCanID;		// CANID_11_bits
	union	{
		u8		nData[8];
		u16		wData[4];
		u32		dwData[2];
	};
	u16		nChksum;	// wComm_b0..15 + wData[0..3]
} COMMPACKET, *LPCOMMPACKET;

#define 	UART_DATAQUE_SIZE   256
typedef struct _tagUART_DATAQUE_
{
  u16		nPushIndex;
  u16		nPopIndex;
  u8		nBuffer[UART_DATAQUE_SIZE];
} UART_DATAQUE, *LPUART_DATAQUE;

typedef union _tagUART_LINE_EVENT_
{
  u32 dwData;
  struct {
    u32  bORE        : 1;   // overrun error
    u32  bOE         : 1;   // overflow error
    u32  bPE         : 1;   // parity error
    u32  bFE         : 1;   // frame error
    u32  b5          : 28;
  };
} UART_LINE_EVENT, *LPUART_LINE_EVENT;

////////////////////////////////////////////////////////////////////////////////
// include file
#include  "evkit_adc.h"
#include  "evkit_io.h"
#include  "evkit_uart1.h"
#include  "evkit_uart3.h"
#include  "evkit_can.h"
#include  "evkit_counter3_4.h"
#include  "evkit_message.h"

///////////////////////////////////////////////////////////////////////////////
// function define
extern void MCUInit( void );
extern void SysTickConfig( void );

extern void ClearHostCommWatchDog( void );
extern u32  GetHostCommWatchDogCounter( void );
extern void ClearLocalWatchdog( void );

extern void SetDelayTime( u32 delay );
extern u32  CheckDelayTime( void );
extern void DecrementTimeDelay( void );
extern void Delay( u32 delay );

extern void RecievePacketParsing( LPCOMMPACKET pPacket );

#ifdef  DEBUG
	void assert_failed( u8 *file, unsigned int line );
#endif  // DEBUG
  
////////////////////////////////////////////////////////////////////////////////
// flash memory function
#define	FLASH_PAGE255_ADDR  (u32)0x807f800    // size 2048
extern BOOL FlashReadData ( void *pDataBuffer, u16 nSize );
extern BOOL FlashWriteData( void *pDataBuffer, u16 nSize );

#endif // __DEF_APPLICATION_H__
