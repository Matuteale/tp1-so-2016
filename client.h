#ifndef CLIENT_H
#define CLIENT_H
#endif
#include "com.h"
#include "comFIFOs.h"

void disconnectClient(Connection * connection);
void getCommand (int command, Connection * connection);
static void stand(Connection * connection);
static void hit(Connection * connection);