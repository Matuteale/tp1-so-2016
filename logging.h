#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#define MSG_SIZE 128

typedef struct msgbuf {
    long    mtype;
    char    mtext[MSG_SIZE];
}message_buf;

const key_t key = 2195; //Shared key between sender and receiver
const int msgflg = IPC_CREAT | 0666;
int msqid; //queue identificator

//type = 1->Info
//type = 2->Warning
//type = 3->Error
void logging(char * msg, int type);
