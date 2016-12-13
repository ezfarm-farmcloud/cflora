

#ifndef __TTA12_DEFINE_HEADER__
#define __TTA12_DEFINE_HEADER__

#include "common.h"

#define MAX_SNODE_COUNT 8
#define MAX_ANODE_COUNT 8

#define MAXNODE_CHANNEL 16

#define MAX_SENSOR_COUNT	MAXNODE_CHANNEL*8+1
#define MAX_ACTUATOR_COUNT	MAXNODE_CHANNEL*8+1



// FRAME CONTROL FIELD  ========================
#define FCF_SENSOR     0
#define FCF_ACTUATOR   1

// Frame Type
#define FTYPE_REQUEST     0
#define FTYPE_RESPONSE    1
#define FTYPE_CONFIRM     2
#define FTYPE_NOTIFY      3
#define FTYPE_DATA        4
#define FTYPE_ACK         5

// Payload Type
#define PLTYPE_NODEINIT   0  //���� �ʱ�ȭ
#define PLTYPE_NODEINFO   1  //���� ������û
#define PLTYPE_VALUE      2  //���� ������

// Request Command type
#define REQCMD_INIT				0
#define REQCMD_INIT_NODE		1
#define REQCMD_INIT_ACT			2
#define REQCMD_INIT_SENSOR		2
#define REQCMD_NODE_INFO		3
//#define REQCMD_ACTUATOR_INFO	5
//#define REQCMD_SENSOR_INFO	6

#define REQCMD_ACTUATOR_SET		4
#define REQCMD_PASSIVE_MODE    10
#define REQCMD_ACTIVE_MODE     11
#define REQCMD_EVENT_MODE      12

// NODE ALL
#define NODE_ALL			255

// Sensor type
#define SRADIATION			1  //�ϻ緮
#define TEMPERATURE			2  //�µ�
#define HUMIDITY			3  //����
#define WDIR				4  //ǳ��
#define WSPEED				5  //ǳ��
#define RAIN				6  //����
#define PHORADIATION		7  //���ռ�����
#define ILLUMINATION		8  //����
#define CO2					9
#define MOISTURE			10 //ǳ��
#define PH					11
#define EC					12

#define DIGITALIN			13
#define COUNTERIN			14
#define RAW			        20


#define SENSOR_PASSIVE_MODE		1
#define SENSOR_ACTIVE_MODE    2
#define SENSOR_EVENT_MODE     3

// Active Mode
#define PERSEC					0x00 // 0b0000
#define PERMIN					0x40   // 0b0100
#define PERHOUR					0x80
#define PERDAY					0xC0
//#define PERMONTH

// Event Mode
#define LIMIT_OVER      0x01
#define LIMIT_UNDER     0x02
#define LIMIT_EXCEED    0x03
#define LIMIT_BELOW     0x04
#define LIMIT_EQUAL     0x05

// Actuator type

#define ACT_HEAT			0x01 //����
#define ACT_WARM			0x02 //����
#define ACT_SHUTTER			0x03 //����
#define ACT_AIR				0x04 //����
#define ACT_LIGHT			0x05 //����
#define ACT_WATER			0x06 //����
#define ACT_CO2				0x07
#define ACT_DEHUMIDITY		0x08 //����
#define ACT_PH				0x09
#define ACT_EC				0x0A
#define ACT_ETC				0x10

#define ON_SWITCH			0x21 //ON ��������ġ
#define OFF_SWITCH			0x22 //OFF ��������ġ
#define STOP_SWITCH			0x23 //STOP ��������ġ
#define START_SWITCH		0x24 //START ��������ġ
#define LEFT_SWITCH			0x25 //LEFT ��������ġ
#define RIGHT_SWITCH		0x26 //RIGHT ��������ġ


// ERROR code
#define INIT_ERROR					1
#define RESET_ERROR					2
#define INIT_SNODE_ERROR			3
#define SENSOR_ERROR				4
#define SENSOR_INT_ERROR			5
#define READ_ERROR					6
#define KEEP_ALIVE					8
#define RECEIVED_FAIL				9
#define SW_TIMER_ERROR				10
#define HW_TIMER_ERROR				11
#define PWR_ERROR					12
#define PWR_OFF						13
#define BAT_ERROR					14
#define BAT_LOW						15
#define BAT_OFF						16

#define START_DATA_PACKET           10


#define TIME_MIN 60
#define TIME_HOUR 60*60
#define TIME_DAY TIME_HOUR*24


// MFC,Linux ���� BitField ����ü ������ MSB,LSB ������ �����Ұ�
// ���� ���� �Ҵ� �� ����Ȯ���Ұ� .

// Structure define
// Linux
/*
typedef struct
{
	BYTE device:1;
	BYTE type:3;
	BYTE security:1;
	BYTE ack_confirm:1;
	BYTE reserved:2;

} FrameControlField ;
*/
//windows
typedef struct
{
	BYTE reserved:2;
	BYTE ack_confirm:1;
	BYTE security:1;
	BYTE type:3;
	BYTE device:1;

} FrameControlField ;

typedef struct
{

	BYTE length; //6bit �ΰ��� STATE1 ������ �����÷ο�
	BYTE type;

} PayloadField ;

typedef struct
{
	BYTE id ;
	BYTE type ;
	WORD value ;
} SensorValue ;


typedef struct
{
	BYTE id ;
	BYTE type ;
	WORD value ;
} ActuatorValue ;

typedef struct
{
	WORD NODE_ID ;  //������ȣ
	BYTE CH ; // ������ ä��
	char ID ; // ���� ���̵�
	char ARG ;
	BYTE INIT_TIME ; // �ʱ�ȭ�� ON ���� �ð�
	u32  SET_INTERVAL;  // ON ���� �ð�(�ʴ���)  0xFF : �׻� ON  0x00 : OFF
	u32  COUNT;   // �ٿ�ī��Ʈ(�ʴ���)
	BYTE TYPE;
	WORD CANID;

	WORD SPEED ;
	BYTE PORTID ;
	BYTE LENGTH ;

} HWCONFIG ;

typedef struct
{
	WORD NODE_ID ;
	SensorValue data ;
	BYTE state ;
	BYTE isinit;
	BYTE mode ;
	BYTE time ;
} SensorInfo ;

typedef struct
{
	WORD NODE_ID ;
	ActuatorValue data ;
	BYTE state ;
	BYTE isinit;
	BYTE mode ;
	BYTE time ;
} ActuatorInfo ;


typedef struct
{

	BYTE SW_VER ;
	BYTE PROFILE_VER ;
	WORD CONTROL_ID ;
	WORD NODE_ID ;
	BYTE ISINIT_NODE ;
	BYTE MONITOR_MODE;
	WORD MONITOR_TIME;
	BYTE SENACT_NUM ;
	WORD COMM_ERROR_NUM ;
	WORD SERVICE_ERROR_NUM ;

} NODESTATUS ;

typedef struct
{
	BYTE SW_VER ;
	BYTE PROFILE_VER ;
	WORD CONTROL_ID ;
	BYTE ISINIT_COMM_NODE ;
	BYTE ISINIT_SENACT ;
	BYTE IS_ERROR ;
	BYTE BUSY ;
	BYTE REQUEST_COMM ;

} CONTROLLERSTATUS ;

#define MAX_COMM_SENSOR  5
#define COMM_SENSOR_SIZE  10
typedef struct
{
	BYTE PORT ;
	WORD SPEED ;
	BYTE LEN ;

	char TTY[128] ;
	char EXP[128] ;
	WORD ENCODE ;

	WORD SENSOR_NODE ;
	BYTE SENSOR_CNT ;
} COMM_SENSOR_STRUCT ;

#endif
