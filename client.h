#ifndef CLIENT_H
#define CLIENT_H
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int requestConnection(char * serverFifo, struct ClientInfo * clientInfo);
void disconnect(struct ClientInfo* clientInfo);