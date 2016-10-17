#include "marshalling.h"

char requestChar(Connection * connection) {

	char buf[MAX_BUF];
	comRead(connection, buf, sizeof(char));

	return buf[0];
}

char * requestStr(Connection * connection) {

	int i = 0;
	char c = 1;
	char * str = malloc(sizeof(char) * MAX_BUF);
	while(c != '\0') {
		c = requestChar(connection);
		str[i++] = c;
	}
	str = realloc(str, strlen(str) * sizeof(char));

	return str;
}

int requestInt(Connection * connection) {

	char * buf = malloc(sizeof(int)+1);
	int ans = 0;
	comRead(connection, buf, sizeof(int)+1);
	memcpy(&ans, buf, sizeof(int));
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

	comWrite(connection, str, strlen(str)+1);
}

void sendInt(Connection * connection, int integer) {

	char * buf = malloc(sizeof(int)+1);

	clearBuffer(buf, sizeof(int)+1);
	memcpy(buf, &integer, sizeof(int));
	comWrite(connection, buf, sizeof(int)+1);
	free(buf);
}

void sendChar(Connection * connection, char c) {

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