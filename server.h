#ifndef SERVER_H
#define SERVER_H
#endif

int startServer(int * srvfd);
int acceptConnections(int srvfd, struct ClientInfo* clientTable[], int connectedBooleanTable[]);
struct ClientInfo * createClientInfo(int index, struct ClientInfo * clientTable[], int connectedBooleanTable[]);
int emptySpots(int connectedBooleanTable[]);
int firstEmptySpot(int connectedBooleanTable[]);