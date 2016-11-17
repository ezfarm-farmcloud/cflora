////////////////////////////////////////////////////////////////////////////////
//  Company           : 
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Revision          : V1.00
//  Project           : Complex Enviroment Control System 2015
//  Editor            : Hyunju Lee
//  Create Date       : 2015-06-10
//  File              : CECS_BASEBOARD.C
////////////////////////////////////////////////////////////////////////////////
#include  "def_application.h"

#define   tagHOST_COMM_WATCHDOG_CNT   600000uL

void main( void )
{
  MESSAGE     rxmsg;
  COMMPACKET  tmpPacket;
  
  #ifdef DEBUG
		debug();
	#endif

	ENTR_CRT_SECTION();
  MCUInit();              // init system clock 
  SysTickConfig();        // init system tick timer, tick_time = 1msec
	EXT_CRT_SECTION();

  // can_id configuration
  nAPPLICATION_CAN_ID = ConfigCanID();
  
  // adc_initialize
  ConfigADC();

  USART1_Config( 57600uL );
  USART3_Config( 57600uL );
  ConfigCAN();

  ADCTimerStart( ADC_Channel_0 );

  // message parsing
	while( 1 ) {
    if( GetMessage(&rxmsg) ) {
      if( rxmsg.nMsgType == PACKET_HEADER ) {
        memcpy( &tmpPacket, &rxmsg, COMMPACKET_SIZE );
        
        switch( rxmsg.nMsgSubType & 0xff00 ) {
          case tagCAN_GET_ATOD_LO :
            tmpPacket.wData[0] = tagAnalogInputValue[ADC_Channel_0];
            tmpPacket.wData[1] = tagAnalogInputValue[ADC_Channel_1];
            tmpPacket.wData[2] = tagAnalogInputValue[ADC_Channel_2];
            tmpPacket.wData[3] = tagAnalogInputValue[ADC_Channel_3];
            CANTrasmitMessage( &tmpPacket );
            break;

          case tagCAN_GET_ATOD_HI :
            tmpPacket.wData[0] = tagAnalogInputValue[ADC_Channel_4];
            tmpPacket.wData[1] = tagAnalogInputValue[ADC_Channel_5];
            tmpPacket.wData[2] = tagAnalogInputValue[ADC_Channel_6];
            tmpPacket.wData[3] = tagAnalogInputValue[ADC_Channel_7];
            CANTrasmitMessage( &tmpPacket );
            break;

          case tagCAN_ID_SCANNING :
            CANTrasmitMessage( &tmpPacket );
            break;
            
          default : break;
        }
      }
    }
  }
}

void RecievePacketParsing( LPCOMMPACKET pPacket ) {}

////////////////////////////////////////////////////////////////////////////////
// flash memory function
// using FLASH_PAGE255_ADDR
BOOL FlashReadData( void *pDataBuffer, u16 nSize )
{
  char  *readBuffer;
  u16   i, nSaveChksum;
  u16   nChecksum   = 0;
  u16   ntmpSize    = nSize;
  
  if( nSize % 2 ) { ntmpSize++; }
  
  readBuffer     = malloc( ntmpSize );
  u16 *tmpBuffer = (u16*)readBuffer;
  
  for( i=0; i<(ntmpSize>>1); i++ ) {
    tmpBuffer[i] = *(vu16*)(FLASH_PAGE255_ADDR+(i<<1));
    nChecksum += tmpBuffer[i];
  }
  nSaveChksum = *(vu16*)(FLASH_PAGE255_ADDR+ntmpSize);
  
  memcpy( pDataBuffer, readBuffer, nSize ) ;
  free( readBuffer );

  if( nSaveChksum != nChecksum ) { return FALSE; }
  
  return TRUE;
}

BOOL FlashWriteData( void *pDataBuffer, u16 nSize )
{
  char *writeBuffer;
  u16  i;
  u16  nChecksum  = 0;
  u16  ntmpSize   = nSize;
  BOOL bRet       = TRUE;

  if( nSize % 2 ) { ntmpSize++; }
  
  writeBuffer = malloc( ntmpSize );
  writeBuffer[ntmpSize-1] = 0;
  
  memcpy( writeBuffer, pDataBuffer, nSize );
  u16 *tmpBuffer = (u16*)writeBuffer;
  
  FLASH_Unlock();

  // clear all pending flags
  FLASH_ClearFlag( FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR ); 
  FLASH_ErasePage( FLASH_PAGE255_ADDR );
  
  for( i=0; i<(ntmpSize>>1); i++ ) {
    if( FLASH_COMPLETE == FLASH_ProgramHalfWord(FLASH_PAGE255_ADDR+(i<<1), tmpBuffer[i]) ) { 
      nChecksum += tmpBuffer[i];
    } else { 
      bRet = FALSE;
      break;
    }
  }

  // checksum save
  if( bRet ) {
    if( FLASH_COMPLETE != FLASH_ProgramHalfWord(FLASH_PAGE255_ADDR+ntmpSize, nChecksum) ) { 
      bRet = FALSE;
    }
  }
  free( writeBuffer );
  FLASH_Lock();

  return bRet; 
}

////////////////////////////////////////////////////////////////////////////////
//*** CriticalSecCntr 변수명 변경하지 말것.
unsigned int	CriticalSecCntr = 0;

////////////////////////////////////////////////////////////////////////////////
// Grobal Flag Regist Define
GLOBAL_UREG   m_gUserReg;

////////////////////////////////////////////////////////////////////////////////
// Function Name: MCUInit
// Parameters   : none
// Return       : none       
// Description  : Init system clock
#define		MASTER_CLOCK_OUT_TEST_SYSCLK		0
void MCUInit( void )
{
  RCC_DeInit();

  #ifdef __HSE__
    RCC_HSEConfig( RCC_HSE_ON );
    while( RCC_WaitForHSEStartUp() != SUCCESS );              // wait until the HSE is ready
  #endif

  #ifdef __HSE__	// HSI = 16MHz
    RCC_PLLConfig( RCC_PLLSource_HSE_Div2, RCC_PLLMul_9 );    // Init PLL (16/2) * 9 = 72MHz
  #else   				// HSI = 8MHz
    RCC_PLLConfig( RCC_PLLSource_HSI_Div2, RCC_PLLMul_16 );   // Init PLL 64MHz
  #endif

    RCC_PLLCmd( ENABLE );
    while( RCC_GetFlagStatus( RCC_FLAG_PLLRDY ) == RESET );   // wait until the PLL is ready

    RCC_HCLKConfig	( RCC_SYSCLK_Div1 );											// 72Mhz
    RCC_PCLK2Config	( RCC_HCLK_Div1 );    										// 72Mhz
    RCC_PCLK1Config	( RCC_HCLK_Div2 );												// 36Mhz

  #ifdef EMB_FLASH
    // Init Embedded Flash
    // Zero wait state,     if 0      < HCLK 24 MHz
    // One wait state,      if 24 MHz < HCLK 56 MHz
    // Two wait states,     if 56 MHz < HCLK 72 MHz
    // Flash wait state
    FLASH_SetLatency( FLASH_Latency_2 );
    FLASH_HalfCycleAccessCmd( FLASH_HalfCycleAccess_Disable );// Half cycle access
    FLASH_PrefetchBufferCmd( FLASH_PrefetchBuffer_Enable );		// Prefetch buffer
  #endif

	RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK );							  // Wait till PLL is used as system clock sourc
	while( RCC_GetSYSCLKSource() != 0x08 );

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOF, ENABLE );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOG, ENABLE );
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO,  ENABLE );     // remap

  #if MASTER_CLOCK_OUT_TEST_SYSCLK
    // MOC pin : PA08_pin_num_100
    GPIO_InitTypeDef	GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8; 
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    
    //	#define RCC_MCO_NoClock                  ((u8)0x00)
    //	#define RCC_MCO_SYSCLK                   ((u8)0x04)
    //	#define RCC_MCO_HSI                      ((u8)0x05)
    //	#define RCC_MCO_HSE                      ((u8)0x06)
    //	#define RCC_MCO_PLLCLK_Div2              ((u8)0x07)
    RCC_MCOConfig( RCC_MCO_SYSCLK );
    while(1);
  #else
    RCC_MCOConfig( RCC_MCO_NoClock );
  #endif

  // Function Name  : NVIC_Configuration
  // Description    : Configures Vector Table base location.
  NVIC_DeInit();
  #ifndef  EMB_FLASH
    NVIC_SetVectorTable( NVIC_VectTab_RAM, 0 );               // VECT_TAB_RAM		0x2000 0000
  #else	// current mode               
    NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0 );             // VECT_TAB_FLASH 0x0800 0000
  #endif
  NVIC_PriorityGroupConfig( NVIC_PriorityGroup_0 );

  ClearGlobalUReg();
}

////////////////////////////////////////////////////////////////////////////////
// systick & delay function
static u32 uLTimingDelay              = 0uL;
static u32 uLHostCommWatchdogCounter  = 0uL;

void SysTickConfig( void )
{
  // SysTick end of count event each 1ms with input clock equal to 9MHz
  // default : HCLK/8 = 9MHz
  SysTick_SetReload( 9000uL );
  // Enable SysTick interrupt
  SysTick_ITConfig( ENABLE );

  // Enable the SysTick Counter
  SysTick_CounterCmd( SysTick_Counter_Enable );
}

void ClearHostCommWatchDog( void )      { uLHostCommWatchdogCounter = 0uL; }
u32  GetHostCommWatchDogCounter( void ) { return uLHostCommWatchdogCounter; }
void ClearLocalWatchdog( void )         { }

void SetDelayTime( u32 delay )  { uLTimingDelay = delay; }
u32  CheckDelayTime( void )     { return uLTimingDelay; }
void DecrementTimeDelay( void )
{
	if( uLTimingDelay > 0uL ) { uLTimingDelay--; }
  uLHostCommWatchdogCounter++;
}

void Delay( u32 delay )
{
	uLTimingDelay = delay;
	while( uLTimingDelay != 0uL );
}

////////////////////////////////////////////////////////////////////////////////
// Function Name:   assert_failed
// Parameters:      - file: pointer to the source file name
//                  - line: assert_param error line source number
// Return:          none
// Description:     Reports the name of the source file and the source line number
//                  where the assert_param error has occurred.
#ifdef  DEBUG
void assert_failed( u8 *file, unsigned int line )
{
  volatile BOOL Flag = TRUE;
  // User can add his own implementation to report the file name and line number,
  // ex: printf("Wrong parameters value: file %s on line %d\r", file, line)

  while( Flag ) {} // Infinite loop
}
#endif  // DEBUG
