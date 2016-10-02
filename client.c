#include "blackjacklib.h"
#include "client.h"
#define TEST "Para bailar la bamba"
#define HIT 1 
#define STAND 2 
#define QUIT 3

int main() {

    int c;
    int finish = 0;
    char* buffer = malloc(MAX_BUF);
    memset(buffer, 0, MAX_BUF);
    char* buffer2 = malloc(MAX_BUF);
    memset(buffer2, 0, MAX_BUF);
    char * srvaddr = malloc(sizeof(SRV_PATH));
    Connection * connection;
    strcpy(srvaddr, SRV_PATH);

    connection = comConnect(srvaddr);


    printf("Input %d\n", connection->inputFD);
    printf("Output %d\n", connection->outputFD);
    comRead(connection, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    while( 1 ) {

        printf("ENTER COMMAND:\n1 = HIT\n2 = STAND\n3 = QUIT\n");
        scanf("%d", &c);
        getCommand(c, connection);
        while(getchar() != '\n');
        /*if (c == 'q') {
            disconnectClient(connection);
        }*/
    }

    return 0;

}

void disconnectClient(Connection * connection) {
    disconnect(connection);
    exit(1);
}


static void hit(Connection * connection) {
    char* buffer = malloc(MAX_BUF);
    memset(buffer, 0, MAX_BUF);
    snprintf(buffer, MAX_BUF, "%c\n", 'h');
    //newData(buffer);
    comWrite(connection, buffer, sizeof(buffer));
}

static void stand(Connection * connection) {
    char* buffer = malloc(MAX_BUF);
    memset(buffer, 0, MAX_BUF);
    snprintf(buffer, MAX_BUF, "%c\n", 's');
    //newData(buffer);
    comWrite(connection, buffer, sizeof(buffer));
}

void getCommand (int command, Connection * connection) {

    switch (command) {

        case HIT:
            hit(connection);
            break;

        case STAND:
            stand(connection);
            break;

        case QUIT:
            disconnectClient(connection);
            break;

        default:
            break;
    }
}
