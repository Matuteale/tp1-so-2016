#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "DBfnc.h"

void startDatabase(int seats) {
	dropTable();
	if (DBCreateTable(seats) == -1) {
    	fprintf(stderr, "ERROR: DataBase Table couldn't be created.\n");
    }
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

int getMoney(int seat) {
	return DBGetSeatBalance(seat);
}