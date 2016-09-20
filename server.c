#include "blackjacklib.h"
#include "server.h"


ServerData * newServerData() {
    ServerData * serverData = malloc(sizeof(ServerData));
    memset(serverData->connectedBoolean, 0, sizeof(serverData->connectedBoolean));
    return serverData;
}

int main() {

    ServerData * serverData = newServerData();

    startServer();

    Address * srv = newAddress();
    strcpy(srv->path, SRV_PATH);

    while(1) {
        if (emptySpots(serverData) > 0) {
            Address * listened = listen(srv);
            if (listened != NULL) {
                printf("Connection detected\n");
                addClient(accept(listened), serverData);

            }
        }
        checkConnections(serverData);
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
    printf("Client in spot %d disconnected.\n", index);
    return 1;
}

int hasBeenDisconnected(int index, ServerData * serverData) {
    if( access(serverData->clientTable[index]->output->path, F_OK ) != -1 ) {
        return 0;
    }
    return 1;
}

//TODO: Remover, ya que solo la usamos ahora para probar la forma de desconectar clientes, despues vamos a checkear cuando esten jugando en su turno si se desconectaron.
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