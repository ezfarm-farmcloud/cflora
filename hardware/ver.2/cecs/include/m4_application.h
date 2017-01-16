////////////////////////////////////////////////////////////////////////////////
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Microcontroller		: Cortex M4 STM32F407ZET6
//  Revision          : 1.00
//  Project           : CECS_M4 2016
//  Create Date       : 2016-11-30
//  File              : M4_APPCATION.H
////////////////////////////////////////////////////////////////////////////////
#ifndef __M4_APPCATION_H__
#define __M4_APPCATION_H__

#define   DEF_FW_VERSION  	    1.00f
#define   DEF_FW_DATE		  	    161117
#define   DEF_NONE_EVENT  	    -1
#define   DEF_LT_MODE  	        0

typedef enum  __ENUM_TRI_STATUS__ {
  ENUM_STS_DISABLE = 0,
  ENUM_STS_ENABLE,
  ENUM_STS_ACTIVE
} ENUM_TRI_STATUS;

// user registor define - system status
typedef union __tagSYSTEM_STATUS__ {
	struct {
    u8 bStart         : 1;  // lsb
    u8 bCanEvent 	    : 1;
    u8 bCanError 	    : 1;
    u8 bCanTimeOut    : 1;
    u8 bNotUsed  	    : 4;  // msb
	};
	u8  nReg;
} SYSTEM_STATUS, *LPSYSTEM_STATUS;
#define regSystemStatus     		  ((LPSYSTEM_STATUS)&m_gUserReg.REGA)

#define	OperationStart()          (regSystemStatus->bStart = 1)
#define	OperationStop()				    (regSystemStatus->bStart = 0)
#define	DoStart()					        (regSystemStatus->bStart)

#define	SetCANEvent()             (regSystemStatus->bCanEvent = 1)
#define	ClearCANEvent()				    (regSystemStatus->bCanEvent = 0)
#define	CheckCANEvent()					  (regSystemStatus->bCanEvent)

#define	SetCANError()             (regSystemStatus->bCanError = 1)
#define	ClearCANError()				    (regSystemStatus->bCanError = 0)
#define	CheckCANError()					  (regSystemStatus->bCanError)

#define	SetCanTimeOut()				    (regSystemStatus->bCanTimeOut = 1)
#define	ClearCanTimeOut()         (regSystemStatus->bCanTimeOut = 0)
#define	CheckCanTimeOut()			    (regSystemStatus->bCanTimeOut)

// communication packet structure
#define tagCOMM_TIMEOUT       250     // 250msec

#define tagCAN_GET_ATOD_1ST   0x0000
#define tagCAN_GET_ATOD_2ND   0x0100
#define tagCAN_OUTPUT_RELAY   0x0200
#define tagCAN_IN_ISOLATION   0x0300
#define tagCAN_GET_COUNTER    0x0400
#define tagCAN_GET_ATOD_3RD   0x0500
#define tagCAN_GET_ATOD_4TH   0x0600
#define tagCAN_ID_SCANNING    0x0700

#define	COMMPACKET_SIZE		    14
#define ACK_PACKET_HEADER			(u16)0xaa55
#define NACK_PACKET_HEADER		(u16)0x55aa
typedef struct __tagCOMMPACKET__ {
	u16		nHeader;	// ACK_PACKET_HEADER or NACK_PACKET_HEADER
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

extern COMMPACKET  RxPacket, TxPacket;

extern void ProcedureModuleBase( u16 nTypeID );
extern u16  GetTypeID( void );

#endif // __M4_APPCATION_H__
