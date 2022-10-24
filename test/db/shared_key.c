#include <sqlite3.h>
#include <stdlib.h>

#include "orm.h"
#include "util.h"

#define DBFILE "data/_test_db_shared_key.db"

int main() {
    printf("test shared_key create and read:\n");

    sqlite3 *db = NULL;
    int err = sqlite3_open(DBFILE, &db);
    if (err) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    InitalizeDatabase(db);

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

    SharedKey *sk1 = CreateSharedKey(db, pid, "ctsk");
    SharedKey *sk2 = ReadSharedKey(db, sk1->id);
    SharedKey *sk3 = ReadSharedKey(db, sk1->id);

    if (DEBUG) {
        debug_shared_key(sk1);
        debug_shared_key(sk2);
        debug_shared_key(sk3);
    }

    return 0;
}
