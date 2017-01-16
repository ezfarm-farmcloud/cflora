////////////////////////////////////////////////////////////////////////////////
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Microcontroller		: Cortex M4 STM32F407ZET6
//  Revision          : V1.00
//  Project           : CECS_M4 2016
//  Create Date       : 2016-11-30
//  File              : M4_EVKIT_DEFINE.H
////////////////////////////////////////////////////////////////////////////////
#ifndef __M4_EVKIT_DEFINE_H__
#define __M4_EVKIT_DEFINE_H__

#include "arm_comm.h"
#include "stm32f4xx.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define 	SYSTEM_CORE_CLOCK		  168000000uL		// 168MHz
#define 	DEF_PI()							3.1415925f

#define   MAX_ADC_VALUE         4095

#define   I2C_SPEED_100KHZ      100000
#define   I2C_SPEED_200KHZ      200000
#define   I2C_SPEED_400KHZ      400000
#define   I2C_CHK_DELAY         2500          // ~10msec

////////////////////////////////////////////////////////////////////////////////
// bool define
#define		ON								  true
#define		OFF								  false

#define		HIGH							  true
#define		LOW								  false

#define		OK								  true
#define		NO								  false

#define		ACTIVE						  true
#define		NOACTIVE					  false

////////////////////////////////////////////////////////////////////////////////
// macro define
#define 	min(a, b)					    (((a)>=(b))?(b):(a))
#define 	max(a, b)					    (((a)>=(b))?(a):(b))
#define 	chkbit(reg, no)		    ((reg) & (0x0001<<(no)))
#define 	setbit(reg, no)		    ((reg) | (0x0001<<(no)))
#define 	clrbit(reg, no)		    ((reg) & ~(0x0001<<(no)))

////////////////////////////////////////////////////////////////////////////////
// global Regist Define
typedef union __tagREGISTER__ {
	struct {
		u8 b0	: 1;
		u8 b1	: 1;
		u8 b2	: 1;
		u8 b3	: 1;
		u8 b4	: 1;
		u8 b5	: 1;
		u8 b6	: 1;
		u8 b7	: 1;
	};
	u8	nReg;
} REGISTER, *LPREGISTER;

typedef union __tagGLOBAL_USER_REG__ {
	struct {
		REGISTER		REGA;
		REGISTER		REGB;
		REGISTER		REGC;
		REGISTER		REGD;   // display & write screen info.
	};
	u32 uLREG;
} GLOBAL_UREG, *LPGLOBAL_UREG;
extern GLOBAL_UREG   m_gUserReg;
#define ClearGlobalReg()			(m_gUserReg.uLREG = 0x00000000uL)

typedef 	u16	COLORREF;

////////////////////////////////////////////////////////////////////////////////
// common header
#include "m4_evkit_message.h"
#include "m4_application.h"

// system initialize : startup에서 호출한다.
extern void SystemInit( void );

// SysTick & delay_function
extern void InitSysTick( u32 tick_count );
extern void Delay( u32 delay );
extern void ISR_SysTickHandler( void );

////////////////////////////////////////////////////////////////////////////////
// flash memory function
#define FLASH_Sector_11_ADDR        0x80E0000
#define FLASH_DATA_PAGE_SIZE        32
#define FLASH_DATA_PAGE_HAEDER      0xA55A

extern void FlashErase( u32 nsector );
extern bool FlashWriteData( u16 npage, void *pDataBuffer, u16 nSize );
extern bool FlashReadData( u16 npage, void *pDataBuffer, u16 nSize );

#endif // __M4_EVKIT_DEFINE_H__
