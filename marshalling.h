#ifndef MARSHALLING_H
#define MARSHALLING_H

char * requestStr(Connection * connection); //BLOCKING
int requestInt(Connection * connection); //BLOCKING
Deal * requestDeal(Connection * connection); //BLOCKING

void sendStr(Connection * connection, char * str); //NON BLOCKING
void sendInt(Connection * connection, int integer); //NON BLOCKING
void sendDeal(Connection * connection, Deal * deal); //NON BLOCKING

#endif