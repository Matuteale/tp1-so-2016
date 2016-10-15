#include "blackjacklib.h"
#include "server.h"
#include "logging.h"

ServerData * serverData;

int main() {
    msgctl(msgget(key, msgflg), IPC_RMID, NULL); //Deletes de msg queue
    msqid = msgget(key, msgflg);

    validateConfig();

    serverData = newServerData();
    serverData->gameTable = newTable();
    serverData->srvAddress = newComAddress(readStrFromFile("SERVERPATH.txt"));

    logging("Starting server...", 1);
    if(startServer(serverData) < 0){
        logging("Could not start server, exiting...", 3);
        exit(0);
    }
    logging("Server started.", 1);
    signal(SIGINT, closeServer);

    openListener(serverData->srvAddress);

    startDatabase(PLAYERS);
    generateDeck(serverData);
    shuffleDeck(serverData);

    while(1) {
        checkCurrentConnections(serverData);
        checkIncomingConnections(serverData);
        startRound(serverData);
    }

    return 0;
}

void logging(char * msg, int type){
    message_buf sbuf;
    sbuf.mtype = type;
    (void) strcpy(sbuf.mtext, msg);
    msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, IPC_NOWAIT);
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

int startServer(ServerData * serverData) {

    /* Creating SRV FIFO */
    unlink(serverData->srvAddress->path);
    mkfifo(serverData->srvAddress->path, 0666);

    logging("Opening server path...", 1);
    if(open(serverData->srvAddress->path, O_RDONLY | O_NONBLOCK) < 0){
        logging(strerror(errno), 3);
        return -1;
    }
    logging("Server path successfully opened.", 1);

    return 0;
}

int emptySpots(ServerData * serverData) {
    int i;
    int count = 0;

    for (i = 0; i < PLAYERS; i++) {
        if (serverData->connectedBoolean[i] == 0) {
            count++;
        }
    }

    return count;
}

int firstEmptySpot(ServerData * serverData) {
    int i;

    for (i = 0; i < PLAYERS; i++) {
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
            ComAddress * listened = comListen(serverData->srvAddress);
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
    changeSeatMoney(index, STARTING_BALANCE);
    updateBalance(serverData, index);
    printf("Client connected in spot %d.\n", index);
}

int disconnectClient(ServerData * serverData, int index) {
    if (serverData->connectedBoolean[index] == 0) {
        return 0;
    }
    disconnect(serverData->clientTable[index]);
    serverData->connectedBoolean[index] = 0;
    clearSeat(serverData->gameTable->seats[index]);
    updateClientsOnIndex(serverData, index, CLEARSEAT);
    changeSeatMoney(index, 0);
    printf("Client in spot %d disconnected.\n", index); //DEBUG SERVER
    return 1;
}

int hasBeenDisconnected(ServerData * serverData, int index) {
    return !isConnected(serverData->clientTable[index]->output);
}

int checkConnection(ServerData * serverData, int index) {
    if (serverData->connectedBoolean[index] == 1) {
        if (hasBeenDisconnected(serverData, index)) {
            disconnectClient(serverData, index);
            return 0;
        }
        return 1;
    }
    return 0;
}

void checkCurrentConnections(ServerData * serverData) {
    int i;
    for (i = 0; i < emptySpots(serverData); i++) {
        checkConnection(serverData, i);
    }
}

void closeServer() {
    int i;
    printf("\n");
    for (i = 0; i < PLAYERS; i++) {
        disconnectClient(serverData, i);
    }
    deleteServerData(serverData);
    printf("Closing Server..\n");
    exit(1);
}

void validateConfig() {

    if (DECK_PENETRATION < 0.1 || DECK_PENETRATION > 0.8) {
        fprintf(stderr, "ERROR: INVALID DECK PENETRATION\n");
        exit(-1);
    }

    if (STARTING_BALANCE % 1 != 0 || STARTING_BALANCE < 1) {
        fprintf(stderr, "ERROR: INVALID STARTING BALANCE\n");
        exit(-1);
    }

    if (PLAYING_DECKS % 1 != 0 || PLAYING_DECKS < 1) {
        fprintf(stderr, "ERROR: INVALID DECKS AMOUNT\n");
        exit(-1);
    }

    if (PLAYERS % 1 != 0 || PLAYERS < 1 || PLAYERS > (DECK_SIZE * (1.0 - DECK_PENETRATION))/5) {
        fprintf(stderr, "ERROR: INVALID PLAYERS AMOUNT\n");
        exit(-1);
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
            for (j = 2; j <= 9; j++) {
                serverData->deck[deckIndex++] = newCard(j + '0');
            }
            serverData->deck[deckIndex++] = newCard('0'); //ESTOS SON LOS 10
            serverData->deck[deckIndex++] = newCard('J');
            serverData->deck[deckIndex++] = newCard('Q');
            serverData->deck[deckIndex++] = newCard('K');
        }
    }
}

void shuffleDeck(ServerData * serverData) {
    serverData->deckIndex = 0;
    updateClientsOn(serverData, SHUFFLE);
    shuffle(serverData->deck, DECK_SIZE);
}

int requestBetTo(ServerData * serverData, int index) {
    int valid = 0;
    int bet;
    sendChar(serverData->clientTable[index], BET);
    while(!valid) {
        bet = requestInt(serverData->clientTable[index]);
        if (bet == 0) {
            return bet;
        }
        if (isBetValid(serverData, index, bet)) {
            valid = 1;
            sendStr(serverData->clientTable[index], SUCCESS);
        } else {
            sendStr(serverData->clientTable[index], FAIL);
        }
    }

    //TODO: EN REQUESTINT IF TIMEOUT -> RETURN 0; ESTO HACE QUE DESCONECTE AL CLIENTE.

    return bet;
}

void requestBetToPlayers(ServerData * serverData) {

    int index;

    for(index = 0; index < PLAYERS; index++) {
        if (checkConnection(serverData, index)) {
            updateClientsOnIndex(serverData, index, SETACTIVE);
            int bet = requestBetTo(serverData, index);
            if (bet <= 0) {
                disconnectClient(serverData, index);
            } else {
                changeSeatMoney(index, getMoney(index) - bet);
                serverData->gameTable->seats[index]->currentBet = bet;
                Bet * aux = newBet(bet,index);
                updateClientsOnBet(serverData, aux);
                deleteBet(aux);
                updateBalance(serverData, index);
            }
            updateClientsOnIndex(serverData, index, SETUNACTIVE);
        }
    }
    readTable(); //TODO: REMOVER
}

int isBetValid(ServerData * serverData, int index, int bet) {
    if (bet < 0) {
        return 0;
    }
    if (getMoney(index) < bet) {
        return 0;
    }
    return 1;
}

void updateBalance(ServerData * serverData, int index) {
    sendChar(serverData->clientTable[index], UPDATEBALANCE);
    sendInt(serverData->clientTable[index], getMoney(index));
}

char requestPlay(Connection * connection) {

    char ans;

    sendChar(connection, PLAY);
    char * str = requestStr(connection);
    ans = str[0];

    free(str);
    return ans;
}

void askPlayersForHit(ServerData * serverData) {

    int index;

    for(index = 0; index < PLAYERS; index++) {
        if (checkConnection(serverData, index)) {
            updateClientsOnIndex(serverData, index, SETACTIVE);
            while(serverData->gameTable->seats[index]->score <= MAX_SCORE &&
                requestPlay(serverData->clientTable[index]) == 'H') {
                deal(serverData, index);
            }
        updateClientsOnIndex(serverData, index, SETUNACTIVE);
        }
    }
}

void deal(ServerData * serverData, int index) {
    Card * card = serverData->deck[serverData->deckIndex++];
    addCardToSeat(card, serverData->gameTable->seats[index]);
    Deal * deal = newDeal(card->figure, index);
    updateClientsOnDeal(serverData, deal);
    deleteDeal(deal);
}

void updateClientsOnDeal(ServerData * serverData, Deal * deal) {

    int index;

    for(index = 0; index < PLAYERS; index++) {
        if (checkConnection(serverData, index)) {
            sendChar(serverData->clientTable[index], DEAL);
            sendDeal(serverData->clientTable[index], deal);
        }
    }
}

void updateClientsOnBet(ServerData * serverData, Bet * bet) {

    int index;

    for(index = 0; index < PLAYERS; index++) {
        if (checkConnection(serverData, index)) {
            sendChar(serverData->clientTable[index], UPDATEBET);
            sendBet(serverData->clientTable[index], bet);
        }
    }
}

void updateClientsOn(ServerData * serverData, char action) {

    int i;

    for(i = 0; i < PLAYERS; i++) {
        if (checkConnection(serverData, i)) {
            sendChar(serverData->clientTable[i], action);
        }
    }
}

void updateClientsOnIndex(ServerData * serverData, int index, char action) {

    int i;

    for(i = 0; i < PLAYERS; i++) {
        if (checkConnection(serverData, i)) {
            sendChar(serverData->clientTable[i], action);
            sendInt(serverData->clientTable[i], index);
        }
    }
}

void payWinners(ServerData * serverData) {

    int i;

    for(i = 0; i < PLAYERS; i++) {
        if (checkConnection(serverData, i)) {
            switch(hasWon(serverData->gameTable->seats[i], 
                serverData->gameTable->seats[CROUPIER_SEAT]->score)) {

                case LOSE: {
                    break;
                }
                case DRAW: {
                    changeSeatMoney(i, getMoney(i) +
                        serverData->gameTable->seats[i]->currentBet);
                    break;
                }
                case WIN: {
                    changeSeatMoney(i, getMoney(i) +
                        serverData->gameTable->seats[i]->currentBet*2);
                }
            }
            updateBalance(serverData, i);
        }
    }
}

void dealInitialCards(ServerData * serverData) {

    deal(serverData, CROUPIER_SEAT);

    int i;
    int j;
    for (j = 0; j < 2; j++) {
        for (i = 0; i < PLAYERS; i++) {
            if (checkConnection(serverData, i)) {
                deal(serverData, i);
            }
        }
    }
}

void croupierPlay(ServerData * serverData) {
    updateClientsOnIndex(serverData, CROUPIER_SEAT, SETACTIVE);
    while(serverData->gameTable->seats[CROUPIER_SEAT]->score < CROUPIER_MINSCORE) {
        deal(serverData, CROUPIER_SEAT);
    }
    updateClientsOnIndex(serverData, CROUPIER_SEAT, SETUNACTIVE);

}

void startRound(ServerData * serverData) {
    if (PLAYERS - emptySpots(serverData) > 0) {
        if (hasDeckReachedLimit(serverData->deckIndex)) {
            shuffleDeck(serverData);
        }
        clearTable(serverData->gameTable);
        updateClientsOn(serverData, CLEARTABLE);
        requestBetToPlayers(serverData);
        dealInitialCards(serverData);
        askPlayersForHit(serverData);
        croupierPlay(serverData);
        payWinners(serverData);
        sleep(3); //ES ESTETICO PARA QUE LOS CLIENTES PUEDAN VER COMO TERMINO LA RONDA
    }
}