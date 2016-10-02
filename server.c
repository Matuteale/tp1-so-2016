#include "blackjacklib.h"
#include "server.h"
#include <sys/types.h>

int main() {

    ServerData * serverData = newServerData();
    serverData->gameTable = newTable();

    startServer();

    char * srv = malloc(sizeof(SRV_PATH));
    strcpy(srv, SRV_PATH);

    while(1) {
        if (emptySpots(serverData) > 0) {
            char * listened = comListen(srv);
            if (listened != NULL) {
                printf("Connection detected\n");
                addClient(comAccept(listened), serverData);
            }
        }
        checkConnections(serverData);
    }

    return 0;
}

ServerData * newServerData() {
    ServerData * serverData = malloc(sizeof(ServerData));
    memset(serverData->connectedBoolean, 0, sizeof(serverData->connectedBoolean));
    return serverData;
}

void deleteServerData(ServerData * serverData) {
    free(serverData);
}

// Connection related functions ---------------------------------------------------------------

int startServer() {

    /* Creating SRV FIFO */
    unlink(SRV_PATH);
    mkfifo(SRV_PATH, 0666);

    open(SRV_PATH, O_RDONLY | O_NONBLOCK);
    
    return 1;
}

int emptySpots(ServerData * serverData) {
    int i;
    int count = 0;

    for (i = 0; i < MAX_PLAYERS; i++) {
        if (serverData->connectedBoolean[i] == 0) {
            count++;
        }
    }

    return count;
}

int firstEmptySpot(ServerData * serverData) {
    int i;

    for (i = 0; i < MAX_PLAYERS; i++) {
        if (serverData->connectedBoolean[i] == 0) {
            return i;
        }
    }

    return -1;
}

void addClient(Connection * connection, ServerData * serverData) {
    int index = firstEmptySpot(serverData);
    serverData->clientTable[index] = connection;
    serverData->connectedBoolean[index] = 1;
    printf("Client connected in spot %d.\n", index);
}

int disconnectClient(int index, ServerData * serverData) {
    if (serverData->connectedBoolean[index] == 0) {
        return 0;
    }
    disconnect(serverData->clientTable[index]);
    serverData->connectedBoolean[index] = 0;
    setUnActive(serverData->gameTable->playerSeats[index]);
    updateClientsOnDisconnect(serverData, index);
    printf("Client in spot %d disconnected.\n", index);
    return 1;
}

int hasBeenDisconnected(int index, ServerData * serverData) {
    if( access(serverData->clientTable[index]->output, F_OK ) != -1 ) {
        return 0;
    }
    return 1;
}

//TODO: Remover, ya que solo la usamos ahora para probar la forma de desconectar clientes
// despues vamos a checkear cuando esten jugando en su turno si se desconectaron.
void checkConnections(ServerData * serverData) {
    int i;
    for (i = 0; i < MAX_PLAYERS; i++) {
        if (serverData->connectedBoolean[i] == 1) {
            if (hasBeenDisconnected(i, serverData)) {
                disconnectClient(i, serverData);
            }
        }
    }
}

// Game related functions ----------------------------------------------------------------------

void generateDeck(ServerData * serverData) {
    
    int deckIndex = 0;

    int i;
    int j;
    int k;

    for(k = 0; k < PLAYING_DECKS; k++) {
        for (i = 0; i < SUITS; i++) {
            serverData->deck[deckIndex++] = newCard('A');
            for (j = 2; j <= 10; j++) {
                serverData->deck[deckIndex++] = newCard(j + '0');
            }
            serverData->deck[deckIndex++] = newCard('J');
            serverData->deck[deckIndex++] = newCard('Q');
            serverData->deck[deckIndex++] = newCard('K');
        }
    }
}

void shuffleDeck(ServerData * serverData) {

}

void sendAction(Connection * connection, char action) {

    char * str = malloc(2*sizeof(char));

    str[0] = action;
    str[1] = '\0';

    //sendStr(connection, str);

    free(str);
}

int requestBet() {

    //TODO
    return 0;
}

char requestPlay(Connection * Connection) {

    char ans;

    //sendAction(connection, PLAY);

    //char * str = requestStr(connection);

    //ans = str[0];

    //free(str);

    return ans;
}

void dealCardsToPlayers(ServerData * serverData) {

    int index;

    for(index = 0; index < MAX_PLAYERS; index++) {
        if (serverData->connectedBoolean[index] == 1) {
            if (hasBeenDisconnected(index, serverData)) {
                disconnectClient(index, serverData);
            } else {
                updateClientsOnActive(serverData, index);
                while(serverData->gameTable->playerSeats[index]->score <= MAX_SCORE &&
                    requestPlay(serverData->clientTable[index]) == 'H') {

                    deal(serverData, index);
                }
                updateClientsOnUnActive(serverData, index);
            }
        }
    }
}

void deal(ServerData * serverData, int index) {
    Card * card = serverData->deck[serverData->deckIndex++];
    addCardToSeat(card, serverData->gameTable->playerSeats[index]);
    Deal * deal = newDeal(card->figure, index);
    //updateClientsOnDeal(serverData, deal);
}

void updateClientsOnDeal(ServerData * serverData, Deal * deal) {

    int index;

    for(index = 0; index < MAX_PLAYERS; index++) {
        if (serverData->connectedBoolean[index] == 1) {
            if (hasBeenDisconnected(index, serverData)) {
                disconnectClient(index, serverData);
            } else {
                //sendAction(serverData->clientTable[index], DEAL);
                //sendDeal(serverData->clientTable[index], deal);
            }
        }
    }
}

void updateClientsOnDisconnect(ServerData * serverData, int index) {

    int i;

    for(i = 0; i < MAX_PLAYERS; i++) {
        if (serverData->connectedBoolean[i] == 1) {
            if (hasBeenDisconnected(i, serverData)) {
                disconnectClient(i, serverData);
            } else {
                //sendAction(serverData->clientTable[index], CLEANSEAT);
                //sendInt(index);
            }
        }
    }

}

void updateClientsOnActive(ServerData * serverData, int index) {

}

void updateClientsOnUnActive(ServerData * serverData, int index) {

}