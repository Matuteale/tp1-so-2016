#include "marshalling.h"
//DEBUG #include "comFIFOs.h" //TODO: REMOVER, ES SOLO PARA TEST

char requestChar(Connection * connection) {

	char buf[MAX_BUF];
	//DEBUG printf("ESPERANDO CHAR DE %s FD %d\n", connection->input->path, connection->inputFD);
	comRead(connection, buf, sizeof(char));
	//DEBUG printf("CHAR RECIBIDO %c\n", buf[0]);

	return buf[0];
}

char * requestStr(Connection * connection) {

	int i = 0;
	char c = 1;
	char * str = malloc(sizeof(char) * MAX_BUF);
	//DEBUG printf("ESPERANDO STR DE %s FD %d\n", connection->input->path, connection->inputFD);
	while(c != '\0') {
		c = requestChar(connection);
		str[i++] = c;
	}
	//DEBUG printf("STRING RECIBIDO %s\n", str);
	str = realloc(str, strlen(str) * sizeof(char));

	return str;
}

int requestInt(Connection * connection) {

	char * buf = malloc(sizeof(int)+1);
	int ans = 0;
	//DEBUG printf("ESPERANDO INT DE %s FD %d\n", connection->input->path, connection->inputFD);
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

Bet * requestBet(Connection * connection) {
	int i = requestInt(connection);
	int j = requestInt(connection);
	Bet * bet = newBet(i,j);
	return bet;
}

void sendStr(Connection * connection, char * str) {
	//usleep(CLOCK);
	//DEBUG printf("ENVIANDO STR %s A %s FD %d\n", str, connection->output->path, connection->outputFD);
	comWrite(connection, str, strlen(str)+1);
}

void sendInt(Connection * connection, int integer) {
	//usleep(CLOCK);
	char * buf = malloc(sizeof(int)+1);

	clearBuffer(buf, sizeof(int)+1);

	memcpy(buf, &integer, sizeof(int));
	//DEBUG printf("ENVIANDO INT %d A %s FD %d\n", integer, connection->output->path, connection->outputFD);
	comWrite(connection, buf, sizeof(int)+1);

	free(buf);
}

void sendChar(Connection * connection, char c) {
	//DEBUG printf("ENVIANDO CHAR %c A %s FD %d\n", c, connection->output->path, connection->outputFD);
	char buf[MAX_BUF] = {0};
	buf[0] = c;
    comWrite(connection, buf, sizeof(char));
}

void sendDeal(Connection * connection, Deal * deal) {
	sendChar(connection, deal->card);
	sendInt(connection, deal->playerNumber);
}

void sendBet(Connection * connection, Bet * bet) {
	sendInt(connection, bet->bet);
	sendInt(connection, bet->playerNumber);
}