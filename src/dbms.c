#include "dbms.h"

SommelierDBMS *initialize_sommelier_dbms() {
    SommelierDBMS *dbms = INITIALIZE(SommelierDBMS);

    if (dbms != NULL) {
        dbms->insert_count = 0;
        dbms->filepath = NULL;
        dbms->transaction_name = current_transaction_name(dbms);
        dbms->db = NULL;

        // if (dbms->db == NULL) {
        //     logging_error("SommelierDBMS::db is NULL.");
        // }
    } else {
        logging_error("SommelierDBMS is NULL.");
    }

    return dbms;
}

char *current_transaction_name(SommelierDBMS *dbms) {
    char *tr_name = alloc_string(MAX_SIZE_TRANSACTION_NAME);
    sprintf(tr_name, "Transaction%d",
            dbms->insert_count / COMMIT_TRANSACTION_PER_INSERT);
    return tr_name;
}

int OpenSommelierDB(SommelierDBMS *dbms, const char *dbfile) {
    dbms->filepath = alloc_string(strlen(dbfile) + 1);
    dbms->filepath = copy_string(dbms->filepath, dbfile);

    int err = sqlite3_open(dbfile, &(dbms->db));
    if (err) {
        char msg[200] = "";

        sprintf(msg, "Can't open database (%s): %s", dbfile,
                sqlite3_errmsg(dbms->db));

        logging_error(msg);

        sqlite3_close(dbms->db);
        exit(1);
    }

    return err;
}

void StartTransaction(SommelierDBMS *dbms) {
    char sql_tr[64] = "";
    sprintf(sql_tr, "BEGIN TRANSACTION %s;", current_transaction_name(dbms));
    orm_execute_sql(dbms->db, sql_tr, 0, 0);
}

void CommitTransaction(SommelierDBMS *dbms) {
    char sql_tr[64] = "";
    sprintf(sql_tr, "COMMIT TRANSACTION %s;", current_transaction_name(dbms));
    orm_execute_sql(dbms->db, sql_tr, 0, 0);
}

void CloseSommelierDB(SommelierDBMS *dbms) {
    CommitTransaction(dbms);

    sqlite3_close(dbms->db);
    free(dbms->filepath);
    free(dbms->transaction_name);
    free(dbms);
}

sqlite3 *sommelier_connection(SommelierDBMS *dbms) {
    if (VERBOSE) {
        char msg[200] = "";
        sprintf(msg, "DB connection: %s, DB Transaction: %s", dbms->filepath,
                dbms->transaction_name);
        logging_debug(msg);
    }

    return dbms->db;
}

sqlite3 *sommelier_connection_with_insert(SommelierDBMS *dbms) {
    sqlite3 *con = sommelier_connection(dbms);

    if ((dbms->insert_count + 1) % COMMIT_TRANSACTION_PER_INSERT == 0) {
        if (DEBUG) {
            char msg[100] = "";
            sprintf(msg, "current insert count: %d", dbms->insert_count);
            logging_debug(msg);
        }

        char sql_tr[64] = "";
        sprintf(sql_tr, "COMMIT TRANSACTION %s;",
                current_transaction_name(dbms));
        orm_execute_sql(con, sql_tr, 0, 0);

        dbms->insert_count++;

        sprintf(sql_tr, "BEGIN TRANSACTION %s;",
                current_transaction_name(dbms));

        orm_execute_sql(con, sql_tr, 0, 0);
    } else {
        dbms->insert_count++;
    }

    return con;
}

int orm_execute_sql(sqlite3 *db, const char *sql, sqlite3_callback cb,
                    void *obj) {
    if (DEBUG) {
        char _sql[MAX_SIZE_LOGGING_SQL_TEXT] = "";
        strncpy(_sql, sql, MAX_SIZE_LOGGING_SQL_TEXT - 1);
        _sql[MAX_SIZE_LOGGING_SQL_TEXT - 1] = '\0';

        char *suffix = strlen(sql) >= MAX_SIZE_LOGGING_SQL_TEXT ? "..." : "";

        char msg[MAX_SIZE_LOGGING_SQL_TEXT + 10] = "";
        sprintf(msg, "SQL: %s%s", _sql, suffix);
        logging_debug(msg);
    }

    char *zErrMsg = NULL;
    int rc = sqlite3_exec(db, sql, cb, obj, &zErrMsg);
    if (rc != SQLITE_OK) {
        char msg[200] = "";
        sprintf(msg, "SQL error (%d): %s", rc, zErrMsg);
        logging_error(msg);
        sqlite3_free(zErrMsg);
    }

    return rc;
}
