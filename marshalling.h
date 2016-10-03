#ifndef MARSHALLING_H
#define MARSHALLING_H

#include "blackjacklib.h"
#include "com.h"

char requestChar(Connection * connection); //BLOCKING
char * requestStr(Connection * connection); //BLOCKING
int requestInt(Connection * connection); //BLOCKING
Deal * requestDeal(Connection * connection); //BLOCKING

void sendChar(Connection * connection, char action); //NON BLOCKING
void sendStr(Connection * connection, char * str); //NON BLOCKING
void sendInt(Connection * connection, int integer); //NON BLOCKING
void sendDeal(Connection * connection, Deal * deal); //NON BLOCKING

#endif