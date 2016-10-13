#ifndef SERVER_H
#define SERVER_H

#include "com.h"
#include "comFIFOs.h"
#include "blackjacklib.h"

typedef struct ServerData{
	ComAddress * srvAddress;
	Connection * clientTable[MAX_PLAYERS];
    int connectedBoolean[MAX_PLAYERS];
    Table * gameTable;
    Card * deck[DECK_SIZE];
    int deckIndex;
    int balance[MAX_PLAYERS];
}ServerData;

ServerData * newServerData();
void deleteServerData(ServerData * serverData);

// Connection related functions ---------------------------------------------------------------

void initializeDataBase();
int emptySpots(ServerData * serverData);
int firstEmptySpot(ServerData * serverData);
void checkIncomingConnections(ServerData * serverData);
void addClient(Connection * connection, ServerData * serverData);
int disconnectClient(ServerData * serverData, int index);
int hasBeenDisconnected(ServerData * serverData, int index);
int checkConnection(ServerData * serverData, int index);
void checkCurrentConnections(ServerData * serverData);

// Game related functions ----------------------------------------------------------------------

void generateDeck(ServerData * serverData);
void shuffleDeck(ServerData * serverData);
int requestBetTo(ServerData * serverData, int index);
void requestBetToPlayers(ServerData * serverData);
int isBetValid(ServerData * serverData, int index, int bet);
void updateBalance(ServerData * serverData, int index);
char requestPlay(Connection * Connection);
void askPlayersForHit(ServerData * serverData);
void deal(ServerData * serverData, int index);
void updateClientsOnDeal(ServerData * serverData, Deal * deal);
void updateClientsOnBet(ServerData * serverData, Bet * bet);
void updateClientsOn(ServerData * serverData, char action);
void updateClientsOnIndex(ServerData * serverData, int index, char action);
void payWinners(ServerData * serverData);
void dealInitialCards(ServerData * serverData);
void croupierPlay(ServerData * serverData);
void startRound(ServerData * serverData);

#endif