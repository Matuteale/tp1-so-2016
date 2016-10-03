#ifndef CLIENT_H
#define CLIENT_H

#include "com.h"
#include "comSOCKs.h"
#include "blackjacklib.h"

typedef struct ClientData{
	Connection * serverConnection;
	Table * gameTable;
	int balance;
}ClientData;

ClientData * newClientData();

void deleteClientData(ClientData * clientData);
void askToQuit(ClientData * clientData);
void disconnectClient(ClientData * clientData);
void waitForServer(ClientData * clientData);
void bet(ClientData * clientData);
void play(ClientData * clientData);
void deal(ClientData * clientData);
void clearSeatAction(ClientData * clientData);
void clearTableAction(ClientData * clientData);
void shuffleAction();
void setActiveAction(ClientData * clientData);
void setUnActiveAction(ClientData * clientData);

#endif