#include "com.h"
#include "comSOCKs.h"
#include "blackjacklib.h"

Connection * newConnection() {
	Connection * connection;
	connection = malloc(sizeof (Connection));
    if (connection == NULL) {
        return NULL;
    }
	connection->ip = calloc(1, (colonPosition + 1) * sizeof (char));	
	if (connection->ip == NULL) {
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
	Connection * connection;

    if (!checkPort(addr)) {
        return NULL;
    }
    connection = newConnection();
    if (connection == NULL) {
        return NULL;
    }
    connection->port = atoi(addr);
    if(generateTCPSocketServer == -1) {
    	return NULL;
    }

    return 
}
Connection * comAccept(char * addressToAccept) {
	
}
int comWrite(Connection * connection, char * dataToWrite, int size) {
	return write(connection->socketFD, dataToWrite, size);
}
int comRead(Connection * connection, char * dataToRead, int size){
	return read(connection->socketFD, dataToWrite, size);	
}
void disconnect(Connection * connection) {
	shutdown(connection->socketFD, SHUT_WR);
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
    strncpy(fixedAddress.sun_path, SRV_PATH_SOCK, sizeof(fixedAddress.sun_path) - 1);
    if (bind(socketFD, (struct sockaddr *) &fixedAddress, sizeof(fixedAddress)) < 0){
        return NULL;    
    }
    return socketFD;
}


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

/** Validation Functions **/

static int checkIP(const char *ip) {
 
    regex_t regex;
    char *expr = getIPRegEx();
    int result = 0;
 
    if (expr == NULL) {
        return -1;
    }
 
    regcomp(&regex, expr, REG_EXTENDED);
    result = !regexec(&regex, ip, 0, NULL, 0); /*Returns 0 if success, or a non-zero code if not match */
    free(expr); /* Must free because getIpRegex function allocates memory */
    return result;
 
}

static int searchForColon(const char *address) {
 
    int i = 0;
    while (address[i] != 0 && address[i] != ':') {
        i++;
    }
    return (address[i] == 0) ? -1 : i;
}
 

static char* getIPRegEx() {
 
    char *expr1 = "^((0|[3-9][0-9]?|1[0-9]?[0-9]?|25[0-5]|2[0-4]?[0-9]?|2[0-9])\\.)";
    char *expr2 = "{1,3}(0|[3-9][0-9]?|1[0-9]?[0-9]?|25[0-5]|2[0-4]?[0-9]?|2[0-9])$";
    size_t len1 = strlen(expr1);
    size_t len2 = strlen(expr2);
    size_t totalLength = len1 + len2;
 
    char *expr = calloc(1, (totalLength + 1) * sizeof (char));
    if (expr == NULL) {
        return NULL;
    }
    memcpy(expr, expr1, len1 * sizeof (char));
    memcpy(expr + len1, expr2, len2 * sizeof (char));
 
    return expr;
}

static char* getPortRegEx() {
 
    return "^(0|[1-9][0-9]{1,4})$";
}


static int checkPort(const char *portStr) {
 
    regex_t regex;
    char *expr = getPortRegEx();
    int result = 0;
    unsigned short port;
    regcomp(&regex, expr, REG_EXTENDED);
    result = regexec(&regex, portStr, 0, NULL, 0); /* Returns 0 if success, or a non-zero code if not match */
 
    if (result) {
        return 0; /* Port doesn't match with specified format */
    }
    port = atoi(portStr);
    if (port > 65535) { /* Just check upper limit because a negative number wouldn't have passsed the regex test */
        return 0; /* Port out of range*/
    }
    return 1;
}
 

static int isValidAddress(const char *address) {
 
    int colonPosition = searchForColon(address);
    char *ipPart;
    char *portPart;
    int portPartLength = strlen(address) - colonPosition - 1;
    int flag1 = 0, flag2 = 0;
 
    if (colonPosition < 0) {
        return 0; /* Invalid IP Address. Colon is missing */
    }
 
    ipPart = calloc(1, (colonPosition + 1) * sizeof (char));
    portPart = calloc(1, (portPartLength + 1) * sizeof (char));
    memcpy(ipPart, address, colonPosition * sizeof (char));
    memcpy(portPart, address + colonPosition + 1, portPartLength * sizeof (char));
 
    flag1 = checkIP(ipPart);
    flag2 = checkPort(portPart);
 
    if (flag1 == -1) {
        return -1;
    }
 
    return flag1 && flag2;
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