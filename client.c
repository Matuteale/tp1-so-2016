#include "blackjacklib.h"
#include "client.h"


int main() {

    char c;

    Address * srvaddr = newAddress();
    Connection * connection;

    strcpy(srvaddr->path, SRV_PATH);

    connection = connect(srvaddr);

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