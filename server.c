#include "blackjacklib.h"
#include "server.h"

int main() {

    startServer();

    Address * srv = newAddress();
    strcpy(srv->path, SRV_PATH);

    while(1) {
        Address * listened = listen(srv);
        if (listened != NULL) {
            printf("Connection detected\n");
            accept(listened);
        }
    }

    return 0;
}

int startServer() {

    /* Creating SRV FIFO */
    unlink(SRV_PATH);
    mkfifo(SRV_PATH, 0666);

    open(SRV_PATH, O_RDONLY | O_NONBLOCK);
    
    return 1;
}