#ifndef COMSOCKET_H
#define COMSOCKET_H
#endif
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <regex.h>


typedef struct Connection
{
	char* ip;
	in_port_t port; // equivalente a uint16_t
	int socketFD;
};
typedef struct ComAddress {
    char * path;
    char * port;    
    int socketFD;
};


/** Address format xxx.xxx.xxx.xxx:PPPPP **/