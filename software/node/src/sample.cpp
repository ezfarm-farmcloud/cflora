액추에이터 동작 예제 --------------------------------------------------------------------

void ActuatorSet(void)
{
	// REQCMD_ACTUATOR_SET
	TTA12Packet TTAPacket;

	TTAPacket.Clear() ; 
	TTAPacket.SetFrameDevice( FCF_ACTUATOR ) ;
	TTAPacket.SetFrameType( FTYPE_REQUEST) ;
	TTAPacket.SetSecurity( 1 ) ;
	TTAPacket.SetAckConfirm( 1 ) ;

	TTAPacket.SetControlID( 0 ) ;
	TTAPacket.SetNodeID( 0 ) ;
	//TTAPacket.SetRequestType( REQCMD_PASSIVE_MODE ) ; 
	//TTAPacket.SetRequestType( REQCMD_ACTIVE_MODE ) ; 
	//TTAPacket.SetRequestType( REQCMD_EVENT_MODE ) ; 
	TTAPacket.SetRequestType( REQCMD_ACTUATOR_SET ) ; 
	SequenceCount++ ;	
	TTAPacket.MakeHeader() ; 
	TTAPacket.MakePayload() ;
/*
	for( int i = 0 ; i < 8 ; i++)
	{
		SensorSettingData[i].id = i ;
		SensorSettingData[i].type = SRADIATION ; 
		SensorSettingData[i].value = 0 ;
	}
	TTAPacket.MakeSensorData( PLTYPE_VALUE,8,SensorSettingData ) ;  	
*/
/*
	for( int i = 0 ; i < 2 ; i++)
	{
		ActuatorSettingData[i].id = i+1 ;
		ActuatorSettingData[i].type = ACT_WARM ; 
		ActuatorSettingData[i].value = 0x0000 ;
	}
*/
	ActuatorSettingData[0].id = 1 ;            // 환경설정에서 지정한 ID 값 ( CH값 아님 )
	ActuatorSettingData[0].type = ACT_WARM ; 
	ActuatorSettingData[0].value = 0x0000 ;


	ActuatorSettingData[1].id = 1 ;
	ActuatorSettingData[1].type = ACT_WARM ; 
	ActuatorSettingData[1].value = 0x010A ;  // 상위 ARG,하위 ON time

	ActuatorSettingData[2].id = 3 ;
	ActuatorSettingData[2].type = ACT_WARM ; 
	ActuatorSettingData[2].value = 0x00FF ;

	TTAPacket.MakeActuatorData( PLTYPE_VALUE,3,ActuatorSettingData ) ;  	
	

	BYTE tranbuff[512] ; //TCP BUFFER 
	CPacket *pk = TTAPacket.GetPacket() ; 
	int size = pk->Getsize() ;
  //TCP/IP 전송 
	m_List.HexaAddString( CXListBox::Red,CXListBox::White, pk->GetBYTE(), pk->Getsize());
	for(int i=0; i<MAX_CONNECTION; i++)
	{
		if (m_SocketManager[i].IsOpen() && m_pCurServer != &m_SocketManager[i])
			m_SocketManager[i].WriteComm(pk->GetBYTE(), pk->Getsize(), INFINITE);
	}

}

Sensor Packet 수신 처리 예제 -----------------------------------------------------------------------

	TTA12PacketParsing TTAParser;  // TTA12 패킷 해석 클래스 

	SensorValue getsData[8] ;    // TTA12_define.h 참조 
	ActuatorValue getaData[8] ;  // TTA12_define.h 참조 

	
	// 메인프로그램의 루프 
	while(1)
	{

		if( TCP_CMD_HEAD < TCP_CMD_TAIL )
		{

			if( TCP_CMD_BUF[TCP_CMD_HEAD].size > 0 )
			{
				TTAParser.Clear() ; 
				TTAParser.packet.Add( TCP_CMD_BUF[TCP_CMD_HEAD].buf, TCP_CMD_BUF[TCP_CMD_HEAD].size ) ; 

				if( TTAParser.ParsingPacket() == FTYPE_DATA )
				{ 

					CPacket *rk = TTAParser.GetReturnPacket() ;  // ACK 패킷을 만들어 리턴해줌
					printf("FTYPE_DATA \n") ;
					send_msg( (char *)rk->GetBYTE(),rk->Getsize() ) ;

					If( TTAParser.FCF.device == FCF_SENSOR ) 
					{
						int count  = TTAParser.GetSensorData( getsData ) ; 
						//리턴값: 패킷내의 수신데이터 개수 한패킷의 데이터갯수는 8개 이하
				
					}	
			 
					If( TTAParser.FCF.device == FCF_ACTUATOR) 
					{
						int count  = TTAParser.GetActuatorData( getaData ) ;
					}


				}

			}

			//cmd_packet.Clear() ;
			
			pthread_mutex_lock(&cmd_mutex);
			TCP_CMD_HEAD++ ; 
			if(  TCP_CMD_HEAD == TCP_CMD_TAIL ) TCP_CMD_HEAD=TCP_CMD_TAIL=0 ; 
			pthread_mutex_unlock(&cmd_mutex);
		}

		
		usleep(20000) ;
	}

