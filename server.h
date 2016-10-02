#ifndef SERVER_H
#define SERVER_H

#include "com.h"
#include "comFIFOs.h"
#include "blackjacklib.h"

typedef struct ServerData{
	Connection * clientTable[MAX_PLAYERS];
    int connectedBoolean[MAX_PLAYERS];
    Table * gameTable;
    Card * deck[CARDS_PER_DECK * PLAYING_DECKS];
    int deckIndex;
}ServerData;

ServerData * newServerData();
void deleteServerData(ServerData * serverData);

// Connection related functions ---------------------------------------------------------------

int startServer();
int emptySpots(ServerData * serverData);
int firstEmptySpot(ServerData * serverData);
void addClient(Connection * connection, ServerData * serverData);
int disconnectClient(int index, ServerData * serverData);
int hasBeenDisconnected(int index, ServerData * serverData);
void checkConnections(ServerData * serverData);

// Game related functions ----------------------------------------------------------------------

void generateDeck(ServerData * serverData);
void shuffleDeck(ServerData * serverData);
//TODO: VER SI ESTA FUNCION QUEDA ACA O EN MARSHALLING
void sendAction(Connection * connection, char action);
int requestBet();
char requestPlay(Connection * Connection);
void dealCardsToPlayers(ServerData * serverData);
void deal(ServerData * serverData, int index);
void updateClientsOnDeal(ServerData * serverData, Deal * deal);
void updateClientsOnDisconnect(ServerData * serverData, int index);
void updateClientsOnActive(ServerData * serverData, int index);
void updateClientsOnUnActive(ServerData * serverData, int index);

#endif