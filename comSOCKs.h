#ifndef COMSOCKET_H
#define COMSOCKET_H
#include <sys/socket.h>
#include <signal.h>
#include <sys/un.h>


typedef struct Connection
{
	//char* ip;
	//in_port_t port; // equivalente a uint16_t
	int socketFD;
};


/** Address format xxx.xxx.xxx.xxx:PPPPP **/
#endif