#ifndef DATABASE_H
#define DATABASE_H

#include "blackjacklib.h"
#include "DBfnc.h"

int startDatabase(int seats);
void readTable();
void changeSeatMoney(int seat, int moneyToChange);
void dropTable();
int getMoney(int seat);

#endif