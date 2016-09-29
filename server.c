#include "blackjacklib.h"
#include "server.h"
#include <sys/types.h>

ServerData * newServerData() {
    ServerData * serverData = malloc(sizeof(ServerData));
    memset(serverData->connectedBoolean, 0, sizeof(serverData->connectedBoolean));
    fd_set set;
    return serverData;
}

int main() {

    ServerData * serverData = newServerData();

    startServer();

    char * srv = malloc(sizeof(SRV_PATH));
    strcpy(srv, SRV_PATH);
    char * buffer = malloc(MAX_BUF);
    char * buffer2 = malloc(MAX_BUF);
    char * card = malloc(MAX_BUF);

    while(1) {
        if (emptySpots(serverData) > 0) {
            char * listened = comListen(srv);
            if (listened != NULL) {
                printf("Connection detected\n");
                addClient(comAccept(listened), serverData);

            }
        }
        for(int i = 0; i<MAX_PLAYERS; i++) {
            if(serverData->connectedBoolean[i] != 0) {
                snprintf(card, sizeof(int), "%d\n", rand() % 5);
                //printf("%d\n", card);
                //comWrite(serverData->clientTable[i], card, MAX_BUF);
                if(comRead(serverData->clientTable[i], buffer, MAX_BUF) > 0) {
                printf("%s\n", buffer);
                printf("Arriba esta el buffer\n");
                comWrite(serverData->clientTable[i], buffer, MAX_BUF);     
                }
                
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
    char* buffer = malloc(MAX_BUF);
    strcpy(buffer, "Hola\0");
    int index = firstEmptySpot(serverData);
    serverData->clientTable[index] = connection;
    printf("%X\n", connection);
    printf("%X\n", serverData->clientTable[index]);
    serverData->connectedBoolean[index] = 1;
    printf("Client connected in spot %d.\n", index);
    comWrite(serverData->clientTable[index], buffer, MAX_BUF); 
    printf("%X\n", serverData->clientTable[index]);
    printf("input %d\n", serverData->clientTable[index]->inputFD);
    printf("output %d\n", serverData->clientTable[index]->outputFD);
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
    if( access(serverData->clientTable[index]->output, F_OK ) != -1 ) {
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