#ifndef _DBFNC_H_
#define _DBFNC_H_

#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

#define NO_SESSION NULL
#define SQL_SELECT_ALL "select * from Players;"
#define SQL_CREATE_TABLE "CREATE TABLE Players(SEAT INT PRIMARY KEY NOT NULL, MONEY INT NOT NULL);"
#define SQL_ADD_PLAYER "INSERT INTO Players VALUES(%d, 100);"
#define DATABASE_NAME "database.db"
#define SQL_DROP_TABLE "DROP TABLE IF EXISTS 'Players' "
#define SQL_UPDATE_PLAYER ("UPDATE Players SET Money='%d' WHERE Seat='%d'")

int printRow(void *unused, int argc, char **argv, char **azColName);
sqlite3* DBOpen();
int DBCreateTable();
int DBCheckTableExistance(sqlite3* db);
int DBDropTable();
int DBUpdatePlayer (int seat, int money);
int DBReadTable();

#endif