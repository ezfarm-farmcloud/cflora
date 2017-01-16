/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Company           : MesaMedical Co., Ltd.
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Microcontroller		: Cortex M4 STM32F407ZET6
//  Revision          : V1.00
//  Project           : Ultra Surgery 2013
//  Editor            : Hyunju Lee
//  Create Date       : 2013-07-30
//  Modify Date       : 2016-03-06
//  File              : M4_EVKIT_DEFINE.C
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "m4_evkit_define.h"

u32						CriticalSecCntr = 0;  // *** CriticalSecCntr 변수명 변경하지 말것.
GLOBAL_UREG   m_gUserReg;           // Grobal Flag Regist Define

////////////////////////////////////////////////////////////////////////////////
//	Vector Table base offset field. This value must be a multiple of 0x200.
#define VECT_TAB_OFFSET  0x00 

////////////////////////////////////////////////////////////////////////////////
//	PLL Parameters
#define		HSE_16MHZ					16
#define		HSE_25MHZ					25
#define		CLOCK_FREQUENCY		HSE_25MHZ

#if CLOCK_FREQUENCY == HSE_25MHZ
  #define PLL_M       25			// PLL_VCO = (HSE_VALUE / PLL_M) * PLL_N
#else
  #define PLL_M       16			// PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
#endif

#define PLL_N         336
#define PLL_P         2				// SYSCLK = PLL_VCO / PLL_P
#define PLL_Q         7				// USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ

__I u8 AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

static void SetSysClock(void)
{
  __IO u32 StartUpCounter = 0, HSEStatus = 0;
  
  RCC->CR |= ((u32)RCC_CR_HSEON);			// Enable HSE
  do {	// Wait till HSE is ready and if Time out is reached exit
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while( (HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT) );

  if( (RCC->CR & RCC_CR_HSERDY) != RESET ) {
    HSEStatus = (u32)0x01;
  } else { HSEStatus = (u32)0x00; }

  if( HSEStatus == (u32)0x01 ) {
    // Select regulator voltage output Scale 1 mode, System frequency up to 168 MHz
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;

    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;		// HCLK  = SYSCLK / 1		
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;		// PCLK2 = HCLK / 2
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;		// PCLK1 = HCLK / 4

    // Configure the main PLL
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);
    RCC->CR |= RCC_CR_PLLON;									// Enable the main PLL
    while( (RCC->CR & RCC_CR_PLLRDY) == 0 );	// Wait till the main PLL is ready
   
    // Configure Flash prefetch, Instruction cache, Data cache and wait state
    FLASH->ACR = FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;

    // Select the main PLL as system clock source
    RCC->CFGR &= (u32)((u32)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    // Wait till the main PLL is used as system clock source
    while ( (RCC->CFGR & (u32)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL );
  } else {
		// If HSE fails to start-up, the application will have wrong clock
    // configuration. User can add here some code to deal with this error
  }
}

void SystemInit(void)
{
  // FPU settings
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  // set CP10 and CP11 Full Access
  #endif

  // Reset the RCC clock configuration to the default reset state
  RCC->CR			|= (u32)0x00000001;		// Set HSION bit
  RCC->CFGR	 	 = 0x00000000;				// Reset CFGR register
	RCC->CR			&= (u32)0xFEF6FFFF;		// Reset HSEON, CSSON and PLLON bits	
  RCC->PLLCFGR = 0x24003010;				// Reset PLLCFGR register
  RCC->CR 		&= (u32)0xFFFBFFFF;		// Reset HSEBYP bit
  RCC->CIR		 = 0x00000000;				// Disable all interrupts

  // Configure the System clock source, PLL Multiplier and Divider factors, 
  // AHB/APBx prescalers and Flash settings
  SetSysClock();

  // Configure the Vector Table location add offset address
	#ifdef VECT_TAB_SRAM
  	SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET;	// Vector Table Relocation in Internal SRAM
	#else
  	SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; // Vector Table Relocation in Internal FLASH
	#endif

  NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

  //////////////////////////////////////////////////////////////////////////////
  //// GPIO & Clock & AF default initialize
	GPIO_DeInit(GPIOA);
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOD);
  GPIO_DeInit(GPIOE);
  GPIO_DeInit(GPIOF);
  GPIO_DeInit(GPIOG);
  GPIO_DeInit(GPIOH);
  GPIO_DeInit(GPIOI);
	
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE );
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE );
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE, ENABLE );
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOF, ENABLE );
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOG, ENABLE );

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SYSCFG, ENABLE );

	GPIOA->AFR[0] = GPIOA->AFR[1] = 0x00000000uL;
	GPIOB->AFR[0] = GPIOB->AFR[1] = 0x00000000uL;
	GPIOC->AFR[0] = GPIOC->AFR[1] = 0x00000000uL;
	GPIOD->AFR[0] = GPIOD->AFR[1] = 0x00000000uL;
	GPIOE->AFR[0] = GPIOE->AFR[1] = 0x00000000uL;
	GPIOF->AFR[0] = GPIOF->AFR[1] = 0x00000000uL;
	GPIOG->AFR[0] = GPIOG->AFR[1] = 0x00000000uL;
}

// SysTick & delay_function
u32   uLTimingDelay       = 0uL;
u32   uLSoundPatternCnt   = 0uL;
u32   uLSoundPatternTick  = 0uL;

void InitSysTick( u32 tick_count )
{
	uLSoundPatternTick = uLSoundPatternCnt = uLTimingDelay = 0uL;
	if( SysTick_Config( SYSTEM_CORE_CLOCK/tick_count ) ) { while (1); }
}

// SysTick_Handler
void ISR_SysTickHandler( void )
{ // system tick time 1msec
	if( uLTimingDelay > 0uL ) { uLTimingDelay--; }
}

void Delay( u32 delay )
{
	uLTimingDelay = delay;
	while( uLTimingDelay != 0uL );
}

void FlashErase( u32 nsector )
{ // FLASH_Sector_11
  FLASH_Unlock();

  // clear all pending flags
  FLASH_ClearFlag(  FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR \
                  | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR | FLASH_FLAG_BSY ); 
  FLASH_EraseSector( nsector, VoltageRange_3 );   // Device operating range: 2.7V to 3.6V

  FLASH_Lock();
}

// flash_function
// header & data save, generate checksum
bool FlashWriteData( u16 npage, void *pDataBuffer, u16 nSize )
{
  u16  i;
  u16  nChecksum  = 0;
  bool bRet       = true;
  u16  ntmpSize   = nSize;
  
  if( nSize % 2 ) { ntmpSize++; }
  
  char *writeBuffer = malloc( ntmpSize );
  writeBuffer[ntmpSize-1] = 0;
  
  memcpy( writeBuffer, pDataBuffer, ntmpSize );
  u16 *tmpBuffer = (u16*)writeBuffer;
  
  FLASH_Unlock();
 
  if(  FLASH_COMPLETE
    == FLASH_ProgramHalfWord(FLASH_Sector_11_ADDR+FLASH_DATA_PAGE_SIZE*npage, FLASH_DATA_PAGE_HAEDER) )
  {
    for( i=1; i<=(ntmpSize>>1); i++ ) {
      if(  FLASH_COMPLETE
        == FLASH_ProgramHalfWord(FLASH_Sector_11_ADDR+FLASH_DATA_PAGE_SIZE*npage+(i<<1), tmpBuffer[i-1]) ) { 
        nChecksum += tmpBuffer[i-1];
      } else { 
        bRet = false;
        break;
      }
    }
  } else { bRet = false; }

  // checksum save
  if( bRet ) {
    if( FLASH_COMPLETE != FLASH_ProgramHalfWord(FLASH_Sector_11_ADDR+FLASH_DATA_PAGE_SIZE*npage+ntmpSize+2, nChecksum) ) { 
      bRet = false;
    }
  }
  
  free( writeBuffer );
  FLASH_Lock();

  return bRet; 
}

// data load & check header, check checksum
bool FlashReadData( u16 npage, void *pDataBuffer, u16 nSize )
{
  u16   i, nSaveChksum;
  u16   nChecksum   = 0;
  u16   ntmpSize    = nSize;
  bool  bRet        = true;
  
  if( nSize % 2 ) { ntmpSize++; }
  
  char  *readBuffer = malloc( ntmpSize );
  u16   *tmpBuffer  = (u16*)readBuffer;
  
  if( *(vu16*)(FLASH_Sector_11_ADDR+FLASH_DATA_PAGE_SIZE*npage) == FLASH_DATA_PAGE_HAEDER ) {
    for( i=1; i<=(ntmpSize>>1); i++ ) {
      tmpBuffer[i-1] = *(vu16*)(FLASH_Sector_11_ADDR+FLASH_DATA_PAGE_SIZE*npage+(i<<1));
      nChecksum += tmpBuffer[i-1];
    }
    nSaveChksum = *(vu16*)(FLASH_Sector_11_ADDR+FLASH_DATA_PAGE_SIZE*npage+ntmpSize+2);
    
    if( nSaveChksum == nChecksum ) { 
      memcpy( pDataBuffer, readBuffer, nSize );
    } else { bRet = false; }
  } else { bRet = false; }
  
  free( readBuffer );
  
  return bRet;
}
