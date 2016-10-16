#include "logging.h"

void createLoggingSystem(){
	msgctl(msgget(key, msgflg), IPC_RMID, NULL); //Deletes de msg queue
    msqid = msgget(key, msgflg);
}

void logging(char * msg, int type){
    message_buf sbuf;
    sbuf.mtype = type;
    (void) strcpy(sbuf.mtext, msg);
    msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, IPC_NOWAIT);
}