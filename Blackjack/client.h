#ifndef CLIENT_H
#define CLIENT_H

#include "com.h"
#include "blackjacklib.h"

typedef struct ClientData{
	Connection * serverConnection;
	Table * gameTable;
	int balance;
}ClientData;

ClientData * newClientData();
void deleteClientData(ClientData * clientData);

void askToQuit();
void disconnectClient();
void waitForServer();
void bet();
void play();
void deal();
void clearSeatAction();
void clearTableAction();
void shuffleAction();
void setActiveAction();
void setUnActiveAction();
void updateBalance();
void updateBet();

#endif