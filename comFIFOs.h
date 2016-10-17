#ifndef COMFIFOS_H
#define COMFIFOS_H

#define MAX_BUF 1024
#define MAX_PID_LENGTH 20

// MSGS CODES
#define SUCCESS "1"
#define FAIL "0"

typedef struct Connection {
    ComAddress * input; //READ
    ComAddress * output; //WRITE
    int inputFD;
    int outputFD;
};

typedef struct ComAddress {
    char * path;
};

#endif