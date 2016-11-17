////////////////////////////////////////////////////////////////////////////////
//  Company           : 
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Revision          : 1.00
//  Project           : EVKIT 2014
//  Editor            : Hyunju Lee
//  Create Date       : 2014-05-30
//  File              : EVKIT_MESSAGE.H
////////////////////////////////////////////////////////////////////////////////
#ifndef __EVKIT_MESSAGE_H__
#define __EVKIT_MESSAGE_H__

// message define
#define		MSG_MASK					(u32)0xFFFF0000
#define		MSG_NONE					(u32)0x00000000
#define		MSG_KEY_INPUT			(u32)0x00010000
#define		MSG_ADC_COMP			(u32)0x00020000
#define		MSG_RTC_SECOND		(u32)0x00030000
#define		MSG_TOUCHSCREEN		(u32)0x00040000

#define		DEF_MESSAGE_CNT   32
#define		MESSAGE_SIZE      14
typedef struct __tagMESSAGE__ {
	union	{
    u32  hMsg;
		struct {
      u16 nMsgType;       // nHeader
      u16 nMsgSubType;    // wCanID
		};
  };
  
	union	{
		struct {
			union {
				u8	nldata[4];
				u16 wldata[2];
				u32	lparam;
				s32	nlparam;
			};
			union {
				u8	nhdata[4];
				u16 whdata[2];
				u32	hparam;
				s32	nhparam;
			};
		};
		u8			nData[8];
		u16 		wData[4];
		u32			dwData[2];
	};
  
	u16				nCheck;       // nChksum
} MESSAGE, *LPMESSAGE;

extern void   ClrMessage();
extern BOOL   IsMessage();
extern BOOL 	PushMessage( MESSAGE msg );
extern BOOL 	GetMessage( LPMESSAGE msg );

#endif // __EVKIT_MESSAGE_H__
