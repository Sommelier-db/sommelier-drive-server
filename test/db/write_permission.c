#include <sqlite3.h>
#include <stdlib.h>

#include "orm.h"
#include "util.h"

#define DBFILE "data/_test_db_write_permission.db"

int main() {
    printf("test write_permission create and read:\n");

    sqlite3 *db = NULL;
    int err = sqlite3_open(DBFILE, &db);
    if (err) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    // InitalizeDatabase(db);

    sqlite3_exec(db,
                 "INSERT INTO user_table (DataPublicKey, KeywordPublicKey, "
                 "Nonce) values ('dpk1', 'kpk1', 1)",
                 0, 0, 0);
    char sql[MAX_SIZE_SQL_CREATE_USER] = "";
    int uid = (int)sqlite3_last_insert_rowid(db);
    sprintf(sql,
            "INSERT INTO path_table (UserID, PermissionHash, DataCipherText, "
            "KeywordCipherText) values (%d, 'ph%d', 'dct%d', 'kct%d')",
            uid, uid, uid, uid);
    sqlite3_exec(db, sql, 0, 0, 0);
    int pid = (int)sqlite3_last_insert_rowid(db);

    WritePermission *wp1 = CreateWritePermission(db, pid, uid);
    WritePermission *wp2 = ReadWritePermission(db, wp1->id);

    if (DEBUG) {
        debug_write_permission(wp1);
        debug_write_permission(wp2);
    }

    return 0;
}
