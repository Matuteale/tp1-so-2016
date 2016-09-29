#include "blackjacklib.h"
#include "client.h"
#define TEST "Para bailar la bamba"

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
    while( !finish ) {
        comRead(connection, buffer, sizeof(buffer));
        printf("%s\n", buffer);
        scanf("%d", &c);
        switch(c){
            case 0: 
                disconnectClient(connection);
                finish++;
                break;
            case 1:
                scanf("%s", buffer);
                snprintf(buffer2, sizeof(buffer), "%s\0", buffer);
                printf("%s\n", buffer);
                comWrite(connection, buffer, sizeof(buffer));
                break;
            default:
                printf("NYET\n");
                break;
        }
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