#ifndef _DBFNC_H_
#define _DBFNC_H_

#include <stdio.h>
#include <string.h>
#include "sqlite3.h"

#define NO_SESSION NULL
#define SQL_SELECT_ALL "select * from Players;"
#define SQL_GET_SEAT_BALANCE "select Balance from Players where Seat=%d;"
#define SQL_CREATE_TABLE "CREATE TABLE Players(SEAT INT PRIMARY KEY NOT NULL, BALANCE INT NOT NULL);"
#define SQL_ADD_PLAYER "INSERT INTO Players VALUES(%d, 0);"
#define DATABASE_NAME "database.db"
#define SQL_DROP_TABLE "DROP TABLE IF EXISTS 'Players' "
#define SQL_UPDATE_PLAYER ("UPDATE Players SET Balance='%d' WHERE Seat='%d'")

//Callback function para read Table
int printRow(void *unused, int argc, char **argv, char **azColName);
//Abre la base de datos
sqlite3* DBOpen();
//Crea una table con Seat Balance.
int DBCreateTable(int seats);
//Funcion de validacion para el open
int DBCheckTableExistance(sqlite3* db);
//Tira abajo la tabla actual
int DBDropTable();
//Le cambia el balance al asiento Seat por la cantidad ingresada como balance
int DBUpdatePlayer(int seat, int balance);
//Imprime el estado actual de la DB
int DBReadTable();
//Devuelve el value de la query por el Balance que se junte con seat
int DBGetSeatBalance(int seat);

#endif