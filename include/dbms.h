#pragma once

#include "sommelier-db.h"
#include "util.h"

#define MAX_SIZE_TRANSACTION_NAME 32
#define COMMIT_TRANSACTION_PER_INSERT 10
#define MAX_SIZE_LOGGING_SQL_TEXT 160

typedef struct sommelier_db {
    int insert_count;
    char *filepath;
    char *transaction_name;
    sqlite3 *db;
} SommelierDBMS;

SommelierDBMS *initialize_sommelier_dbms();
int OpenSommelierDB(SommelierDBMS *, const char *);
void CloseSommelierDB(SommelierDBMS *);
void StartTransaction(SommelierDBMS *);
void CommitTransaction(SommelierDBMS *);
char *current_transaction_name(SommelierDBMS *);
sqlite3 *sommelier_connection(SommelierDBMS *);
sqlite3 *sommelier_connection_with_insert(SommelierDBMS *);
int orm_execute_sql(sqlite3 *, const char *sql, sqlite3_callback, void *);
