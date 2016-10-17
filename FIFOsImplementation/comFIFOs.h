#ifndef COMFIFOS_H
#define COMFIFOS_H

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