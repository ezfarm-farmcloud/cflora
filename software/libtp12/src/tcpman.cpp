#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h> 
#include "tcpman.h"
#include "signal.h"


void* Read_Thread(void * arg);
int send_msg(char * msg, int len);

static int clnt_cnt=0;
static int clnt_socks[MAX_CLNT];

pthread_mutex_t tcp_mutex ;
pthread_mutex_t cmd_mutex ;

char trx_str[128] ;


WORD TCP_CMD_HEAD = 0 ; 
WORD TCP_CMD_TAIL = 0 ; 
MESSAGE_STRUCT TCP_CMD_BUF[256] ;



tcpmanager::tcpmanager() 
{
	 initok=0; 
	 serv_sock=0;
	 clnt_sock=0;
	 current_sock=0;
	 pthread_mutex_init(&tcp_mutex, NULL);	 
	 pthread_mutex_init(&cmd_mutex, NULL);	 
} 

tcpmanager::~tcpmanager() 
{
	CloseServer();
} 

void SortSockBuff(int sock)
{
	int i;
	

	pthread_mutex_lock(&tcp_mutex) ;	
	for(i=0; i<clnt_cnt; i++)   // remove disconnected client
	{
		if(sock==clnt_socks[i])
		{
			
			while( i < clnt_cnt-1)
			{
				//printf("%d, %d=%d\n", i , clnt_socks[i],clnt_socks[i+1] ) ;
				clnt_socks[i]=clnt_socks[i+1];
				i++ ;
			}
			
			break;
		}
	}
	if( clnt_cnt > 0) clnt_cnt--;
/*
	for(i=0; i<clnt_cnt; i++)   // remove disconnected client
	{
		printf("sockbuf num[%d]:%d \n",i,clnt_socks[i] ) ; 
	}
*/
	close( sock );
	
	pthread_mutex_unlock(&tcp_mutex);
}	

int tcpmanager::InitServer( int port ) 
{

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
  int option = 1;          // SO_REUSEADDR 의 옵션 값을 TRUE 로
  setsockopt( serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option) );
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET; 
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(port);
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
	{
		printf("bind error !! \n") ; close(serv_sock); return 0 ;
	}
	if(listen(serv_sock, 5)==-1)
	{
		printf("listen() error \n") ; return 0 ; 
	}
	initok=1;
	return 1; 	
	
}

int tcpmanager::ClientConnect( char *ipstr, int port ) 
{
 
	clnt_sock=socket(AF_INET, SOCK_STREAM, 0); // PF_INER => AF_INET
  
	memset(&clnt_adr, 0, sizeof(clnt_adr));
	
	clnt_adr.sin_addr.s_addr=inet_addr(ipstr);
	clnt_adr.sin_family=AF_INET; 
	clnt_adr.sin_port=htons(port);
	
    //Connect to remote server
  if (connect(clnt_sock , (struct sockaddr *)&clnt_adr , sizeof(clnt_adr)) < 0)
  {
      printf("client connect failed. Error[%d]\n",port);
      return -1;
  }
     
  printf("Connected %s\n",ipstr) ;
	initok=2 ;
	
	if(  clnt_sock != -1 )
	{
		pthread_mutex_lock(&tcp_mutex);
		clnt_socks[clnt_cnt++]=clnt_sock;

		pthread_create(&t_id, NULL, Read_Thread, (void*)&clnt_sock);
		//SetThreadPriority( t_id, THREAD_PRIORITY_HIGHEST ) ; 
		pthread_detach(t_id);
		usleep(1000) ;
		printf("socketID %d Connected client IP: %s \n",clnt_sock, inet_ntoa(clnt_adr.sin_addr));
		
		for(int i=0; i<clnt_cnt; i++)   // remove disconnected client
		{
			printf("sockbuf num[%d]:%d \n",i,clnt_socks[i] ) ; 
		}		
		pthread_mutex_unlock(&tcp_mutex);


	} 	
	
	return clnt_sock ; 	
	
}

void tcpmanager::CloseClient(int sock) 
{
		SortSockBuff( sock ) ;

	  printf(" Disconnect socketID %d\n", clnt_sock ) ;
}

void tcpmanager::CloseServer(void)
{
	if( initok ) close(serv_sock);
}

int tcpmanager::getConnectionCount( void) 
{
	return clnt_cnt ;
}

int	tcpmanager::Wait_Connect(void)  
{
	clnt_adr_sz=sizeof(clnt_adr);
	printf("Wait conntection...\n") ;
	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, (socklen_t*)&clnt_adr_sz);
	if(  clnt_sock != -1 )
	{
		pthread_mutex_lock(&tcp_mutex);
		clnt_socks[clnt_cnt++]=clnt_sock;

		pthread_create(&t_id, NULL, Read_Thread, (void*)&clnt_sock);
		//SetThreadPriority( t_id, THREAD_PRIORITY_HIGHEST ) ; 
		pthread_detach(t_id);
		usleep(1000) ;
		printf("socketID %d Connected client IP: %s \n",clnt_sock, inet_ntoa(clnt_adr.sin_addr));
		
		for(int i=0; i<clnt_cnt; i++)   // remove disconnected client
		{
			printf("sockbuf num[%d]:%d \n",i,clnt_socks[i] ) ; 
		}		
		pthread_mutex_unlock(&tcp_mutex);

/*
		g_GPIO->output( PTT_OUT,0 ) ;

		pthread_mutex_lock(&mutex_cmd) ;

		memset(trx_str,0x00,128 ) ;
		sprintf(trx_str,"$CMRC,CONSOCK,%d,%s*\n",clnt_sock,inet_ntoa(clnt_adr.sin_addr)) ;	

		CMD_BUF[CMD_TAIL].rx_index = clnt_sock ;
		CMD_BUF[CMD_TAIL].size = strlen(trx_str) ;
		memset( CMD_BUF[CMD_TAIL].buf , 0x00 , 128 ) ; 
		memcpy( CMD_BUF[CMD_TAIL].buf , trx_str, CMD_BUF[CMD_TAIL].size ) ; 	
		if( CMD_TAIL < 255 ) CMD_TAIL++ ; 
		pthread_mutex_unlock(&mutex_cmd);
*/
	} 
	return 1 ;
}


void *Read_Thread(void * arg)
{
	int clnt_sock=*((int*)arg);
	int str_len=0, i;
	char msg[BUF_SIZE] ;

	
	while( (str_len=read(clnt_sock, msg, sizeof(msg)) ) != 0 )
	{	
		//printf( "return size:%d\n" ,str_len ) ;  
		pthread_mutex_lock(&cmd_mutex);
		//send_msg(msg, str_len);
		TCP_CMD_BUF[TCP_CMD_TAIL].rx_index = clnt_sock ;
		TCP_CMD_BUF[TCP_CMD_TAIL].size = str_len ;
		memset( TCP_CMD_BUF[TCP_CMD_TAIL].buf , 0x00 , 128 ) ; 
		memcpy( TCP_CMD_BUF[TCP_CMD_TAIL].buf , msg, str_len ) ; 
		
		if( TCP_CMD_TAIL < 254 ) TCP_CMD_TAIL++ ; 
		pthread_mutex_unlock(&cmd_mutex);
	
		
	}

	SortSockBuff( clnt_sock ) ;

	printf(" Disconnect socketID %d\n", clnt_sock ) ;

	return NULL;
}

int send_msg(char *msg, int len)   // send to all
{
	int i,j;
	int clnt_sock ;
//	pthread_mutex_lock(&cmd_mutex);
//	printf(" tcp send  start\n") ;
	for(i=0; i<clnt_cnt; i++)
	{
		
		if( write(clnt_socks[i], msg, len) != len )
		{

			printf("TCP send Error %d\n",len) ; 
/*			
			clnt_sock = clnt_socks[i] ;
			printf(" Disconnect socketID %d\n", clnt_sock ) ;
			printf(" Disconnect socketID %d\n", clnt_sock ) ;
			printf(" Disconnect socketID %d\n", clnt_sock ) ;
			
			for(j=0; j<clnt_cnt; j++)   // remove disconnected client
			{
				if(clnt_sock==clnt_socks[j])
				{
					while(j <clnt_cnt-1) 
					{
						clnt_socks[j]=clnt_socks[j+1];
						j++ ;
					}
					break ;
				}
			}
	
			if( clnt_cnt > 0) clnt_cnt--;
			close(clnt_sock);
			break;
*/
		}
		//printf("%s",msg) ; 
		//usleep(1000*10) ;
	}
	//printf(" tcp send  end\n") ;
//	pthread_mutex_unlock(&cmd_mutex);
	return clnt_cnt ;
}
