#ifndef SERVER_H
#define SERVER_H

#include "com.h"
#include "blackjacklib.h"
#include "logging.h"

typedef struct ServerData{
	ComAddress * srvAddress;
	Connection * clientTable[PLAYERS];
    int connectedBoolean[PLAYERS];
    Table * gameTable;
    Card * deck[DECK_SIZE];
    int deckIndex;
}ServerData;

ServerData * newServerData();
void deleteServerData(ServerData * serverData);

// Connection related functions ---------------------------------------------------------------

int emptySpots();
int firstEmptySpot();
void checkIncomingConnections();
void addClient(Connection * connection);
int disconnectClient(int index);
int hasBeenDisconnected(int index);
int checkConnection(int index);
void checkCurrentConnections();
void closeServer();
void validateConfig();

// Game related functions ----------------------------------------------------------------------

void generateDeck();
void shuffleDeck();
int requestBetTo(int index);
void requestBetToPlayers();
int isBetValid(int index, int bet);
void updateBalance(int index);
char requestPlay(Connection * Connection);
void askPlayersForHit();
void deal(int index);
void updateClientsOnDeal(Deal * deal);
void updateClientsOnBet(Bet * bet);
void updateClientsOn(char action);
void updateClientsOnIndex(int index, char action);
void payWinners();
void dealInitialCards();
void croupierPlay();
void startRound();

#endif