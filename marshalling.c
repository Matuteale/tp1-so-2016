#include "marshalling.h"

char requestChar(Connection * connection) {

	char c;

	printf("ESPERANDO CHAR\n");
	char * aux = requestStr(connection);

	c = aux[0];

	free(aux);

	return c;
}

char * requestStr(Connection * connection) {

	char * str = malloc(sizeof(char) * MAX_BUF);
	printf("ESPERANDO STR\n");
	while(comRead(connection, str, MAX_BUF) <= 0);

	str = realloc(str, strlen(str) * sizeof(char));

	return str;
}

int requestInt(Connection * connection) {

	char * buf = malloc(sizeof(int)+1);
	int ans = 0;
	printf("ESPERANDO INT\n");
	while(comRead(connection, buf, sizeof(int)+1) <= 0);

	memcpy(&ans, buf, sizeof(int));

	free(buf);

	return ans;
}

Deal * requestDeal(Connection * connection) {

}

void sendStr(Connection * connection, char * str) {
	printf("ENVIANDO STR\n");
	comWrite(connection, str, strlen(str));
}

void sendInt(Connection * connection, int integer) {

	char * buf = malloc(sizeof(int)+1);

	clearBuffer(buf, sizeof(int)+1);

	memcpy(buf, &integer, sizeof(int));
printf("ENVIANDO INT\n");
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

}