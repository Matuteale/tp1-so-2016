#include "blackjacklib.h"
#include "server.h"
#include <sys/types.h>

int main() {

    ServerData * serverData = newServerData();
    serverData->gameTable = newTable();
    strcpy(serverData->srvpath, SRV_PATH);

    startServer(serverData);

    while(1) {
        checkCurrentConnections(serverData);
        checkIncomingConnections(serverData);
        startRound(serverData);
    }

    return 0;
}

ServerData * newServerData() {
    ServerData * serverData = malloc(sizeof(ServerData));
    serverData->srvpath = malloc(sizeof(SRV_PATH));
    memset(serverData->connectedBoolean, 0, sizeof(serverData->connectedBoolean));
    return serverData;
}

void deleteServerData(ServerData * serverData) {
    free(serverData->srvpath);
    free(serverData);
}

// Connection related functions ---------------------------------------------------------------

int startServer(ServerData * serverData) {

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

void checkIncomingConnections(ServerData * serverData) {
    int i;
    for (i = 0; i < emptySpots(serverData); i++) {
        if (emptySpots(serverData) > 0) {
            char * listened = comListen(serverData->srvpath);
            if (listened != NULL) {
                printf("Connection detected\n");
                addClient(comAccept(listened), serverData);
            }
        }
    }
}

void addClient(Connection * connection, ServerData * serverData) {
    int index = firstEmptySpot(serverData);
    serverData->clientTable[index] = connection;
    serverData->connectedBoolean[index] = 1;
    serverData->balance[index] = STARTING_MONEY;
    printf("Client connected in spot %d.\n", index);
}

int disconnectClient(int index, ServerData * serverData) {
    if (serverData->connectedBoolean[index] == 0) {
        return 0;
    }
    disconnect(serverData->clientTable[index]);
    serverData->connectedBoolean[index] = 0;
    clearSeat(serverData->gameTable->seats[index]);
    updateClientsOn(serverData, index, CLEARSEAT);
    printf("Client in spot %d disconnected.\n", index);
    return 1;
}

int hasBeenDisconnected(int index, ServerData * serverData) {
    if( access(serverData->clientTable[index]->output, F_OK ) != -1 ) {
        return 0;
    }
    return 1;
}

void checkCurrentConnections(ServerData * serverData) {
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

int requestBet(ServerData * serverData, int index) {
    int valid = 0;
    int bet;
    sendChar(serverData->clientTable[index], BET);
    printf("D\n");
    while(!valid) {
        bet = requestInt(serverData->clientTable[index]);
        printf("D\n");
        if (isBetValid(serverData, index, bet)) {
            valid = 1;
            sendStr(serverData->clientTable[index], SUCCESS);
        }
    }

    //TODO: IF TIMEOUT -> RETURN 0; ESTO HACE QUE DESCONECTE AL CLIENTE.

    return bet;
}

void requestBetToPlayers(ServerData * serverData) {

    int index;

    for(index = 0; index < MAX_PLAYERS; index++) {
        if (serverData->connectedBoolean[index] == 1) {
            if (hasBeenDisconnected(index, serverData)) {
                disconnectClient(index, serverData);
            } else {
                updateClientsOn(serverData, index, SETACTIVE);
                int bet = requestBet(serverData, index);
                printf("END OF REQUESTBET\n");
                if (bet <= 0) {
                    disconnectClient(index, serverData);
                } else {
                    serverData->balance[index] -= bet;
                    updateBalance(serverData, index);
                }
                updateClientsOn(serverData, index, SETUNACTIVE);
            }
        }
    }
}

int isBetValid(ServerData * serverData, int index, int bet) {
    if (bet < 0) {
        return 0;
    }
    if (serverData->balance[index] < bet) {
        return 0;
    }
    return 1;
}

void updateBalance(ServerData * serverData, int index) {
    sendChar(serverData->clientTable[index], UPDATEBALANCE);
    sendInt(serverData->clientTable[index], serverData->balance[index]);
}

char requestPlay(Connection * connection) {

    char ans;

    sendChar(connection, PLAY);

    char * str = requestStr(connection);

    ans = str[0];

    free(str);

    return ans;
}

void askPlayerForHit(ServerData * serverData) {

    int index;

    for(index = 0; index < MAX_PLAYERS; index++) {
        if (serverData->connectedBoolean[index] == 1) {
            if (hasBeenDisconnected(index, serverData)) {
                disconnectClient(index, serverData);
            } else {
                updateClientsOn(serverData, index, SETACTIVE);
                while(serverData->gameTable->seats[index]->score <= MAX_SCORE &&
                    requestPlay(serverData->clientTable[index]) == 'H') {

                    deal(serverData, index);
                }
                updateClientsOn(serverData, index, SETUNACTIVE);
            }
        }
    }
}

void deal(ServerData * serverData, int index) {
    Card * card = serverData->deck[serverData->deckIndex++];
    addCardToSeat(card, serverData->gameTable->seats[index]);
    Deal * deal = newDeal(card->figure, index);
    updateClientsOnDeal(serverData, deal);
}

void updateClientsOnDeal(ServerData * serverData, Deal * deal) {

    int index;

    for(index = 0; index < MAX_PLAYERS; index++) {
        if (serverData->connectedBoolean[index] == 1) {
            if (hasBeenDisconnected(index, serverData)) {
                disconnectClient(index, serverData);
            } else {
                sendChar(serverData->clientTable[index], DEAL);
                //sendDeal(serverData->clientTable[index], deal);
            }
        }
    }
}

void updateClientsOnShuffle(ServerData * serverData) {

    int i;

    for(i = 0; i < MAX_PLAYERS; i++) {
        if (serverData->connectedBoolean[i] == 1) {
            if (hasBeenDisconnected(i, serverData)) {
                disconnectClient(i, serverData);
            } else {
                sendChar(serverData->clientTable[i], SHUFFLE);
            }
        }
    }
}

void updateClientsOn(ServerData * serverData, int index, char action) {

    int i;

    for(i = 0; i < MAX_PLAYERS; i++) {
        if (serverData->connectedBoolean[i] == 1) {
            if (hasBeenDisconnected(i, serverData)) {
                disconnectClient(i, serverData);
            } else {
                sendChar(serverData->clientTable[i], action);
                sendInt(serverData->clientTable[i], index);
            }
        }
    }
}

void payWinners(ServerData * serverData) {

    int i;

    for(i = 0; i < MAX_PLAYERS; i++) {
        if (serverData->connectedBoolean[i] == 1) {
            if (hasBeenDisconnected(i, serverData)) {
                disconnectClient(i, serverData);
            } else {
                if(hasWon(serverData->gameTable->seats[i], 
                    serverData->gameTable->seats[CROUPIER_SEAT])) {
                    serverData->balance[i] +=
                        (serverData->gameTable->seats[i]->currentBet *2);
                    updateBalance(serverData, i);
                }
            }
        }
    }
}

int hasWon(Seat * seat, int croupierScore) {
    if (seat->score > 21) {
        return 0;
    }
    if (seat->score < croupierScore) {
        return 0;
    }
    return 1;
}

void dealInitialCards(ServerData * serverData) {

printf("B\n");
    deal(serverData, CROUPIER_SEAT);
printf("B\n");
    int i;
    int j;
    for (j = 0; j < 2; j++) {
        for (i = 0; i < MAX_PLAYERS; i++) {
            deal(serverData, i);
        }
    }
}

void croupierPlay(ServerData * serverData) {
    updateClientsOn(serverData, CROUPIER_SEAT, SETACTIVE);
    while(serverData->gameTable->seats[CROUPIER_SEAT]->score < 17) {
        deal(serverData, CROUPIER_SEAT);
    }
    updateClientsOn(serverData, CROUPIER_SEAT, SETUNACTIVE);

}

void startRound(ServerData * serverData) {
    if (MAX_PLAYERS - emptySpots(serverData) > 0) {
        if (hasDeckReachedLimit(serverData->deckIndex)) {
            clearTable(serverData->gameTable);
            updateClientsOnShuffle(serverData);
            shuffleDeck(serverData);
        }
        printf("A\n");
        requestBetToPlayers(serverData);
        printf("A\n");
        dealInitialCards(serverData);
        askPlayerForHit(serverData);
        croupierPlay(serverData);
        payWinners(serverData);
    }
}