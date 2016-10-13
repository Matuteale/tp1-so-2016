#include "com.h"
#include "comFIFOs.h"
#include "blackjacklib.h"


Connection * newConnection() {
    Connection * connection = malloc(sizeof(Connection));
    connection->input = malloc(MAX_BUF);
    memset(connection->input, 0, MAX_BUF);
    connection->output = malloc(MAX_BUF);
    memset(connection->output, 0, MAX_BUF);
    return connection;
}


char * comListen(Parameters * params) {

    int inputAuxFD;

    char buffer[MAX_BUF];

    inputAuxFD = open(params->addr, O_RDONLY | O_NONBLOCK);

    if (read(inputAuxFD, buffer, MAX_BUF) <= 0) {
        close(inputAuxFD);
        return NULL;
    }

    char * aux = malloc(MAX_BUF);
    strcpy(aux, buffer);

    close(inputAuxFD);

    return aux;

}

Connection * comAccept(Parameters * params) {

    char buffer[MAX_BUF];

    Connection * connection = newConnection();

    /* Copying CLIENTIN FIFO PATH to Connection Output */
    strcpy(connection->output, params->addr);

    /* Opening OUTPUT (CLIENTIN) FD */
    connection->outputFD = open(connection->output, O_WRONLY);

    /* Creating CLIENTOUT FIFO PATH */
    strncpy(connection->input, connection->output, indexOf(connection->output,'C'));
    strcat(connection->input, "CLIENTOUT");

    /* Creating CLIENTOUT FIFO */
    unlink(connection->input);
    mkfifo(connection->input, 0666);

    /* Opening CLIENTOUT FD */
    connection->inputFD = open(connection->input, O_RDONLY | O_NONBLOCK);
    printf("fffff\n");

    /* Writing CLIENTOUT FIFO PATH to Client */
    write(connection->outputFD, connection->input, MAX_BUF);

    //ACA IRIAN LOS CLOSE
 
    comRead(connection, buffer, MAX_BUF);
    // O ACA
    if (strcmp(buffer, SUCCESS) == 0) {
        strcpy(buffer, SUCCESS); //If i just send SUCCESS in the line below it doesnt work.
        comWrite(connection, buffer, MAX_BUF);
        printf("CONNECTED\n");
        return connection;
    } else {
        unlink(connection->input);
    }

    return NULL;

}

Connection * comConnect(Parameters * params) {

    int outputAuxFD;
    char * auxPID;
    char buffer[MAX_BUF];

    Connection * connection = newConnection();

    /* Allocating */
    auxPID = malloc(MAX_PID_LENGTH*sizeof(char));

    /* Obtaining PID string format */
    sprintf(auxPID, "%d", getpid());

    /* Creating INPUT path */
    strcpy(connection->input, params->addr);
    strcat(connection->input, auxPID);
    strcat(connection->input, "CLIENTIN");

    /* Creating INPUT Fifo */
    unlink(connection->input);
    mkfifo(connection->input, 0666);

    /* Opening AUX OUTPUT FD */
    outputAuxFD = open(params->addr, O_WRONLY);

    /* Opening INPUT FD */
    connection->inputFD = open(connection->input, O_RDONLY | O_NONBLOCK);

    /* Writing INPUT FIFO PATH to AUX OUTPUT FD, requesting a connection */
    write(outputAuxFD, connection->input, MAX_BUF);

    /* Waiting for SRV response */
    while(read(connection->inputFD, buffer, MAX_BUF) <= 0);

    /* Closing FD */
    close(outputAuxFD);

    /* Opening OUTPUT FD */
    strcpy(connection->output, buffer);
    connection->outputFD = open(connection->output, O_WRONLY);

    /* Requesting SRV confirmation on successful connection */
    strcpy(buffer, SUCCESS); //If i just send SUCCESS in the line below it doesnt work.
    comWrite(connection, buffer, MAX_BUF);

    /* Waiting for SRV response */ //TODO: FIX WARNINGS, SHOULD COMPARE BYTES.
    comRead(connection, buffer, MAX_BUF);
    if (strcmp(buffer, SUCCESS) == 0) {
        printf("CONNECTED\n");
        return connection;
    } else {
        unlink(connection->input);
    }
    
    return NULL;
}

int comWrite(Connection * connection, char * dataToWrite, int size) {

    return write(connection->outputFD, dataToWrite, size);

}

int comRead(Connection * connection, char * dataToRead, int size) {

    int ret;

    while(ret = read(connection->inputFD, dataToRead, size) <= 0);
    
    return ret;
}

//TODO: Decidir si hacemos que retorne int segun si desconecta correctamente o lo dejamos asi
void disconnect(Connection * connection) {
    close(connection->inputFD);
    close(connection->outputFD);
    unlink(connection->input);
    free(connection->input);
    free(connection->output);
    free(connection);
}