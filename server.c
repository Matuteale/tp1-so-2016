#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "blackjacklib.h"

#define MAX_BUF 1024

int main() {
	
    char * srvfifo= "/tmp/srv";
    char * clientin;
    char * clientout;

    char buffer[MAX_BUF];

    int srvfd;
    int clientinfd;
    int clientoutfd;

    clientin = malloc(30); //NO MAGIC NUMBERS
    clientout = malloc(30);

    unlink(srvfifo);
    /* Creo la FIFO de entrada del Servidor */
    mkfifo(srvfifo, 0666);

    /* Creo la FIFO de SALIDA del cliente */
    //mkfifo(clientout, 0666);

    srvfd = open(srvfifo, O_RDONLY);


    printf("Reading Client INPUT file path.\n");
    read(srvfd, buffer, MAX_BUF);
    printf("Path: %s\n", buffer);

    strcpy(clientin, buffer);

    clearBuffer(buffer, MAX_BUF);

    clientinfd = open(clientin, O_WRONLY);

    /* Creo el PATH del CLIENTOUT */
    strncpy(clientout, clientin, indexOf(clientin,'C'));
    strcat(clientout, "CLIENTOUT");

    unlink(clientout);
    mkfifo(clientout, 0666);

    clientoutfd = open(clientout, O_RDONLY | O_NONBLOCK);

    printf("Writing in Client INPUT: %s\n", clientout);
    write(clientinfd, clientout, MAX_BUF);

    clearBuffer(buffer, MAX_BUF);
    while(read(clientoutfd, buffer, MAX_BUF) <= 0);

    printf("%s\n", buffer);

    close(srvfd);
    close(clientinfd);
    close(clientoutfd);

    /* Remuevo la FIFO */
    unlink(srvfifo);
    unlink(clientout);

    return 0;
}