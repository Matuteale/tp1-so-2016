#ifndef COMSOCKET_H
#define COMSOCKET_H
#endif
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <regex.h>

#define MAX_BUF 1024

// MSGS CODES
#define SUCCESS "1"
#define FAIL "0"

typedef struct Connection {
	//char* ip;
	//in_port_t port;
	int socketFD;
};

typedef struct ComAddress {
    char * path;
    char * port;
    int socketFD;
};