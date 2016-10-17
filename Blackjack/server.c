#include "server.h"

ServerData * serverData;

int main() {

    startLogging();

    validateConfig();

    serverData = newServerData();
    serverData->gameTable = newTable();
    serverData->srvAddress = newComAddress(readStrFromFile("SERVERPATH.txt"));

    logg("Starting server..", 1);
    if(openListener(serverData->srvAddress) < 0){
        logg(strerror(errno), 3);
        logg("Couldn't open server Listener", 3);
        closeServer();
    }
    logg("Server started", 1);

    signal(SIGINT, closeServer);

    logg("Creating database..", 1);
    if(startDatabase(PLAYERS) < 0){
        logg("DataBase Table couldn't be created", 3);
    }else{
        logg("Database started", 1);
    }

    generateDeck();
    shuffleDeck();

    while(1) {
        checkCurrentConnections();
        checkIncomingConnections();
        startRound();
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

int emptySpots() {
    int i;
    int count = 0;

    for (i = 0; i < PLAYERS; i++) {
        if (serverData->connectedBoolean[i] == 0) {
            count++;
        }
    }

    return count;
}

int firstEmptySpot() {
    int i;

    for (i = 0; i < PLAYERS; i++) {
        if (serverData->connectedBoolean[i] == 0) {
            return i;
        }
    }

    return -1;
}

void checkIncomingConnections() {
    int i;
    for (i = 0; i < emptySpots(); i++) {
        if (emptySpots() > 0) {
            ComAddress * listened = comListen(serverData->srvAddress);
            if (listened != NULL) {
                logg("Connection detected", 1);
                addClient(comAccept(listened));
            }
        }
    }
}

void addClient(Connection * connection) {
    logg("Connecting client.." , 1);
    int index = firstEmptySpot();
    serverData->clientTable[index] = connection;
    serverData->connectedBoolean[index] = 1;
    changeSeatMoney(index, STARTING_BALANCE);
    updateBalance(index);

    //LOGGING
    char buf[MAX_BUF];
    sprintf(buf, "Client connected in spot %d", index);
    logg(buf, 1);
}

int disconnectClient(int index) {
    if (serverData->connectedBoolean[index] == 0) {
        return 0;
    }

    logg("Disconnecting client.." , 1);

    disconnect(serverData->clientTable[index]);
    serverData->connectedBoolean[index] = 0;
    clearSeat(serverData->gameTable->seats[index]);
    updateClientsOnIndex(index, CLEARSEAT);
    changeSeatMoney(index, 0);

    //LOGGING
    char buf[MAX_BUF];
    sprintf(buf, "Client in spot %d disconnected", index);
    logg(buf, 1);
    return 1;
}

int hasBeenDisconnected(int index) {
    return !isConnected(serverData->clientTable[index]);
}

int checkConnection(int index) {
    if (serverData->connectedBoolean[index] == 1) {
        if (hasBeenDisconnected(index)) {
            disconnectClient(index);
            return 0;
        }
        return 1;
    }
    return 0;
}

void checkCurrentConnections() {
    int i;
    for (i = 0; i < emptySpots(); i++) {
        checkConnection(i);
    }
}

void closeServer() {
    logg("Closing server..", 1);

    int i;

    updateClientsOn(DISCONNECT);
    for (i = 0; i < PLAYERS; i++) {
        disconnectClient(i);
    }
    deleteServerData(serverData);

    printf("\nServer closed..\n");
    logg("Server closed", 1);
    exit(1);
}

void validateConfig() {

    logg("Validating server configuration..", 1);

    if (DECK_PENETRATION < 0.1 || DECK_PENETRATION > 0.8) {
        fprintf(stderr, "ERROR: INVALID DECK PENETRATION\n");
        logg("INVALID DECK PENETRATION", 3);
        logg("Could not continue, exiting..", 3);
        exit(-1);
    }

    if (STARTING_BALANCE % 1 != 0 || STARTING_BALANCE < 1) {
        fprintf(stderr, "ERROR: INVALID STARTING BALANCE\n");
        logg("INVALID STARTING BALANCE", 3);
        logg("Could not continue, exiting..", 3);
        exit(-1);
    }

    if (PLAYING_DECKS % 1 != 0 || PLAYING_DECKS < 1) {
        fprintf(stderr, "ERROR: INVALID DECKS AMOUNT\n");
        logg("IINVALID DECKS AMOUNT", 3);
        logg("Could not continue, exiting..", 3);
        exit(-1);
    }

    if (PLAYERS % 1 != 0 || PLAYERS < 1 || PLAYERS > (DECK_SIZE * (1.0 - DECK_PENETRATION))/5) {
        fprintf(stderr, "ERROR: INVALID PLAYERS AMOUNT\n");
        logg("INVALID PLAYERS AMOUNT", 3);
        logg("Could not continue, exiting..", 3);
        exit(-1);
    }
    logg("Configuration validated", 1);
}

// Game related functions ----------------------------------------------------------------------

void generateDeck() {

    logg("Generating deck..", 1);
    
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

    logg("Deck generated", 1);
}

void shuffleDeck() {
    serverData->deckIndex = 0;
    updateClientsOn(SHUFFLE);
    shuffle(serverData->deck, DECK_SIZE);

    logg("Deck shuffled", 1);
}

int requestBetTo(int index) {
    int valid = 0;
    int bet;
    sendChar(serverData->clientTable[index], BET);
    while(!valid) {
        bet = requestInt(serverData->clientTable[index]);
        if (bet == 0) {
            return bet;
        }
        if (isBetValid(index, bet)) {
            valid = 1;
            sendStr(serverData->clientTable[index], SUCCESS);
        } else {
            sendStr(serverData->clientTable[index], FAIL);
        }
    }

    return bet;
}

void requestBetToPlayers() {

    logg("Requesting bet to players..", 1);

    int index;

    for(index = 0; index < PLAYERS; index++) {
        if (checkConnection(index)) {
            updateClientsOnIndex(index, SETACTIVE);
            int bet = requestBetTo(index);
            if (bet <= 0) {
                disconnectClient(index);
            } else {
                changeSeatMoney(index, getMoney(index) - bet);
                serverData->gameTable->seats[index]->currentBet = bet;
                Bet * aux = newBet(bet,index);
                updateClientsOnBet(aux);
                deleteBet(aux);
                updateBalance(index);
            }
            updateClientsOnIndex(index, SETUNACTIVE);
        }
    }
}

int isBetValid(int index, int bet) {
    if (bet < 0) {
        return 0;
    }
    if (getMoney(index) < bet) {
        return 0;
    }
    return 1;
}

void updateBalance(int index) {

    //LOGGING
    char buf[MAX_BUF];
    sprintf(buf, "Updating balance on client %d", index);
    logg(buf, 1);

    sendChar(serverData->clientTable[index], UPDATEBALANCE);
    sendInt(serverData->clientTable[index], getMoney(index));
}

char requestPlay(Connection * connection) {

    sendChar(connection, PLAY);
    return requestChar(connection);
}

void askPlayersForHit() {

    logg("Asking players for action..", 1);

    int index;

    for(index = 0; index < PLAYERS; index++) {
        if (checkConnection(index)) {
            updateClientsOnIndex(index, SETACTIVE);
            while(serverData->gameTable->seats[index]->score < MAX_SCORE) {
                char c = requestPlay(serverData->clientTable[index]);
                if (c == 'H') {
                    //LOGGING
                    char buf[MAX_BUF];
                    sprintf(buf, "Player %d hits", index);
                    logg(buf, 1);

                    deal(index);
                } else {
                    if (c == NULL) {
                        disconnectClient(index);
                    }
                    break;
                }
            }
        updateClientsOnIndex(index, SETUNACTIVE);
        }
    }
}

void deal(int index) {

    Card * card = serverData->deck[serverData->deckIndex++];
    addCardToSeat(card, serverData->gameTable->seats[index]);
    Deal * deal = newDeal(card->figure, index);
    updateClientsOnDeal(deal);
    deleteDeal(deal);

    //LOGGING
    char buf[MAX_BUF];
    sprintf(buf, "Card %c dealt to player %d", card->figure, index);
    logg(buf, 1);
}

void updateClientsOnDeal(Deal * deal) {

    logg("Updating clients on deal..", 1);

    int index;

    for(index = 0; index < PLAYERS; index++) {
        if (checkConnection(index)) {
            sendChar(serverData->clientTable[index], DEAL);
            sendDeal(serverData->clientTable[index], deal);
        }
    }
}

void updateClientsOnBet(Bet * bet) {

    logg("Updating clients on bet..", 1);

    int index;

    for(index = 0; index < PLAYERS; index++) {
        if (checkConnection(index)) {
            sendChar(serverData->clientTable[index], UPDATEBET);
            sendBet(serverData->clientTable[index], bet);
        }
    }
}

void updateClientsOn(char action) {

    logg("Updating clients on action..", 1);
    int i;

    for(i = 0; i < PLAYERS; i++) {
        if (checkConnection(i)) {
            sendChar(serverData->clientTable[i], action);
        }
    }
}

void updateClientsOnIndex(int index, char action) {

    logg("Updating clients on action with index..", 1);

    int i;

    for(i = 0; i < PLAYERS; i++) {
        if (checkConnection(i)) {
            sendChar(serverData->clientTable[i], action);
            sendInt(serverData->clientTable[i], index);
        }
    }
}

void payWinners() {

    logg("Paying winners..", 1);

    int i;

    for(i = 0; i < PLAYERS; i++) {
        if (checkConnection(i)) {
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
                    break;
                }
            }
            updateBalance(i);
        }
    }
}

void dealInitialCards() {

    logg("Dealing initial cards..", 1);

    deal(CROUPIER_SEAT);

    int i;
    int j;
    for (j = 0; j < 2; j++) {
        for (i = 0; i < PLAYERS; i++) {
            if (checkConnection(i)) {
                deal(i);
            }
        }
    }
}

void croupierPlay() {

    logg("Croupier is playing..", 1);

    updateClientsOnIndex(CROUPIER_SEAT, SETACTIVE);
    while(serverData->gameTable->seats[CROUPIER_SEAT]->score < CROUPIER_MINSCORE) {
        deal(CROUPIER_SEAT);
    }
    updateClientsOnIndex(CROUPIER_SEAT, SETUNACTIVE);

}

void startRound() {

    if (PLAYERS - emptySpots() > 0) {

        logg("Round is starting..", 1);

        if (hasDeckReachedLimit(serverData->deckIndex)) {
            shuffleDeck();
        }
        clearTable(serverData->gameTable);
        updateClientsOn(CLEARTABLE);
        requestBetToPlayers();
        dealInitialCards();
        askPlayersForHit();
        croupierPlay();
        payWinners();
        sleep(3); //ES ESTETICO PARA QUE LOS CLIENTES PUEDAN VER COMO TERMINO LA RONDA

        logg("Round ended", 1);
    }
}