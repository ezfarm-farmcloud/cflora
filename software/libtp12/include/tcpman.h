

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <netdb.h>
#include "common.h"

#define BUF_SIZE 128
#define MAX_CLNT 256


#ifndef __TCPMANAGER__
#define __TCPMANAGER__

class tcpmanager
{
private :	
	
	
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;	
	int current_sock ;
public: 
	int initok;
public :


tcpmanager() ; 
	~tcpmanager() ; 
	
	int InitServer( int port ) ;
	int ClientConnect( char *ipstr, int port ) ;
	
	void CloseServer(void) ;
	void CloseClient(int sock) ;
	int getConnectionCount( void) ;
	
	int	Wait_Connect(void) ; 
	
		
} ; 

extern "C"
{
int send_msg(char *msg, int len)  ;
}

extern pthread_mutex_t tcp_mutex ;
extern pthread_mutex_t cmd_mutex ;

extern WORD TCP_CMD_HEAD ; 
extern WORD TCP_CMD_TAIL ; 
extern MESSAGE_STRUCT TCP_CMD_BUF[256] ;

#endif