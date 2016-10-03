#ifndef SERVER_H
#define SERVER_H

#include "com.h"
#include "comFIFOs.h"
#include "blackjacklib.h"

typedef struct ServerData{
	char * srvpath;
	Connection * clientTable[MAX_PLAYERS];
    int connectedBoolean[MAX_PLAYERS];
    Table * gameTable;
    Card * deck[CARDS_PER_DECK * PLAYING_DECKS];
    int deckIndex;
    int balance[MAX_PLAYERS];
}ServerData;

ServerData * newServerData();
void deleteServerData(ServerData * serverData);

// Connection related functions ---------------------------------------------------------------

int startServer(ServerData * serverData);
int emptySpots(ServerData * serverData);
int firstEmptySpot(ServerData * serverData);
void checkIncomingConnections(ServerData * serverData);
void addClient(Connection * connection, ServerData * serverData);
int disconnectClient(int index, ServerData * serverData);
int hasBeenDisconnected(int index, ServerData * serverData);
void checkCurrentConnections(ServerData * serverData);

// Game related functions ----------------------------------------------------------------------

void generateDeck(ServerData * serverData);
void shuffleDeck(ServerData * serverData);
int requestBet(ServerData * serverData, int index);
void requestBetToPlayers(ServerData * serverData);
int isBetValid(ServerData * serverData, int index, int bet);
void updateBalance(ServerData * serverData, int index);
char requestPlay(Connection * Connection);
void askPlayerForHit(ServerData * serverData);
void deal(ServerData * serverData, int index);
void updateClientsOnDeal(ServerData * serverData, Deal * deal);
void updateClientsOnShuffle(ServerData * serverData);
void updateClientsOn(ServerData * serverData, int index, char action);
void payWinners(ServerData * serverData);
int hasWon(Seat * seat, int croupierScore);
void dealInitialCards(serverData);
void croupierPlay(ServerData * serverData);
void startRound(ServerData * serverData);

#endif