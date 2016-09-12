#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "blackjacklib.h"

#define MAX_BUF 1024

int main() {

    int srvfd;
    int clientinfd;
    int clientoutfd;

    char * srvfifo = "/tmp/srv";
    char * clientin;
    char * clientout;
    char * pid;
    char buffer[MAX_BUF];

    clientout = malloc(30);

    /*Obtengo PID y creo PATH para FIFO*/
    pid = malloc(15*sizeof(char));
    sprintf(pid, "%d", getpid());

    clientin = malloc(30);
    strcpy(clientin, srvfifo);
    strcat(clientin, pid);
    strcat(clientin, "CLIENTIN");

    unlink(clientin);
    /* Creo CLIENTIN */
    mkfifo(clientin, 0666);

    /* Abro los fd del SERVER y CLIENTIN */
    printf("Opening Server FIFO.\n");
    srvfd = open(srvfifo, O_WRONLY);
    printf("Opening Client INPUT FIFO.\n");
    clientinfd = open(clientin, O_RDONLY | O_NONBLOCK);

    /* Le aviso al servidor que me quiero conectar */
    printf("Writing Client INPUT file PATH to Server.\n");
    write(srvfd, clientin, MAX_BUF);
    printf("Sending PATH: %s\n", clientin);

    /* Espero a que el servidor me responda*/
    printf("Waiting for Server response.\n");

    while(read(clientinfd, buffer, MAX_BUF) <= 0);
    /*while (read(clientinfd, buffer, MAX_BUF)) {
        sleep(2);
        clearBuffer(buffer, MAX_BUF);
        read(clientinfd, buffer, MAX_BUF);
        printf("%s\n", buffer);
    }*/

    strcpy(clientout, buffer);

    clientoutfd = open(clientout, O_WRONLY | O_NONBLOCK);

    printf("%s\n", clientout);

    write(clientoutfd, "OK", MAX_BUF);

    printf("Connected.\n");

    close(srvfd);
    close(clientinfd);
    close(clientoutfd);

    unlink(clientin);

    return 0;

}