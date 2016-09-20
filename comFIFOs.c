#include "com.h"
#include "blackjacklib.h"

Connection * newConnection() {
    Connection * connection = malloc(sizeof(Connection));
    connection->input = newAddress();
    connection->output = newAddress();
    return connection;
}

Address * newAddress() {
    Address * address = malloc(sizeof(Address));
    address->path = malloc(MAX_PATH);
    return address;
}

Address * listen(Address * addr) {

    int inputAuxFD;

    char buffer[MAX_BUF];

    inputAuxFD = open(addr->path, O_RDONLY | O_NONBLOCK);

    if (read(inputAuxFD, buffer, MAX_BUF) <= 0) {
        close(inputAuxFD);
        return NULL;
    }

    Address * aux = newAddress();
    strcpy(aux->path, buffer);

    close(inputAuxFD);

    return aux;

}

Connection * accept(Address * addressToAccept) {

    char buffer[MAX_BUF];

    Connection * connection = newConnection();

    /* Copying CLIENTIN FIFO PATH to Connection Output */
    strcpy(connection->output->path, addressToAccept->path);

    /* Opening OUTPUT (CLIENTIN) FD */
    connection->outputFD = open(connection->output->path, O_WRONLY);

    /* Creating CLIENTOUT FIFO PATH */
    strncpy(connection->input->path, connection->output->path, indexOf(connection->output->path,'C'));
    strcat(connection->input->path, "CLIENTOUT");

    /* Creating CLIENTOUT FIFO */
    unlink(connection->input->path);
    mkfifo(connection->input->path, 0666);

    /* Opening CLIENTOUT FD */
    connection->inputFD = open(connection->input->path, O_RDONLY | O_NONBLOCK);

    /* Writing CLIENTOUT FIFO PATH to Client */
    write(connection->outputFD, connection->input->path, MAX_BUF);

    //ACA IRIAN LOS CLOSE
 
    while(comRead(connection, buffer, MAX_BUF) <= 0);
    // O ACA

    if (strcmp(buffer, SUCCESS) == 0) {
        strcpy(buffer, SUCCESS); //If i just send SUCCESS in the line below it doesnt work.
        comWrite(connection, buffer, MAX_BUF);
        printf("CONNECTED\n");
        return connection;
    } else {
        unlink(connection->input->path);
    }

    return NULL;

}

Connection * connect(Address * addr) {

    int outputAuxFD;
    char * auxPID;
    char buffer[MAX_BUF];

    Connection * connection = newConnection();

    /* Allocating */
    auxPID = malloc(MAX_PID_LENGTH*sizeof(char));

    /* Obtaining PID string format */
    sprintf(auxPID, "%d", getpid());

    /* Creating INPUT path */
    strcpy(connection->input->path, addr->path);
    strcat(connection->input->path, auxPID);
    strcat(connection->input->path, "CLIENTIN");

    /* Creating INPUT Fifo */
    unlink(connection->input->path);
    mkfifo(connection->input->path, 0666);

    /* Opening AUX OUTPUT FD */
    outputAuxFD = open(addr->path, O_WRONLY);

    /* Opening INPUT FD */
    connection->inputFD = open(connection->input->path, O_RDONLY | O_NONBLOCK);

    /* Writing INPUT FIFO PATH to AUX OUTPUT FD, requesting a connection */
    write(outputAuxFD, connection->input->path, MAX_BUF);

    /* Waiting for SRV response */
    while(read(connection->inputFD, buffer, MAX_BUF) <= 0);

    /* Closing FD */
    close(outputAuxFD);

    /* Opening OUTPUT FD */
    strcpy(connection->output->path, buffer);
    connection->outputFD = open(connection->output->path, O_WRONLY);

    /* Requesting SRV confirmation on successful connection */
    strcpy(buffer, SUCCESS); //If i just send SUCCESS in the line below it doesnt work.
    comWrite(connection, buffer, MAX_BUF);

    /* Waiting for SRV response */ //TODO: FIX WARNINGS, SHOULD COMPARE BYTES.
    while(comRead(connection, buffer, MAX_BUF) <= 0);
    if (strcmp(buffer, SUCCESS) == 0) {
        printf("CONNECTED\n");
        return connection;
    } else {
        unlink(connection->input->path);
    }
    
    return NULL;
}

int comWrite(Connection * connection, char * dataToWrite, int size) {

    return write(connection->outputFD, dataToWrite, MAX_BUF);

}

int comRead(Connection * connection, char * dataToRead, int size) {

    return read(connection->inputFD, dataToRead, MAX_BUF);

}

//TODO: Decidir si hacemos que retorne int segun si desconecta correctamente o lo dejamos asi
void disconnect(Connection * connection) {
    close(connection->inputFD);
    close(connection->outputFD);
    unlink(connection->input->path);
    free(connection->input->path);
    free(connection->output->path);
    free(connection->input);
    free(connection->output);
    free(connection);
}