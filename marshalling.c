#include "marshalling.h"

char * requestStr(Connection * connection) {

	char * str = malloc(sizeof(char) * MAX_BUF);

	while(comRead(connection, str, MAX_BUF) <= 0);

	str = realloc(str, strlen(str) * sizeof(char));

	return str;
}

int requestInt(Connection * connection) {

	char * buf = malloc(sizeof(int)+1);
	int ans = 0;

	while(comRead(connection, buf, sizeof(int)+1) <= 0);

	memcpy(&ans, buf, sizeof(int));

	free(buf);

	return ans;
}

Deal * requestDeal(Connection * connection) {

}

void sendStr(Connection * connection, char * str) {

	comWrite(connection, str, strlen(str));
}

void sendInt(Connection * connection, int integer) {

	char * buf = malloc(sizeof(int)+1);

	clearBuffer(buf, sizeof(int)+1);

	memcpy(buf, &integer, sizeof(int));

	comWrite(connection, buf, sizeof(int)+1);

	free(buf);
}

void sendDeal(Connection * connection, Deal * deal) {

}