#include "database.h"

int startDatabase(int seats) {
	dropTable();
	if (DBCreateTable(seats) == -1) {
    	fprintf(stderr, "[Warning]: DataBase Table couldn't be created.\n");
    	return -1;
    }
    return 0;
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