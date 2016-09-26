#ifndef COM_H
#define COM_H
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "comFIFOs.h"
#include "comSOCKs.h"

Connection * newConnection();
Address * newAddress();

Connection * connect(Address * addr);
Address * listen(Address * addr);
Connection * accept(Address * addressToAccept);
int comWrite(Connection * connection, char * dataToWrite, int size);
int comRead(Connection * connection, char * dataToRead, int size);
void disconnect(Connection * connection);