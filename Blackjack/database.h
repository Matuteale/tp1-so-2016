#ifndef DATABASE_H
#define DATABASE_H

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "DBfnc.h"

int startDatabase(int seats);
void readTable();
void changeSeatMoney(int seat, int moneyToChange);
void dropTable();
int getMoney(int seat);

#endif