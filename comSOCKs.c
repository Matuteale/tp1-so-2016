#include "com.h"
#include "comSOCKs.h"
#include "blackjacklib.h"
#define MAX_PLAYERS 8

Connection * newConnection() {
	Connection * connection;
	connection = malloc(sizeof (Connection));
    if (connection == NULL) {
        return NULL;
    }
    return connection;
}

Connection * comConnect(char * addr) {
	int fd;
	Connection * connection = newConnection();
	struct sockaddr_un cliAddress;

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(fd == -1){
		return NULL;
	}
	memset(&cliAddress, 0, sizeof(struct sockaddr_un));
	cliAddress.sun_family = AF_UNIX;
	strncpy(cliAddress.sun_path, addr, sizeof(cliAddress.sun_path) - 1);
	if (connect(fd, (struct sockaddr *) &cliAddress, sizeof(cliAddress)) == -1){
		return NULL;	
	}
	connection->socketFD = fd;
	return connection;
}

char * comListen(char * addr) {
	printf("el fd del server es %d\n", (int)addr);
	int socketFD = (int) addr;
    while(listen(socketFD, MAX_PLAYERS) == -1);
    return socketFD;
}

Connection * comAccept(char * addressToAccept) {
	Connection * connection = newConnection();
	struct sockaddr_un cliName;
	socklen_t cliNameLen;
	int fd = (int)addressToAccept;
	connection->socketFD = accept(fd, (struct sockaddr*)&cliName, &cliNameLen);
	return connection;
}

int comWrite(Connection * connection, char * dataToWrite, int size) {
	 char * buf = malloc(MAX_BUF);

    clearBuffer(buf, MAX_BUF);

    while(strcmp(buf, SUCCESS) != 0) {
        clearBuffer(buf, MAX_BUF);
        read(connection->socketFD, buf, sizeof(SUCCESS) +1);
    }

    free(buf);

    return write(connection->socketFD, dataToWrite, size);

}
int comRead(Connection * connection, char * dataToRead, int size){
	char * buf = malloc(MAX_BUF);

    strcpy(buf, SUCCESS);

    write(connection->socketFD, buf, sizeof(SUCCESS)+1);

    free(buf);

    int ret;

    while(ret = read(connection->socketFD, dataToRead, size) <= 0);

    return ret;
	
}
void disconnect(Connection * connection) {
	close(connection->socketFD);
}

int createOriginalSocket() {
    int socketFD;
    socketFD = socket(AF_UNIX, SOCK_STREAM, 0);
    if(socketFD == -1) {
        return -1;
    }
    struct sockaddr_un fixedAddress;
    memset(&fixedAddress, 0, sizeof(struct sockaddr_un));
    fixedAddress.sun_family = AF_UNIX;
    strncpy(fixedAddress.sun_path, SRV_PATH, sizeof(fixedAddress.sun_path) - 1);
    if (bind(socketFD, (struct sockaddr *) &fixedAddress, sizeof(fixedAddress)) < 0){
        return NULL;    
    }
    return socketFD;
}

