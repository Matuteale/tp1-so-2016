#include "blackjacklib.h"
#include "server.h"

int main() {

    Connection * clientTable[MAX_PLAYERS];
    int connectedBoolean[MAX_PLAYERS] = {0};

    startServer();

    Address * srv = newAddress();
    strcpy(srv->path, SRV_PATH);

    while(1) {
        if (emptySpots(connectedBoolean) > 0) {
            Address * listened = listen(srv);
            if (listened != NULL) {
                printf("Connection detected\n");
                addClient(accept(listened), clientTable, connectedBoolean);

            }
        }
    }

    return 0;
}

int startServer() {

    /* Creating SRV FIFO */
    unlink(SRV_PATH);
    mkfifo(SRV_PATH, 0666);

    open(SRV_PATH, O_RDONLY | O_NONBLOCK);
    
    return 1;
}

int emptySpots(int * connectedBoolean) {
    int i;
    int count = 0;

    for (i = 0; i < MAX_PLAYERS; i++) {
        if (connectedBoolean[i] == 0) {
            count++;
        }
    }

    return count;
}

int firstEmptySpot(int * connectedBoolean) {
    int i;

    for (i = 0; i < MAX_PLAYERS; i++) {
        if (connectedBoolean[i] == 0) {
            return i;
        }
    }

    return -1;
}

void addClient(Connection * connection, Connection ** clientTable, int * connectedBoolean) {

    int index = firstEmptySpot(connectedBoolean);
    clientTable[index] = connection;
    connectedBoolean[index] = 1;
    printf("Client added to spot %d\n", index);
}