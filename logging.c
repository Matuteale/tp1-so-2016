//Basado en las instrucciones de https://users.cs.cf.ac.uk/Dave.Marshall/C/node25.html

#include "logging.h"

int main() {
    int msgQueueId;
    message_buf  rbuf;
    if ((msgQueueId = msgget(key, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    while(1){

        if (msgrcv(msgQueueId, &rbuf, MSG_SIZE, 0, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }

        if(rbuf.mtype == 1){ // Info
            printf("[Info]: %s\n", rbuf.mtext);
        }
        if(rbuf.mtype == 2){ // Warning
            printf("[Warning]: %s\n", rbuf.mtext);
        }
        if(rbuf.mtype == 3){ // Error
            printf("[Error]: %s\n", rbuf.mtext);
        }


    }
}

