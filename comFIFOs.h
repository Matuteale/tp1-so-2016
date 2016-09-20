#ifndef COMFIFOS_H
#define COMFIFOS_H
#endif

typedef struct Address {
    char * path;
} Address;

typedef struct Connection {
    Address * input; //READ
    Address * output; //WRITE
    int inputFD;
    int outputFD;
} Connection;