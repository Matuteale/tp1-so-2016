#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "DBfnc.h"

void startDatabase() {
	DBCreateTable();
}

void readTable() {
	DBReadTable();
}

void changeSeatMoney(int seat, int moneyToChange) {
	DBUpdatePlayer (seat, moneyToChange);
}

void dropTable() {
	DBDropTable();
}

void getMoney(int seat) {
	DBGetSeatMoney(seat);
}