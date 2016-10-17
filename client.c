#include "blackjacklib.h"
#include "client.h"

ClientData * clientData;

int main() {

    ComAddress * srvAddress = newComAddress(readStrFromFile("SERVERPATH.txt"));

    clientData = newClientData();

    signal(SIGINT, disconnectClient);

    clientData->serverConnection = comConnect(srvAddress);

    if (clientData->serverConnection == NULL) {
        deleteClientData(clientData);
    }
    clientData->gameTable = newTable();

    while(1) {
        clearScreen();
        showTable(clientData->gameTable);
        waitForServer(clientData);
        usleep(CLOCK);
    }

    return 0;

}

ClientData * newClientData() {
    ClientData * clientData = malloc(sizeof(ClientData));
    return clientData;
}

void deleteClientData(ClientData * clientData) {
    free(clientData);
}

void askToQuit(ClientData * clientData) {

    char c;

    while(!feof(stdin)) {
        if ((c = getchar()) == 'q') {
            if ((c = getchar()) == '\n') {
                disconnectClient();
            }
        }
        while((c = getchar()) != '\n' && !feof(stdin));
    }
}

void disconnectClient() {
    printf("\n");
    sendChar(clientData->serverConnection, NULL);
    sendInt(clientData->serverConnection,0);
    disconnect(clientData->serverConnection);
    deleteTable(clientData->gameTable);
    deleteClientData(clientData);
    printf("Closing Client..\n");
    exit(1);
}

void waitForServer(ClientData * clientData) {

    printf("Waiting for turn...\n");

    char c = requestChar(clientData->serverConnection);
    //DEBUG printf("ACTION SENT: %c\n", c);
    switch(c) {
        case BET: {
            bet(clientData);
            break;
        }
        case PLAY: {
            play(clientData);
            break;
        }
        case DEAL: {
            deal(clientData);
            break;
        }
        case CLEARSEAT: {
            clearSeatAction(clientData);
            break;
        }
        case CLEARTABLE: {
            clearTableAction(clientData);
            break;
        }
        case SHUFFLE: {
            shuffleAction(clientData);
            break;
        }
        case SETACTIVE: {
            setActiveAction(clientData);
            break;
        }
        case SETUNACTIVE: {
            setUnActiveAction(clientData);
            break;
        }
        case UPDATEBALANCE: {
            updateBalance(clientData);
            break;
        }
        case UPDATEBET: {
            updateBet(clientData);
            break;
        }
    }
}

void bet(ClientData * clientData) {

    //struct timeval start, stop;
    //gettimeofday(&start, NULL);

    int valid = 0;

    while(!valid) {
        printf("Your Balance: %d\n", clientData->balance);
        printf("Place your bet: (or type 'q' to quit)\n");
        char * str = getStr(MAX_DIGITS);
        if (str != NULL) {
            if(str[0] == 'q' && str[1] == '\0' ) {
                free(str);
                sendInt(clientData->serverConnection, 0);
                disconnectClient();
            }

            int value = strToInt(str);
            free(str);
            if (value != NULL && value > 0) {
                sendInt(clientData->serverConnection, value);
                if (strcmp(requestStr(clientData->serverConnection), SUCCESS) == 0) {
                    valid = 1;
                }
            }
        }
        if (!valid) {
            fprintf(stderr, "Invalid amount. Try again.\n");
        }
    }
}

void play(ClientData * clientData) {

    char ans;
    do {
        printf("Enter an action:  'H' | HIT -- 'S' | STAND\n");
        ans = getCharacter();
        if (ans != NULL) {
            ans = toUpper(ans);
        }
    } while(ans == NULL || (ans != 'H' && ans != 'S'));

    sendChar(clientData->serverConnection, ans);
}

void deal(ClientData * clientData) {

    Deal * deal = requestDeal(clientData->serverConnection);
    addCardToSeat(newCard(deal->card), clientData->gameTable->seats[deal->playerNumber]);
    deleteDeal(deal);
}

void clearSeatAction(ClientData * clientData) {

    int playerNumber = requestInt(clientData->serverConnection);

    clearSeat(clientData->gameTable->seats[playerNumber]);
}

void clearTableAction(ClientData * clientData) {

    clearTable(clientData->gameTable);
}

void shuffleAction(ClientData * clientData) {

    printf("Deck is being Shuffled ...");

    sleep(SCREEN_TIME); //SOLO ESTETICO
}

void setActiveAction(ClientData * clientData) {

    int playerNumber = requestInt(clientData->serverConnection);

    setActive(clientData->gameTable->seats[playerNumber]);
}

void setUnActiveAction(ClientData * clientData) {

    int playerNumber = requestInt(clientData->serverConnection);

    setUnActive(clientData->gameTable->seats[playerNumber]);
}

void updateBalance(ClientData * clientData) {

    clientData->balance = requestInt(clientData->serverConnection);
}

void updateBet(ClientData * clientData) {
    Bet * bet = requestBet(clientData->serverConnection);

    clientData->gameTable->seats[bet->playerNumber]->currentBet = bet->bet;

    deleteBet(bet);
}