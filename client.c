#include "blackjacklib.h"
#include "client.h"


int main() {

    char c;

    char * srvaddr = malloc(sizeof(SRV_PATH));
    Connection * connection;

    strcpy(srvaddr, SRV_PATH);

    connection = comConnect(srvaddr);

    while( (c = getchar()) != '\n') {
        if (c == 'q') {
            disconnectClient(connection);
        }
    }

    return 0;

}

void disconnectClient(Connection * connection) {
    disconnect(connection);
    exit(1);
}