#ifndef __COMMON_DEFINE__
#define __COMMON_DEFINE__

#define BYTE unsigned char
#define WORD unsigned short

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long

// Message packet buffer 
typedef struct 
{
	int    tx_index ; 
	int    rx_index ; 
	
	BYTE   buf[128] ;
	int    size ;
	
} MESSAGE_STRUCT ;


#define	COMMPACKET_SIZE		14
#define PACKET_HEADER			(u16)0xaa55
typedef struct __tagCOMMPACKET__ {
	u16		nHeader;	// 0xaa55
	u16		wCanID;		// __tagPCCOMM__, CANID_11_bits
	union	{
		u8		nData[8];
		u16		wData[4];
		u32		dwData[2];
//		u64		nLongLong;
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

#endif 