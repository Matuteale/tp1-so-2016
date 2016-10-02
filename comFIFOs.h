#ifndef COMFIFOS_H
#define COMFIFOS_H

typedef struct Connection {
    char * input; //READ
    char * output; //WRITE
    int inputFD;
    int outputFD;
};

#endif