#include "com.h"
#include "comSOCKs.h"
#include "blackjacklib.h"

Connection * newConnection() {
    Connection * connection = malloc(sizeof(Connection));
    return connection;
}

ComAddress * newComAddress(char * path) {
    ComAddress * address = malloc(sizeof(ComAddress));
    address->path = path;
    address->port = "8000";
    return address;
}


Connection * comConnect(ComAddress * addr) {
	Connection * connection = newConnection();
	int fd;
    struct timeval timeout;      
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    struct sockaddr_un clientAddr;
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    printf("%d\n", fd);
    if(fd == -1) {
        return NULL;
    }
    memset(&clientAddr, 0, sizeof(struct sockaddr_un));
    clientAddr.sun_family = AF_UNIX;
    strncpy(clientAddr.sun_path, addr->path, sizeof(clientAddr.sun_path) - 1);
    if(connect(fd, (struct sockaddr *) &clientAddr, sizeof(clientAddr)) == -1) {
        printf("NO CONECTA\n");
        return NULL;
    }
    connection->socketFD = fd;
    char buffer[MAX_BUF];
    int test = comRead(connection, buffer, MAX_BUF);
    if (strcmp(buffer, SUCCESS) == 0) {
        comWrite(connection, buffer, MAX_BUF);
        printf("CONNECTED\n");
        return connection;
    } 
    return connection;
}

ComAddress * comListen(ComAddress * addr) {
	char buffer[MAX_BUF];
    int sock;
    struct sockaddr_un clientName;
    socklen_t cliNameLen;
    printf("ESCUCHO a %d\n", addr->socketFD);
    if ((sock = accept(addr->socketFD, (struct sockaddr*)&clientName, &cliNameLen)) <= 0) {
        printf("NO HAY NADA\n");
        return NULL;
    }
    ComAddress * aux = newComAddress(addr->path);
    aux->socketFD = sock;
    return aux;
}

Connection * comAccept(ComAddress * addressToAccept) {
    Connection * connection = newConnection();
    connection->socketFD = addressToAccept->socketFD;
    char buffer[MAX_BUF];
    strcpy(buffer, SUCCESS);
    int i = comWrite(connection, buffer, MAX_BUF);
    printf("%d\n", i);
    int test = comRead(connection, buffer, MAX_BUF);
    if (strcmp(buffer, SUCCESS) == 0) {
        return connection;
    } 
    struct timeval timeout;      
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    return connection;
}

int comWrite(Connection * connection, char * dataToWrite, int size) {

    return write(connection->socketFD, dataToWrite, size);

}

int comRead(Connection * connection, char * dataToRead, int size) {

    return read(connection->socketFD, dataToRead, size);
}
void disconnect(Connection * connection) {
	close(connection->socketFD);
}

int openListener(ComAddress * address) {
	unlink(address->path);
	int socketFD;
    socketFD = socket(AF_UNIX, SOCK_STREAM, 0);
    if(socketFD == -1) {
        return -1;
    }
    struct sockaddr_un fixedAddress;
    memset(&fixedAddress, 0, sizeof(struct sockaddr_un));
    fixedAddress.sun_family = AF_UNIX;
    strncpy(fixedAddress.sun_path, address->path, sizeof(fixedAddress.sun_path) - 1);
    if (bind(socketFD, (struct sockaddr *) &fixedAddress, sizeof(fixedAddress)) < 0) {
        return NULL;
    }
    listen(socketFD, 8);
    address->socketFD = socketFD;
    int flags = fcntl(address->socketFD, F_GETFL, 0);
    fcntl(address->socketFD, F_SETFL, flags | O_NONBLOCK);
}

int isConnected(Connection * connection) {
    if(write(connection->socketFD, "c", 1) == -1) {
        return 0;
    } 
    return 1;
}

void closeListener(ComAddress * address) {
	shutdown(address->socketFD, 0);
}