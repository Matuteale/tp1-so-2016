#include "com.h"
#include "blackjacklib.h"



Connection * newConnection();
Address * newAddress();

Connection * connect(Address * addr) {
	Connection connection = malloc(sizeof(*connection));
	if(connection == NULL) {
		return NULL;
	}
	return 
}
Address * listen(Address * addr);
Connection * accept(Address * addressToAccept);
int comWrite(Connection * connection, char * dataToWrite, int size);
int comRead(Connection * connection, char * dataToRead, int size);
void disconnect(Connection * connection);


/** Socket Function with respective validations**/

static int createSocket(int domain, int type, int protocolDesc) {
	int socket = socket(domain, type, protocolDesc);
	if(socket == -1) {
		return fprintf(stderr, "Socket creation failed\n", );
	}
	return socket;
}

static int connectSocket(int socketFD, const struct sockaddr * address, socklen_t addressLen) {
	if(connect(socketFD, address, addressLen)) {
		fprintf(stderr, "Could not connect\n");
		return -1;
	}	
	return 0;
}

static int bindSocket(int socketFD, const struct sockaddr * address, socklen_t addressLen) {
	if(bind(socketFD, address, addressLen)) {
		fprintf(stderr, "Could not bind\n");
		return -1;
	}	
	return 0;
}

static int listenSocket(int socketFD, const struct sockaddr * address, socklen_t addressLen) {
	if(listen(socketFD, address, addressLen)) {
		fprintf(stderr, "Could not listen\n");
		return -1;
	}	
	return 0;
}

static int closeSocket() {

}