#ifndef DATABASE_H
#define DATABASE_H

#include "blackjacklib.h"
#include "DBfnc.h"

void startDatabase();
void readTable();
void changeSeatMoney(int seat, int moneyToChange);
void dropTable();
void getMoney(int seat);

#endif