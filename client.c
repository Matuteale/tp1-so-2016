#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "blackjacklib.h"
#include <errno.h>
#include "client.h"


int main() {

    struct ClientInfo* clientInfo = (struct ClientInfo*) malloc(sizeof(struct ClientInfo));

    requestConnection(SRV_PATH, clientInfo);

    while(1);

    return 0;

}

int requestConnection(char * serverFifo, struct ClientInfo * clientInfo) {

    /* Variables */
    int srvfd;
    char * pid;
    char buffer[MAX_BUF];

    /* Allocating */
    clientInfo->clientin = malloc(MAX_PATH);
    clientInfo->clientout = malloc(MAX_PATH);
    pid = malloc(MAX_PID_LENGTH*sizeof(char));

    /* Obtaining PID string format */
    sprintf(pid, "%d", getpid());

    /* Creating CLIENTIN path */
    strcpy(clientInfo->clientin, serverFifo);
    strcat(clientInfo->clientin, pid);
    strcat(clientInfo->clientin, "CLIENTIN");

    /* Creating CLIENTIN Fifo */
    unlink(clientInfo->clientin);
    mkfifo(clientInfo->clientin, 0666);

    /* Opening SERVER FD */
    srvfd = open(serverFifo, O_WRONLY);
    
    /* Opening CLIENTIN FD */
    clientInfo->clientinfd = open(clientInfo->clientin, O_RDONLY | O_NONBLOCK);

    /* Writing CLIENTIN FIFO PATH to SRV FD, requesting a connection */
    write(srvfd, clientInfo->clientin, MAX_BUF);

    /* Waiting for SRV response */
    while(read(clientInfo->clientinfd, buffer, MAX_BUF) <= 0);

    /* Copying CLIENTOUT FIFO PATH */
    strcpy(clientInfo->clientout, buffer);

    /* Opening CLIENTOUT FD */
    clientInfo->clientoutfd = open(clientInfo->clientout, O_WRONLY);

    /* Requesting SRV confirmation on successful connection */
    strcpy(buffer, SUCCESS); //If i just send SUCCESS in the line below it doesnt work.
    write(clientInfo->clientoutfd, buffer, MAX_BUF);

    /* Waiting for SRV response */ //TODO: FIX WARNINGS, SHOULD COMPARE BYTES.
    while(read(clientInfo->clientinfd, buffer, MAX_BUF) <= 0);
    if (strcmp(buffer, SUCCESS) == 0) {
        printf("CONNECTED\n");
        return 1;
    }
    
    return 0;

}