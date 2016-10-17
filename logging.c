//Basado en las instrucciones de https://users.cs.cf.ac.uk/Dave.Marshall/C/node25.html

#include "logging.h"
#include <stdio.h>

const key_t key = 2195; //Shared key between sender and receiver
const int msgflg = IPC_CREAT | 0666;
int msqid; //queue identificator

void logging(char * msg, int type){
    message_buf sbuf;
    sbuf.mtype = type;
    (void) strcpy(sbuf.mtext, msg);
    msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, IPC_NOWAIT);
}
void startLogging(){
    msgctl(msgget(key, msgflg), IPC_RMID, NULL); //Deletes de msg queue
    msqid = msgget(key, msgflg);
}
