#ifndef COMSOCKET_H
#define COMSOCKET_H
#endif
#include <sys/socket.h>
#include <netinet/in.h>


struct Connection
{
	char* ip;
	in_port_t port; // equivalente a uint16_t
	int socketFD;
};