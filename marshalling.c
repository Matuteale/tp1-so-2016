#include "marshalling.h"
#include "comFIFOs.h" //TODO: REMOVER, ES SOLO PARA TEST
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
	printf("ESPERANDO STR DE %s FD %d\n", connection->input, connection->inputFD);
	while(comRead(connection, str, MAX_BUF) <= 0);

	str = realloc(str, strlen(str) * sizeof(char));

	return str;
}

int requestInt(Connection * connection) {

	char * buf = malloc(sizeof(int)+1);
	int ans = 0;
	printf("ESPERANDO INT DE %s FD %d\n", connection->input, connection->inputFD);
	while(comRead(connection, buf, sizeof(int)+1) <= 0);

	memcpy(&ans, buf, sizeof(int));

	free(buf);

	return ans;
}

Deal * requestDeal(Connection * connection) {

}

void sendStr(Connection * connection, char * str) {
	printf("ENVIANDO STR A %s FD %d\n", connection->output, connection->outputFD);
	comWrite(connection, str, strlen(str));
}

void sendInt(Connection * connection, int integer) {

	char * buf = malloc(sizeof(int)+1);

	clearBuffer(buf, sizeof(int)+1);

	memcpy(buf, &integer, sizeof(int));
printf("ENVIANDO INT A %s FD %d\n", connection->output, connection->outputFD);
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