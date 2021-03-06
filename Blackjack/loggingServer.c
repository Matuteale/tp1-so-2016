//Basado en las instrucciones de https://users.cs.cf.ac.uk/Dave.Marshall/C/node25.html

#include "logging.h"

void closeLoggingServer();

FILE * fp;

const key_t key = 2195; //Shared key between sender and receiver
const int msgflg = IPC_CREAT | 0666;

int main() {
    int msgQueueId;
    message_buf  rbuf;
    if ((msgQueueId = msgget(key, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    fp = fopen("/tmp/log.txt", "w+" );

    signal(SIGINT, closeLoggingServer);

    while(msgrcv(msgQueueId, &rbuf, MSG_SIZE, 0, 0) >= 0){

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
    }
    perror("msgrcv");
    fclose(fp);
}

void closeLoggingServer(){
    fputs("[Info]: Closing logging...\n", fp);
    fputs("[Info]: Logging closed.", fp);
    fclose(fp);
    printf("\n[Info]: Closing logging...\n");
    printf("[Info]: Logging closed.\n");
    exit(1);
}
