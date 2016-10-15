#include "com.h"
#include "comFIFOs.h"

#define MAX_BUF 1024
#define MAX_PID_LENGTH 20

// MSGS CODES
#define SUCCESS "1"
#define FAIL "0"

Connection * newConnection() {
    Connection * connection = malloc(sizeof(Connection));
    return connection;
}

ComAddress * newComAddress(char * path) {
    ComAddress * address = malloc(sizeof(ComAddress));
    address->path = path;
    return address;
}

void deleteComAddress(ComAddress * address) {
    free(address->path);
    free(address);
}


ComAddress * comListen(ComAddress * address) {

    int inputAuxFD;

    char buffer[MAX_BUF];

    inputAuxFD = open(address->path, O_RDONLY | O_NONBLOCK);

    if (read(inputAuxFD, buffer, MAX_BUF) <= 0) {
        close(inputAuxFD);
        return NULL;
    }

    char * path = malloc(strlen(buffer)+1);
    strcpy(path, buffer);

    close(inputAuxFD);

    ComAddress * aux = newComAddress(path);

    return aux;
}

Connection * comAccept(ComAddress * addressToAccept) {

    char buffer[MAX_BUF];
    char * inputPath;

    Connection * connection = newConnection();

    /* Copying CLIENTIN FIFO PATH to Connection Output */
    connection->output = addressToAccept;

    /* Opening OUTPUT (CLIENTIN) FD */
    connection->outputFD = open(connection->output->path, O_WRONLY);

    /* Creating CLIENTOUT FIFO PATH */
    inputPath = malloc(strlen(connection->output->path)+2);
    strncpy(inputPath, connection->output->path, indexOf(connection->output->path,'C'));
    strcat(inputPath, "CLIENTOUT");
    connection->input = newComAddress(inputPath);

    /* Creating CLIENTOUT FIFO */
    unlink(connection->input->path);
    mkfifo(connection->input->path, 0666);

    /* Opening CLIENTOUT FD */
    connection->inputFD = open(connection->input->path, O_RDWR);

    /* Writing CLIENTOUT FIFO PATH to Client */
    write(connection->outputFD, connection->input->path, MAX_BUF);
 
    comRead(connection, buffer, MAX_BUF);

    if (strcmp(buffer, SUCCESS) == 0) {
        comWrite(connection, buffer, MAX_BUF);
        printf("CONNECTED\n");
        return connection;
    } else {
        disconnect(connection);
    }

    return NULL;

}

Connection * comConnect(ComAddress * address) {

    int outputAuxFD;
    char * auxPID;
    char * inputPath;
    char * outputPath;
    char buffer[MAX_BUF];

    Connection * connection = newConnection();

    /* Allocating */
    auxPID = malloc(MAX_PID_LENGTH*sizeof(char));

    /* Obtaining PID string format */
    sprintf(auxPID, "%d", getpid());

    /* Creating INPUT path */
    inputPath = malloc(strlen(address->path) + strlen(auxPID) + strlen("CLIENTIN") +1);
    strcpy(inputPath, address->path);
    strcat(inputPath, auxPID);
    free(auxPID);
    strcat(inputPath, "CLIENTIN");
    connection->input = newComAddress(inputPath);

    /* Creating INPUT Fifo */
    unlink(connection->input->path);
    mkfifo(connection->input->path, 0666);

    /* Opening AUX OUTPUT FD */
    outputAuxFD = open(address->path, O_WRONLY);

    /* Opening INPUT FD */
    connection->inputFD = open(connection->input->path, O_RDWR);

    /* Writing INPUT FIFO PATH to AUX OUTPUT FD, requesting a connection */
    write(outputAuxFD, connection->input->path, MAX_BUF);

    /* Waiting for SRV response */
    while(read(connection->inputFD, buffer, MAX_BUF) <= 0);

    /* Closing FD */
    close(outputAuxFD);

    /* Opening OUTPUT FD */
    outputPath = malloc(strlen(buffer)+1);
    strcpy(outputPath, buffer);
    connection->output = newComAddress(outputPath);
    connection->outputFD = open(connection->output->path, O_WRONLY);

    /* Requesting SRV confirmation on successful connection */
    strcpy(buffer, SUCCESS);
    comWrite(connection, buffer, MAX_BUF);

    /* Waiting for SRV response */
    comRead(connection, buffer, MAX_BUF);
    if (strcmp(buffer, SUCCESS) == 0) {
        printf("CONNECTED\n");
        return connection;
    } else {
        disconnect(connection);
    }
    
    return NULL;
}

void openListener(ComAddress * address) {
    unlink(address->path);
    mkfifo(address->path, 0666);

    open(address->path, O_RDONLY | O_NONBLOCK);
}

void closeListener(ComAddress * address) {
    close(address->path);
    unlink(address->path);
}

int isConnected(ComAddress * address) {
    if( access(address->path, F_OK ) != -1 ) {
        return 1;
    }
    return 0;
}

int comWrite(Connection * connection, char * dataToWrite, int size) {

    return write(connection->outputFD, dataToWrite, size);

}

int comRead(Connection * connection, char * dataToRead, int size) {

    int ret;

    return read(connection->inputFD, dataToRead, size);
}

void disconnect(Connection * connection) {
    close(connection->inputFD);
    close(connection->outputFD);
    unlink(connection->input->path);
    unlink(connection->output->path);
    deleteComAddress(connection->input);
    deleteComAddress(connection->output);
    free(connection);
}