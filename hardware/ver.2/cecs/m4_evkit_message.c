/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Company           : MesaMedical Co., Ltd.
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Revision          : V1.00
//  Project           : MSTD 2013
//  Editor            : Hyunju Lee
//  Create Date       : 2013-05-30
//  Modify Date       : 2016-03-06
//  File              : M4_EVKIT_MESSAGE.C
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include  "m4_evkit_define.h"

MESSAGE	m_MSG[DEF_MESSAGE_CNT];
u8			m_MsgHead, m_MsgTail;

void ClearMessageHandle( void ) { m_MsgHead  = m_MsgTail = 0; }

bool PushMessage( MESSAGE msg )
{
	if( ((m_MsgHead+1) % DEF_MESSAGE_CNT) == m_MsgTail ) { return false; }
	
	m_MSG[m_MsgHead++] = msg;
	m_MsgHead %= DEF_MESSAGE_CNT;
	
	return true;
}

bool GetMessage( LPMESSAGE msg )
{
	if( m_MsgHead != m_MsgTail ) {
		*msg = m_MSG[m_MsgTail++];
		m_MsgTail %= DEF_MESSAGE_CNT;

		return true;
	}
	
	return false;
}
