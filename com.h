#ifndef COM_H
#define COM_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

typedef struct Connection Connection;
typedef struct ComAddress ComAddress;

Connection * newConnection();
ComAddress * newComAddress();
void deleteComAddress(ComAddress * address);
Connection * comConnect(ComAddress * address);
ComAddress * comListen(ComAddress * address);
Connection * comAccept(ComAddress * addressToAccept);
int openListener(ComAddress * address);
void closeListener(ComAddress * address);
int isConnected(ComAddress * address);
int comWrite(Connection * connection, char * dataToWrite, int size);
int comRead(Connection * connection, char * dataToRead, int size);
void disconnect(Connection * connection);

#endif