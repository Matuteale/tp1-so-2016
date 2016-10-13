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
typedef struct Parameters Parameters;

Connection * newConnection();
Parameters * newParameters();
Connection * comConnect(Parameters * params);
char * comListen(Parameters * params);
Connection * comAccept(Parameters * params);
int comWrite(Connection * connection, char * dataToWrite, int size);
int comRead(Connection * connection, char * dataToRead, int size);
void disconnect(Connection * connection);

#endif