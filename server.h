#ifndef SERVER_H
#define SERVER_H
#endif
#include "com.h"

typedef struct {
	Connection * clientTable[MAX_PLAYERS];
    int connectedBoolean[MAX_PLAYERS];
} ServerData;

ServerData * newServerData();

int startServer();
int emptySpots(ServerData * serverData);
int firstEmptySpot(ServerData * serverData);
void addClient(Connection * connection, ServerData * serverData);
int disconnectClient(int index, ServerData * serverData);
int hasBeenDisconnected(int index, ServerData * serverData);
void checkConnections(ServerData * serverData);