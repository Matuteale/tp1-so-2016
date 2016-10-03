#include "marshalling.h"
//DEBUG #include "comFIFOs.h" //TODO: REMOVER, ES SOLO PARA TEST
char requestChar(Connection * connection) {

	char c;

	char * aux = requestStr(connection);

	c = aux[0];

	free(aux);

	return c;
}

char * requestStr(Connection * connection) {

	char * str = malloc(sizeof(char) * MAX_BUF);
	//DEBUG printf("ESPERANDO STR DE %s FD %d\n", connection->input, connection->inputFD);
	comRead(connection, str, MAX_BUF);
	//DEBUG printf("STRING RECIBIDO %s\n", str);
	str = realloc(str, strlen(str) * sizeof(char));

	return str;
}

int requestInt(Connection * connection) {

	char * buf = malloc(sizeof(int)+1);
	int ans = 0;
	//DEBUG printf("ESPERANDO INT DE %s FD %d\n", connection->input, connection->inputFD);
	comRead(connection, buf, sizeof(int)+1);
	memcpy(&ans, buf, sizeof(int));
	//DEBUG printf("INT RECIBIDO %d\n", ans);
	free(buf);

	return ans;
}

Deal * requestDeal(Connection * connection) {
	char c = requestChar(connection);
	int i = requestInt(connection);
	Deal * deal = newDeal(c, i);
	return deal;
}

void sendStr(Connection * connection, char * str) {
	usleep(CLOCK);
	//DEBUG printf("ENVIANDO STR %s A %s FD %d\n", str, connection->output, connection->outputFD);
	comWrite(connection, str, strlen(str));
}

void sendInt(Connection * connection, int integer) {
	usleep(CLOCK);
	char * buf = malloc(sizeof(int)+1);

	clearBuffer(buf, sizeof(int)+1);

	memcpy(buf, &integer, sizeof(int));
	//DEBUG printf("ENVIANDO INT %d A %s FD %d\n", integer, connection->output, connection->outputFD);
	comWrite(connection, buf, sizeof(int)+1);

	free(buf);
}

void sendChar(Connection * connection, char action) {
	
    char * str = malloc(2*sizeof(char));

    str[0] = action;
    str[1] = '\0';

    sendStr(connection, str);

    free(str);
}

void sendDeal(Connection * connection, Deal * deal) {
	sendChar(connection, deal->card);
	sendInt(connection, deal->playerNumber);
}