#include "blackjacklib.h"
#include "client.h"

int main() {

    char * srvaddr = malloc(sizeof(SRV_PATH));

    ClientData * clientData = newClientData();

    strcpy(srvaddr, SRV_PATH);
    clientData->serverConnection = comConnect(srvaddr);
    clientData->gameTable = newTable();

    while(1) {
        bet(clientData);

        /* LO QUE VA DENTRO DEL WHILE POSTA
        clearScreen();
        showTable();
        waitForServer(clientData);
        */
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
                disconnectClient(clientData);
            }
        }
        while((c = getchar()) != '\n' && !feof(stdin));
    }
}

void disconnectClient(ClientData * clientData) {
    disconnect(clientData->serverConnection);
    deleteTable(clientData->gameTable);
    deleteClientData(clientData);
    exit(1);
}

void waitForServer(ClientData * clientData) {

    char * buffer;

    buffer = malloc(sizeof(char) * MAX_BUF);

    while(comRead(clientData->serverConnection, buffer,MAX_BUF)<= 0);

    switch(buffer[0]) {
        case 'A': {
            bet(clientData);
            break;
        }
        case 'B': {
            play(clientData);
            break;
        }
        case 'C': {
            deal(clientData);
            break;
        }
        case 'D': {
            clearSeatAction(clientData);
            break;
        }
        case 'E': {
            clearTableAction(clientData);
            break;
        }
        case 'F': {
            shuffleAction();
            break;
        }
        case 'G': {
            setActiveAction(clientData);
            break;
        }
        case 'H': {
            setUnActiveAction(clientData);
            break;
        }
    }

    free(buffer);
}

void bet(ClientData * clientData) {

    struct timeval start, stop;
    gettimeofday(&start, NULL);

    //updateBalance(clientData);

    int valid = 0;

    while(!valid) {
        //printf("Your Balance: %d\n", clientData->balance);
        printf("Place your bet: (or type 'q' to quit)\n");
        char * str = getStr(MAX_DIGITS);
        if (str != NULL) {
            if(str[0] == 'q' && str[1] == '\0' ) {
                free(str);
                disconnectClient(clientData);
            }

            int value = strToInt(str);
            free(str);
            if (value != NULL) {
                printf("ENVIANDO: %d\n", value);
                /*if (sendInt(value)) {
                    valid = 1;
                }*/
            }
        }
        if (!valid) {
            fprintf(stderr, "Invalid amount. Try again.\n");
        }
    }
}

void play(ClientData * clientData) {

    char * ans = NULL;
    while(strcmp(ans, "H") != 0 && strcmp(ans,"S") != 0) {
        printf("Enter an action:  'H' | HIT -- 'S' | STAND\n");
        ans = getStr(1);
    }

    //sendStr(ans);
}

void deal(ClientData * clientData) {

    //Deal * deal = requestDeal(clientData->serverConnection);

    //addCardToSeat(newCard(deal->card), clientData->gameTable->playerSeats[deal->playerNumber]);

    //deleteDeal(deal);
}

void clearSeatAction(ClientData * clientData) {

    //int playerNumber = requestInt(clientData->serverConnection);

    //clearSeat(clientData->playerSeats[playerNumber]);
}

void clearTableAction(ClientData * clientData) {

    clearTable(clientData->gameTable);
}

void shuffleAction() {

    printf("Deck is being Shuffled ...");

    //TODO: VER SI AGREGAR UN WAIT DE 3 SEGUNDOS ROMPE ALGO. ES SOLO ESTETICO.
}

void setActiveAction(ClientData * clientData) {

    //int playerNumber = requestInt(clientData->serverConnection);

    //setActive(clientData->gameTable->playerSeats[playerNumber]);
}

void setUnActiveAction(ClientData * clientData) {

    //int playerNumber = requestInt(clientData->serverConnection);

    //setUnActive(clientData->gameTable->playerSeats[playerNumber]);
}