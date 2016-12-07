
#include "common.h"
#include "TTA12_packet.h"
#include "util.h"



HWCONFIG SENSOR_MAP[MAX_SENSOR_COUNT] ;
HWCONFIG ACTUATOR_MAP[MAX_ACTUATOR_COUNT] ;

int  SensorInterval =60 ;
int  ActuatorInterval=3 ;
int  ISOInterval=3;

int CONTROL_PORT=1 ;  //���� ������ �ø��� ��Ʈ
int PORT_SPEED=115200 ;  //���� ������ �ø��� ��Ʈ ���ǵ�
int HouseControl_ID = 0 ;

char GCG_IP[128] ;
int GCG_PORT=5000 ;

// 2016�� 6�� �߰�
int INSTALL_COMM_SENSOR_COUNT=1;
COMM_SENSOR_STRUCT COMM_SENSOR[MAX_COMM_SENSOR] ;


void PacketVariable_init(void)
{
	InstalledSensorCount=24;
	InstalledActuatorCount=8;
	InstalledSensorNodeCount=3;
	InstalledActuatorNodeCount=1;

	for( int i=0; i < MAX_SNODE_COUNT ;i++)
	{
		SensorNodeStatus[i].SW_VER =0x01 ;
		SensorNodeStatus[i].PROFILE_VER=0x01 ;
		SensorNodeStatus[i].CONTROL_ID=0x00 ;
		SensorNodeStatus[i].NODE_ID=i;
		SensorNodeStatus[i].ISINIT_NODE=0;
		SensorNodeStatus[i].MONITOR_MODE=0;
		SensorNodeStatus[i].MONITOR_TIME=2;
		SensorNodeStatus[i].SENACT_NUM = 8 ;
		SensorNodeStatus[i].COMM_ERROR_NUM=0;
		SensorNodeStatus[i].SERVICE_ERROR_NUM=0;

	}
	for( int i=0; i < MAX_ANODE_COUNT ;i++)
	{
		AcutuatorNodeStatus[i].SW_VER =0x01 ;
		AcutuatorNodeStatus[i].PROFILE_VER=0x01 ;
		AcutuatorNodeStatus[i].CONTROL_ID=0x00 ;
		AcutuatorNodeStatus[i].NODE_ID=i;
		AcutuatorNodeStatus[i].ISINIT_NODE=0;
		AcutuatorNodeStatus[i].MONITOR_MODE=0;
		AcutuatorNodeStatus[i].MONITOR_TIME=2;
		AcutuatorNodeStatus[i].SENACT_NUM = 8 ;
		AcutuatorNodeStatus[i].COMM_ERROR_NUM=0;
		AcutuatorNodeStatus[i].SERVICE_ERROR_NUM=0;

	}

	for( int i=0; i < MAX_SENSOR_COUNT ;i++)
	{
		SI[i].NODE_ID =i/8 ;
		SI[i].data.id =i ;
		SI[i].data.type =0x00 ;
		SI[i].data.value =i;
		SI[i].state=0;
		SI[i].isinit=1;
		SI[i].mode=REQCMD_PASSIVE_MODE ;//REQCMD_ACTIVE_MODE;
		SI[i].time=SensorInterval;

	}
	for( int i=0; i < MAX_ACTUATOR_COUNT ;i++)
	{
		AI[i].NODE_ID =i/8 ;
		AI[i].data.id =i ;
		AI[i].data.type =0x00 ;
		AI[i].data.value =0;
		AI[i].state=0;
		AI[i].isinit=1;
		AI[i].mode=0;
		AI[i].time=0;

	}

}

int ReadDevice_Config(void)
{
    dictionary  *   ini ;

    /* Some temporary variables to hold query results */
    int             b ;
	int             gdata,typedata;
    int             i ;
//    double          d ;
    char						*s ;
	char             sbuf[128] ;
	char             gbuf[128] ;
//	char						*sCH ;


   //ini = iniparser_load((char*)"../../conf/cflora-device.ini");
   //ini = iniparser_load((char*)"cflora-device.ini");
   ini = iniparser_load((char*)"../conf/cflora-device.ini");
    if (ini==NULL) {
        fprintf(stderr, "cannot parse file: cflora-device.ini\n");
        return -1 ;
    }
  //  iniparser_dump(ini, stderr);

   InstalledSensorNodeCount = iniparser_getint(ini, "GENERAL:SENSOR-NODE-COUNT", 0);
	 InstalledActuatorNodeCount = iniparser_getint(ini, "GENERAL:ACTUATOR-NODE-COUNT", 0);

   SensorInterval = iniparser_getint(ini, "GENERAL:SENSOR-INTERVAL", 60);
	 ActuatorInterval = iniparser_getint(ini, "GENERAL:ACTUATOR-INTERVAL", 3);
   ISOInterval = iniparser_getint(ini, "GENERAL:ISO-INTERVAL", 3);

	 //CONTROL_PORT = iniparser_getint(ini, "GENERAL:CONTROL-SERIAL-PORT", 0 );
	 //PORT_SPEED = iniparser_getint(ini, "GENERAL:SERIAL-PORT-SPEED", 115200 );
	 COMM_SENSOR[0].PORT = 0; //iniparser_getint(ini, "GENERAL:CONTROL-SERIAL-PORTID", 0 );
	 COMM_SENSOR[0].SPEED = iniparser_getint(ini, "GENERAL:SERIAL-PORT-SPEED", 115200 );
	 strcpy (COMM_SENSOR[0].TTY, iniparser_getstring(ini, "GENERAL:CONTROL-SERIAL-TTY", "/dev/ttyS1"));

	 HouseControl_ID = iniparser_getint(ini, "GENERAL:HOUSECONTROL_ID", 0 );

	 printf( "InstalledSensorNodeCount: %d\n",InstalledSensorNodeCount ) ;
	 printf( "InstalledActuatorNodeCount: %d\n",InstalledActuatorNodeCount ) ;


/*
		s = iniparser_getstring(ini, "config:auto", NULL);
    s = iniparser_getstring(ini, "config:voip_server", NULL);
*/
/*
	WORD NODE_ID ;  //������ȣ
	BYTE CH ; // ������ ä��
	char ID ; // ���� ���̵�
	char ARG ;
	BYTE INIT_TIME ; // �ʱ�ȭ�� ON ���� �ð�
	u32  SET_INTERVAL;  // ON ���� �ð�(�ʴ���)  0xFF : �׻� ON  0x00 : OFF
	u32  COUNT;   // �ٿ�ī��Ʈ(�ʴ���)
*/

		for( b = 0 ; b < InstalledSensorNodeCount*8 ; b++)
		{
			SENSOR_MAP[b].NODE_ID = 0 ;
			SENSOR_MAP[b].CH = b%8 ;
			SENSOR_MAP[b].ID = 0 ;
			SENSOR_MAP[b].ARG = 0 ;
			SENSOR_MAP[b].INIT_TIME = 0 ;
			SENSOR_MAP[b].TYPE = 0 ;
			SENSOR_MAP[b].CANID = 0 ;

			SENSOR_MAP[b].SET_INTERVAL=SensorInterval ;
			SENSOR_MAP[b].COUNT=SensorInterval;
		}

		for( b = 0 ; b < InstalledActuatorNodeCount*8 ; b++)
		{
			ACTUATOR_MAP[b].NODE_ID = 0 ;
			ACTUATOR_MAP[b].CH = b%8 ;
			ACTUATOR_MAP[b].ID = 0 ;
			ACTUATOR_MAP[b].ARG = 0 ;
			ACTUATOR_MAP[b].INIT_TIME = 0 ;
			ACTUATOR_MAP[b].TYPE = 0 ;
			ACTUATOR_MAP[b].CANID = 0 ;

			ACTUATOR_MAP[b].SET_INTERVAL=ActuatorInterval ;
			ACTUATOR_MAP[b].COUNT=0;
		}


	for( b= 0 ; b < InstalledSensorNodeCount ; b++)
	{
		for( i= 0 ; i < 8 ; i++) // ������ ä���� 8��
		{
			SENSOR_MAP[b*8+i].NODE_ID = b;
			sprintf( sbuf , "SENSOR_NODE_%d:NODETYPE",b ) ;
			typedata = iniparser_getint( ini, sbuf , -1 ) ;
			SENSOR_MAP[b*8+i].TYPE = typedata ;

			sprintf( sbuf , "SENSOR_NODE_%d:CANID",b ) ;
			s = iniparser_getstring(ini, sbuf, NULL);
		  	if( s != NULL ) {
				strcpy( gbuf,s ) ;
				SENSOR_MAP[b*8+i].CANID = (int)strtol(gbuf, NULL, 0);

			}	else 
				SENSOR_MAP[b*8+i].CANID = 0x0fff ;

			printf(" NODE %d CANID %x\n", b, SENSOR_MAP[b*8+i].CANID ) ;

			if( typedata == 1 ) // ADC
			{
				SENSOR_MAP[b*8+i].SET_INTERVAL=SensorInterval ;
				SENSOR_MAP[b*8+i].COUNT=SensorInterval;
			}
			else if( typedata == 2 ) // ISO
			{
				SENSOR_MAP[b*8+i].SET_INTERVAL=ISOInterval ;
				SENSOR_MAP[b*8+i].COUNT=ISOInterval;
			}
			else if( typedata == 3 ) // COUNTER
			{
				SENSOR_MAP[b*8+i].SET_INTERVAL=SensorInterval ;
				SENSOR_MAP[b*8+i].COUNT=SensorInterval;
			}

			sprintf( sbuf , "SENSOR_NODE_%d:CH%02d_ID",b,i ) ;
			gdata = iniparser_getint( ini, sbuf , -1 ) ;
			if( gdata == -1 ) {
				SI[b*8+i].data.id = SENSOR_MAP[b*8+i].ID = 255;
			} else {
				SI[b*8+i].data.id = SENSOR_MAP[b*8+i].ID = gdata ;
				SI[b*8+i].mode=REQCMD_ACTIVE_MODE;
			}

			if( typedata == 10 ) // COMMUNICATION PORT SENSOR ( RS232,485 )
			{
				printf ("comm sensor [%d, %d, %d]\n", b, i + 1, SI[b*8+i].data.id);
				SENSOR_MAP[b*8+i].SET_INTERVAL=SensorInterval ;
				SENSOR_MAP[b*8+i].COUNT=SensorInterval;

				if(i > 0) {
                    if (SI[b*8+i].data.id == 255)
						break;
					COMM_SENSOR[SENSOR_MAP[b*8].ARG].SENSOR_CNT= i + 1;
					printf ("comm sensor [%d]\n", i + 1);
				} else {
					if (INSTALL_COMM_SENSOR_COUNT <= MAX_COMM_SENSOR )  {
						INSTALL_COMM_SENSOR_COUNT++; // 2ä�α��� ����
					} else {
						printf ("Exceed max number of serial sensor nodes\n");
						break;
					}
					printf ("INSTALL_COMM_SENSOR_COUNT [%d]\n", INSTALL_COMM_SENSOR_COUNT);

					SENSOR_MAP[b*8+i].ARG =	INSTALL_COMM_SENSOR_COUNT - 1;

					sprintf( sbuf , "SENSOR_NODE_%d:PORTID",b ) ;
					typedata = iniparser_getint( ini, sbuf , INSTALL_COMM_SENSOR_COUNT ) ;
					COMM_SENSOR[SENSOR_MAP[b*8+i].ARG].PORT = typedata ;

					sprintf( sbuf , "SENSOR_NODE_%d:SPEED",b ) ;
					typedata = iniparser_getint( ini, sbuf , -1 ) ;
					COMM_SENSOR[SENSOR_MAP[b*8+i].ARG].SPEED = typedata ;

					sprintf( sbuf , "SENSOR_NODE_%d:LEN",b ) ;
					typedata = iniparser_getint( ini, sbuf , -1 ) ;
					COMM_SENSOR[SENSOR_MAP[b*8+i].ARG].LEN = typedata ;

					sprintf( sbuf , "SENSOR_NODE_%d:EXP",b ) ;
					s = iniparser_getstring(ini, sbuf, NULL);
					if( s != NULL ) {
						strcpy( COMM_SENSOR[SENSOR_MAP[b*8+i].ARG].EXP,s ) ;
					} else { 
						strcpy( COMM_SENSOR[SENSOR_MAP[b*8+i].ARG].EXP,"[[:digit:]]+" ) ;
					}

					sprintf( sbuf , "SENSOR_NODE_%d:TTY",b ) ;
					s = iniparser_getstring(ini, sbuf, NULL);
					if( s != NULL ) {
						strcpy( COMM_SENSOR[SENSOR_MAP[b*8+i].ARG].TTY,s ) ;
					} else {
						strcpy( COMM_SENSOR[SENSOR_MAP[b*8+i].ARG].TTY,"/dev/ttyS0" ) ;
					}

					sprintf( sbuf , "SENSOR_NODE_%d:ENCODE",b ) ;
					typedata = iniparser_getint( ini, sbuf , 1 ) ;
					COMM_SENSOR[SENSOR_MAP[b*8+i].ARG].ENCODE = typedata ;

					COMM_SENSOR[SENSOR_MAP[b*8+i].ARG].SENSOR_NODE=b;
					COMM_SENSOR[SENSOR_MAP[b*8+i].ARG].SENSOR_CNT=1;

					printf("COMM PORT SENSOR INSTALLED INFO [%d] ------------- \n PORT = %d, TTY = %s \n SPEED= %d\n LEN=%d\n EXP=%s,ENCODE=%d\n",
							SENSOR_MAP[b*8+i].ARG,
							COMM_SENSOR[SENSOR_MAP[b*8+i].ARG].PORT,
							COMM_SENSOR[SENSOR_MAP[b*8+i].ARG].TTY,
							COMM_SENSOR[SENSOR_MAP[b*8+i].ARG].SPEED,
							COMM_SENSOR[SENSOR_MAP[b*8+i].ARG].LEN,
							COMM_SENSOR[SENSOR_MAP[b*8+i].ARG].EXP,
							COMM_SENSOR[SENSOR_MAP[b*8+i].ARG].ENCODE

					) ;
				}
			}

			//printf("%s:%d\n",sbuf,SENSOR_MAP[b*8+i].ID  ) ;
		}
	}

	for( b= 0 ; b < InstalledActuatorNodeCount ; b++)
	{
		for( i= 0 ; i < 8 ; i++) // ������ ä���� 8��
		{
			ACTUATOR_MAP[b*8+i].NODE_ID = b;

			sprintf( sbuf , "ACTUATOR_NODE_%d:CANID",b ) ;
			s = iniparser_getstring(ini, sbuf, NULL);
			ACTUATOR_MAP[b*8+i].TYPE = 5 ;
			if( s != NULL )
			{
			strcpy( gbuf,s ) ;
			ACTUATOR_MAP[b*8+i].CANID = (int)strtol(gbuf, NULL, 0);

			} else ACTUATOR_MAP[b*8+i].CANID = 0xfff;
			printf(" NODE %d CANID %x\n", b, ACTUATOR_MAP[b*8+i].CANID) ;


			sprintf( sbuf , "ACTUATOR_NODE_%d:CH%02d_ID",b,i ) ;
			gdata = iniparser_getint( ini, sbuf , -1 ) ;

			if( gdata == -1 )
			AI[b*8+i].data.id = ACTUATOR_MAP[b*8+i].ID = 255 ;
			else
			AI[b*8+i].data.id = ACTUATOR_MAP[b*8+i].ID = gdata ;

			sprintf( sbuf , "ACTUATOR_NODE_%d:CH%02d_ARG",b,i ) ;
			ACTUATOR_MAP[b*8+i].ARG = iniparser_getint( ini, sbuf , 0 ) ;

			sprintf( sbuf , "ACTUATOR_NODE_%d:CH%02d_INIT",b,i ) ;
			ACTUATOR_MAP[b*8+i].INIT_TIME = iniparser_getint( ini, sbuf , 0 ) ;

			sprintf( sbuf , "ACTUATOR_NODE_%d:CH%02d_MAXTIME",b,i ) ;
			ACTUATOR_MAP[b*8+i].SET_INTERVAL = iniparser_getint( ini, sbuf , 0 ) ;

		}
	}


/*
    ini = iniparser_load(ini_name);
    if (ini==NULL) {
        fprintf(stderr, "cannot parse file: %s\n", ini_name);
        return -1 ;
    }
    //iniparser_dump(ini, stderr);

    b = iniparser_getint(ini, "config:auto", -1);
		printf("AUTO : %d\n",b ) ;
    s = iniparser_getstring(ini, "config:auto", NULL);
    printf("NAME:  [%s]\n", s ? s : "UNDEF");

    s = iniparser_getstring(ini, "config:voip_server", NULL);
    printf("server ip:  [%s]\n", s ? s : "UNDEF");

    i = iniparser_getint(ini, "wine:year", -1);
    printf("Year:      [%d]\n", i);

    d = iniparser_getdouble(ini, "wine:alcohol", -1.0);
    printf("Alcohol:   [%g]\n", d);
*/
    iniparser_freedict(ini);
    return 0 ;
}


int ReadServer_Config(void)
{
    dictionary  *   ini ;

    /* Some temporary variables to hold query results */
//    int             b ;
//    int             i ;
//    double          d ;
    char						*s ;
//		char             sbuf[128] ;
//		char						*sCH ;


    ini = iniparser_load((char*)"../conf/cflora-server.ini");
    //ini = iniparser_load((char*)"../../conf/cflora-server.ini");
		//ini = iniparser_load((char*)"cflora-server.ini");
    if (ini==NULL) {
        fprintf(stderr, "cannot parse file: cflora-server.ini\n");
        return -1 ;
    }
  //  iniparser_dump(ini, stderr);

		s = iniparser_getstring(ini, "GCG:IP", NULL);
		//memset( GCG_IP,0x00,128 ) ;
		strcpy( GCG_IP,s ) ;
		GCG_PORT = iniparser_getint(ini, "GCG:PORT", 5000);

		printf("GCG INFO %s : %d\n",GCG_IP,GCG_PORT ) ;
    iniparser_freedict(ini);
    return 0 ;
}


int GetSensorConfigIndex( char getID )
{
		int b;
		for( b = 0 ; b < InstalledSensorNodeCount*8 ; b++)
		{
			if( SENSOR_MAP[b].ID == getID ) return b ;
		}

		printf( "Not define sensor index (config) : id %d \n",getID ) ;
		return -1;
}

int GetActuatorConfigIndex( char getID ,BYTE arg )
{
		int b;
		for( b = 0 ; b < InstalledActuatorNodeCount*8 ; b++)
		{
			if( ACTUATOR_MAP[b].ID == getID && ACTUATOR_MAP[b].ARG == arg ) return b ;
		}
		printf( "Not define Actuator index (config) : id %d: arg %d \n",getID,arg ) ;
		return -1;
}

void Status_Broadcast(const char *fmt, ...)
{

		char printf_buf[1024];
    va_list args;

    va_start(args, fmt);
    vsprintf(printf_buf, fmt, args);
    va_end(args);

    CPacket tCMD ;
    tCMD.Add( (BYTE *)printf_buf , strlen( printf_buf ) ) ;
    //tCMD.AddCRC() ;tCMD.Add(0x0D) ;tCMD.Add(0x0A) ;
 	// Not used	 by chj
	// send_msg( (char *)tCMD.GetBYTE(),tCMD.Getsize() ) ;

}


/*
void Packet_Make102(const char *fmt, ...)
{

		char printf_buf[1024];
    va_list args;

    va_start(args, fmt);
    vsprintf(printf_buf, fmt, args);
    va_end(args);

    make_packet.Clear() ;
    make_packet.Add( (BYTE *)printf_buf , strlen( printf_buf ) ) ;
    make_packet.AddCRC() ;make_packet.Add(0x0D) ;make_packet.Add(0x0A) ;

		pthread_mutex_lock(&mutex_cmd);
		CMD_BUF[CMD_TAIL].rx_index = 102 ;
		CMD_BUF[CMD_TAIL].size = make_packet.Getsize() ;
		memset( CMD_BUF[CMD_TAIL].buf , 0x00 , 128 ) ;
		memcpy( CMD_BUF[CMD_TAIL].buf , make_packet.GetBYTE() , make_packet.Getsize() ) ;

		if( CMD_TAIL < 255 ) CMD_TAIL++ ;
		pthread_mutex_unlock(&mutex_cmd);

}
*/

void HexaPrint( BYTE *Str,int size )
{
	int crcnt=0;
	int i = 0 ;
	for( i=0; i< size ;i++)
	{
		printf("%02X ", Str[i]);
		if( crcnt++> 8) { printf("\n") ; crcnt=0; }
	}

	printf("\n") ;

}



#ifndef _MSC_VER


//===================================================
// Constant
//===================================================
#define MY_TIMER_SIGNAL  SIGRTMIN
#define ONE_MSEC_TO_NSEC 1000000
#define ONE_SEC_TO_NSEC  1000000000

bool TimerMgr::setSignal()
{

 int rtn;
 struct sigaction sa;

 sa.sa_flags  = SA_SIGINFO;
 sa.sa_sigaction = timer_handler;

 sigemptyset(&sa.sa_mask);
 rtn = sigaction(MY_TIMER_SIGNAL, &sa, NULL);
 if (rtn == -1)
 {
  errReturn("sigaction");
 }
 return true;

}

bool TimerMgr::setTimer(int *key, long intv, TimerHandler tmFn, void *userPtr)
{
 int rtn;
 timer_t timerId;
 struct sigevent sigEvt;

 if (m_TimerNum >= MAX_TIMER_NUM)
 {
  printf("Too many timer\n");
  return false;
 }
 *key = findNewKey();
 /* Create Timer */
 memset(&sigEvt, 0x00, sizeof(sigEvt));
 sigEvt.sigev_notify    = SIGEV_SIGNAL;
 sigEvt.sigev_signo    = MY_TIMER_SIGNAL;
 sigEvt.sigev_value.sival_int = *key;
 rtn = timer_create(CLOCK_REALTIME, &sigEvt, &timerId);
 if (rtn != 0)
 {
  errReturn("timer_create");
 }
 /* Set Timer Interval */
 long nano_intv;
 struct itimerspec its;
 nano_intv = intv * ONE_MSEC_TO_NSEC;
 // initial expiration
 its.it_value.tv_sec  = nano_intv / ONE_SEC_TO_NSEC;
 its.it_value.tv_nsec  = nano_intv % ONE_SEC_TO_NSEC;
 // timer interval
 its.it_interval.tv_sec = its.it_value.tv_sec;
 its.it_interval.tv_nsec = its.it_value.tv_nsec;

 rtn = timer_settime(timerId, 0, &its, NULL);

 if (rtn != 0)
 {
  errReturn("timer_settimer");
 }
 /* Save Timer Inforamtion */
 TimerInfo tm;
 tm.m_TmId   = timerId;
 tm.m_TmFn  = tmFn;
 tm.m_UserPtr  = userPtr;

 m_TmInfoArr[*key] = tm;
 m_TmInfoArr[*key].m_IsUsed  = true;
 m_TimerNum++;

 return true;

}

bool TimerMgr::delTimer(int key)
{
 int rtn;
 TimerInfo tm;

 if (m_TimerNum <= 0)
 {
  printf("Timer not exist\n");
  return false;
 }

 if (m_TmInfoArr[key].m_IsUsed == true)
 {
  tm = m_TmInfoArr[key];
 } else {
  printf("Timer key(%d) not used\n", key);
  return false;
 }
 rtn = timer_delete(tm.m_TmId);
 if (rtn != 0)
 {
  errReturn("timer_delete");
 }
 m_TmInfoArr[key].m_IsUsed = false;
 m_TimerNum--;

 return true;

}

void TimerMgr::callback(int key)
{
 TimerInfo tm;
 tm = m_TmInfoArr[key];
 m_IsCallbacking = true;
 (tm.m_TmFn)(key, tm.m_UserPtr);
 m_IsCallbacking = false;
}

bool TimerMgr::isCallbacking()
{
 return m_IsCallbacking;
}

void TimerMgr::clear()
{
 for (int i = 0; i < MAX_TIMER_NUM; i++)
 {
  if (m_TmInfoArr[i].m_IsUsed == true)
   delTimer(i);
 }
 m_TimerNum  = 0;
}

int TimerMgr::findNewKey()
{
 for (int i = 0; i < MAX_TIMER_NUM; i++)
 {
  if (m_TmInfoArr[i].m_IsUsed == false)
   return i;
 }
}

//===================================================
// USER API
//===================================================

TimerMgr *g_TmMgr = 0;

bool create_timer()
{
 if (g_TmMgr)
 {
  printf("global timer handler exist aleady.\n");
  return false;
 }
 g_TmMgr = new TimerMgr();
 if ( !g_TmMgr)
 {
  printf("create_timer() fail\n");
  return false;
 }
 return g_TmMgr->setSignal();
}

bool set_timer(int *key, long resMSec, TimerHandler tmFn, void *userPtr)
{

 CHECK_NULL(key, false);
 CHECK_NULL(tmFn, false);
 bool rtn;
 rtn = g_TmMgr->setTimer(key, resMSec, tmFn, userPtr);
 if (rtn != true)
 {
  printf("set_timer(%p,%lu,%p,%p) fail\n",
    key, resMSec, tmFn, userPtr);
  return false;
 }
 return true;
}

bool delete_timer(int key)
{
 bool rtn;
 rtn = g_TmMgr->delTimer(key);

 if (rtn != true)
 {
  printf("delete_timer(%d) fail\n", key);
  return false;
 }
 return true;

}

bool destroy_timer()
{
 CHECK_NULL(g_TmMgr, false);
 if (g_TmMgr->isCallbacking() == true)
 {
  printf("Can't destroy timer in callback function.\n");
  return false;
 }
 g_TmMgr->clear();
 delete g_TmMgr;
 g_TmMgr = 0;
 return true;
}

//===================================================
// Timer Handler
//===================================================

static void timer_handler(int sig, siginfo_t *si, void *context)
{
 int key = si->si_value.sival_int;
 g_TmMgr->callback(key);
}

#endif
