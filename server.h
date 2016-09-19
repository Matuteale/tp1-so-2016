#ifndef SERVER_H
#define SERVER_H
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int startServer(int * srvfd);
int acceptConnections(int srvfd, struct ClientInfo* clientTable[], int connectedBooleanTable[]);
struct ClientInfo * createClientInfo(int index, struct ClientInfo * clientTable[], int connectedBooleanTable[]);
int emptySpots(int connectedBooleanTable[]);
int firstEmptySpot(int connectedBooleanTable[]);
int disconnectClient(int clientIndex, struct ClientInfo ** clientTable, int * connectedBooleanTable);