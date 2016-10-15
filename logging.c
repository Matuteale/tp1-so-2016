//Basado en las instrucciones de https://users.cs.cf.ac.uk/Dave.Marshall/C/node25.html

#include "logging.h"
#include <stdio.h>

int main() {
    int msgQueueId;
    message_buf  rbuf;
    if ((msgQueueId = msgget(key, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }
    FILE * fp;

    fp = fopen("/tmp/log.txt", "w+" );

    while(msgrcv(msgQueueId, &rbuf, MSG_SIZE, 0, 0) >= 0){

        fp = fopen("/tmp/log.txt", "a+" );

        if(rbuf.mtype == 1){ // Info
            printf("[Info]: %s\n", rbuf.mtext);
            fputs("[Info]: ", fp);
            fputs(rbuf.mtext, fp);
            fputs("\n", fp);
        }
        if(rbuf.mtype == 2){ // Warning
            printf("[Warning]: %s\n", rbuf.mtext);
            fputs("[Warning]: ", fp);
            fputs(rbuf.mtext, fp);
            fputs("\n", fp);
        }
        if(rbuf.mtype == 3){ // Error
            printf("[Error]: %s\n", rbuf.mtext);
            fputs("[Error]: ", fp);
            fputs(rbuf.mtext, fp);
            fputs("\n", fp);
        }
        fclose(fp);
    }
    perror("msgrcv");
    fclose(fp);
}

