////////////////////////////////////////////////////////////////////////////////
//  Company           : 
//  Compiler          : IAR C/C++ Compiler for ARM, 6.50
//  Revision          : 1.00
//  Project           : EVKIT 2014
//  Editor            : Hyunju Lee
//  Create Date       : 2014-05-30
//  File              : EVKIT_MESSAGE.C
////////////////////////////////////////////////////////////////////////////////
#include  "def_application.h"

MESSAGE   m_MSG[DEF_MESSAGE_CNT];
u8			  m_MsgHead, m_MsgTail;

void ClrMessage()
{
  m_MsgHead  = m_MsgTail = 0;
}

BOOL IsMessage()
{
  if( m_MsgHead == m_MsgTail ) { return FALSE; } else { return TRUE; }
}

BOOL PushMessage( MESSAGE msg )
{
	if( ((m_MsgHead+1) % DEF_MESSAGE_CNT) == m_MsgTail ) { return FALSE; }
	
	m_MSG[m_MsgHead++] = msg;
	m_MsgHead %= DEF_MESSAGE_CNT;
	
	return TRUE;
}

BOOL GetMessage( LPMESSAGE msg )
{
	if( IsMessage() ) {
		*msg = m_MSG[m_MsgTail++];
		m_MsgTail %= DEF_MESSAGE_CNT;

		return TRUE;
	}
	
	return FALSE;
}
