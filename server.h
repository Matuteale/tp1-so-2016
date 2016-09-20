#ifndef SERVER_H
#define SERVER_H
#endif
#include "com.h"

int startServer();
int emptySpots(int * connectedBoolean);
int firstEmptySpot(int * connectedBoolean);
void addClient(Connection * connection, Connection ** clientTable, int * connectedBoolean);