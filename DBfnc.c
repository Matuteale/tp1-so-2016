#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "DBfnc.h"

int printRow(void *unused, int argc, char **argv, char **azColName) {
  unused = 0;
  int i;
  FILE * fp;
  fp = fopen ("dababaseExport.txt", "aw+");
  for (i = 0; i < argc; i=i+2) {
    printf("%s\t%s", argv[i] ? argv[i] : "NULL", argv[i+1] ? argv[i+1] : "NULL");
    fprintf(fp, "%s\t%s\n", argv[i] ? argv[i] : "NULL", argv[i+1] ? argv[i+1] : "NULL");
  }
  fclose(fp);
  printf("\n");
  return 0;
}

sqlite3* DBOpen() {
  sqlite3* db;
  int rc = sqlite3_open(DATABASE_NAME, &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return NO_SESSION;
  }
  return db;
}

int DBCreateTable() {
  sqlite3* db;
  int i;
  int rc = sqlite3_open(DATABASE_NAME, &db);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return -1;
  }

  char *err_msg = 0;
  int req = sqlite3_exec(db, SQL_CREATE_TABLE, 0, 0, &err_msg);
  if (req != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    fprintf(stderr, "SQL error: %d\n", req);
    sqlite3_free(err_msg);
    return -1;
  }

  for(i = 0; i < 8; i++) {
    char sql[200];
    sprintf (sql, SQL_ADD_PLAYER, i);
    req = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (req != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", err_msg);
      fprintf(stderr, "SQL error: %d\n", req);
      sqlite3_free(err_msg);
      return -1;
    }
  }
  return 1;
}

int DBCheckTableExistance(sqlite3* db) {
  char *err_msg = 0;
  int rc = sqlite3_exec(db, SQL_SELECT_ALL, 0, 0, &err_msg);

  if (rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    fprintf(stderr, "SQL error: %d\n", rc);

    if ( rc == -1 ) {
      sqlite3_close(db);
    }
  }
  return rc;
}



int DBDropTable () {

  int rc;
  sqlite3* db = DBOpen();
  rc = DBCheckTableExistance(db);

  if ( db == NULL || rc == -1 ) {
    sqlite3_close(db);
    return -1;
  }
  char *err_msg = 0;

  rc = sqlite3_exec(db, SQL_DROP_TABLE, 0, 0, &err_msg);

  if (rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    fprintf(stderr, "SQL error: %d\n", rc);
    sqlite3_free(err_msg);
    sqlite3_close(db);
  }

  sqlite3_close(db);
  return 1;
}


int DBUpdatePlayer (int seat, int money) {
  int rc;
  sqlite3* db = DBOpen();
  rc = DBCheckTableExistance(db);

  if ( db == NULL || rc == -1 ) {
    sqlite3_close(db);
    return -1;
  }

  char *err_msg = 0;

  char sql[200];
  sprintf (sql, SQL_UPDATE_PLAYER, money, seat);

  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

  if (rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    fprintf(stderr, "SQL error: %d\n", rc);
    sqlite3_free(err_msg);
    sqlite3_close(db);
  }

  sqlite3_close(db);
  return 1;
}


int DBReadTable () {
  remove("dababaseExport.txt");
  int rc;
  sqlite3* db = DBOpen();
  rc = DBCheckTableExistance(db);

  if ( db == NULL || rc == -1 ) {
    sqlite3_close(db);
    return -1;
  }

  char *err_msg = 0;

  printf("Seat\tMoney\n");
  rc = sqlite3_exec(db, SQL_SELECT_ALL  , printRow, 0, &err_msg);

  if (rc != SQLITE_OK ) {
    fprintf(stderr, "Failed to select data\n");
    fprintf(stderr, "SQL error: %s\n", err_msg);

    sqlite3_free(err_msg);
    sqlite3_close(db);

    return 1;
  }
}