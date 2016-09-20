#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "blackjacklib.h"
#include <errno.h>
#include "server.h"

int main() {

    struct ClientInfo * clientTable[MAX_PLAYERS];
    int connectedBooleanTable[MAX_PLAYERS] = {0};

    int srvfd;

    startServer(&srvfd);

    while(1) {
        acceptConnections(srvfd, clientTable, connectedBooleanTable);
    }

    return 0;
}

int startServer(int * srvfd) {

    /* Creating SRV FIFO */
    unlink(SRV_PATH);
    mkfifo(SRV_PATH, 0666);

    /* Opening SRV FD */
    *srvfd = open(SRV_PATH, O_RDONLY | O_NONBLOCK);
    
    return 1;
}

int acceptConnections(int srvfd, struct ClientInfo ** clientTable, int * connectedBooleanTable) {

    int index;

    struct ClientInfo * clientInfo;

    char buffer[MAX_BUF];

    /* If Server has reached max capacity */
    if (emptySpots(connectedBooleanTable) == 0) {
        return 0;
    }

    /* If there is nothing to read */
    if (read(srvfd, buffer, MAX_BUF) <= 0) {
        return 0;
    }

    /* Creating ClientInfo for new connection */
    index = firstEmptySpot(connectedBooleanTable);
    clientInfo = createClientInfo(index, clientTable, connectedBooleanTable);

    /* Allocating */
    clientInfo->clientin = malloc(MAX_PATH);
    clientInfo->clientout = malloc(MAX_PATH);

    /* Copying CLIENTIN FIFO PATH to ClientInfo */
    strcpy(clientInfo->clientin, buffer);

    /* Opening CLIENTIN FD */
    clientInfo->clientinfd = open(clientInfo->clientin, O_WRONLY);

    /* Creating CLIENTOUT FIFO PATH */
    strncpy(clientInfo->clientout, clientInfo->clientin, indexOf(clientInfo->clientin,'C'));
    strcat(clientInfo->clientout, "CLIENTOUT");

    /* Creating CLIENTOUT FIFO */
    unlink(clientInfo->clientout);
    mkfifo(clientInfo->clientout, 0666);

    /* Opening CLIENTOUT FD */
    clientInfo->clientoutfd = open(clientInfo->clientout, O_RDONLY | O_NONBLOCK);

    /* Writing CLIENTOUT FIFO PATH to Client */
    write(clientInfo->clientinfd, clientInfo->clientout, MAX_BUF);

    /* Waiting for Client response */
    while(read(clientInfo->clientoutfd, buffer, MAX_BUF) <= 0);
    if (strcmp(buffer, SUCCESS) == 0) {
        strcpy(buffer, SUCCESS); //If i just send SUCCESS in the line below it doesnt work.
        write(clientInfo->clientinfd, buffer, MAX_BUF);
        printf("CLIENT %d CONNECTED\n", index);
        return 1;
    }
    //TODO: ELSE ERROR.
    return 0;

}

struct ClientInfo * createClientInfo(int index, struct ClientInfo ** clientTable, int * connectedBooleanTable) {

    connectedBooleanTable[index] = 1;

    return clientTable[index] = malloc(sizeof(struct ClientInfo));

}


int emptySpots(int connectedBooleanTable[]) {
    int i;
    int count = 0;

    for (i = 0; i < MAX_PLAYERS; i++) {
        if (connectedBooleanTable[i] == 0) {
            count++;
        }
    }

    return count;
}

int firstEmptySpot(int connectedBooleanTable[]) {
    int i;

    for (i = 0; i < MAX_PLAYERS; i++) {
        if (connectedBooleanTable[i] == 0) {
            return i;
        }
    }

    return -1;
}

/*When clients are playing, is should check if the clientin fifo is linked. In case its not it disconnects the player */
int disconnectClient(int clientIndex, struct ClientInfo ** clientTable, int * connectedBooleanTable) {
    if (connectedBooleanTable[clientIndex] == 0) {
        perror("Client was already disconnected.")
        return 0;
    }
    close(clientTable[clientIndex]->clientinfd);
    close(clientTable[clientIndex]->clientoutfd);
    unlink(clientTable[clientIndex]->clientout);
    return 1;
}