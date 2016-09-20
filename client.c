#include "blackjacklib.h"
#include "client.h"


int main() {

    char c;

    // struct ClientInfo* clientInfo = (struct ClientInfo*) malloc(sizeof(struct ClientInfo));

    Address * srvaddr = newAddress();

    strcpy(srvaddr->path, SRV_PATH);

    connect(srvaddr);

    while( (c = getchar()) != '\n') {
        if (c == 'q') {
            exit(1);
        }
    }

    return 0;

}