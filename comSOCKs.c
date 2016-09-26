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

/** Protocol functions **/

static int generateTCPSocketClient(Connection connection) {
	struct sockaddr_in srvAddr;
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(connection->port);
	//TODO validar
	inet_pton(AF_INET, connection->ip, &srvAddr.sin_addr);
	if((connection->socketFD = createSocket(PF_INET, SOCK_STREAM, 0)) < 0 ) { //PF_INET to call socket, AF for the struct
		return -1
	}
	if(connectSocket(connection->socketFD, (struct sockaddr*) sizeof(srvAddr))) {
		return -1;
	}
	return 0;
}


static int generateTCPSocketServer(Connection connection) {
	struct sockaddr_in srv;

	srv.sin_family = AF_INET;
	srv.sin_addr.s_addr = htonl(INADDR_ANY); //Permite connecciones multiples
	srv.sin_port = htons(connection->port);
	if((connection->socketFD = createSocket(PF_INET, SOCK_STREAM, 0)) < 0) {
		return -1;
	}
	if(bindSocket(connection->socketFD, (struct sockaddr*) &srv, sizeof(srv))){
		return -1;
	}
	if(listenSocket(connection->socketFD, 10)) {
		return -1;
	}
	return 0;
}

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

static int listenSocket(int socketFD, int backlog) {
	if(listen(socketFD, backlog)) {
		fprintf(stderr, "Could not listen\n");
		return -1;
	}	
	return 0;
}

static int closeSocket() {
	return 0;
}