////////////////////////////////////////////////////////////////////////////////
//  Company           : MesaMedical Co., Ltd.
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Revision          : V1.00
//  Project           : MSTD 2013
//  Editor            : Hyunju Lee
//  Create Date       : 2013-05-30
//  Modify Date       : 2016-03-06
//  File              : M4_EVKIT_MESSAGE.H
////////////////////////////////////////////////////////////////////////////////
#ifndef __M4_EVKIT_MESSAGE_H__
#define __M4_EVKIT_MESSAGE_H__

////////////////////////////////////////////////////////////////////////////////
//	Message handler
//	message_structure
#define		DEF_MESSAGE_CNT				32
#define		MESSAGE_SIZE					14
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

#define		MSG_MASK							    (u32)0xFFFF0000
#define		MSG_NONE							    (u32)0x00000000

#define		MSG_KEY_INPUT					    (u32)0x00010000
#define		MSG_ADC_COMP					    (u32)0x00020000
#define		MSG_RTC_SECOND				    (u32)0x00030000
#define		MSG_TEST_TIMETICK			    (u32)0x00040000

#define		MSG_TOUCHSCREEN		        (u32)0x00050000
#define		MSG_TOUCHSCREEN_REPEAT		(u32)0x00060000
#define		MSG_TOUCHSCREEN_RELEASE		(u32)0x00070000

#define		MSG_SM_OCD		            (u32)0x00080000

extern void			ClearMessageHandle( void );
extern bool 		PushMessage( MESSAGE msg );
extern bool 		GetMessage( LPMESSAGE msg );

#endif	//  __M4_EVKIT_MESSAGE_H__