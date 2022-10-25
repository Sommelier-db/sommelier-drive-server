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

    InitalizeDatabase(db);

    User *u = CreateUser(db, "dpk1", "kpk1");
    Path *p = CreatePath(db, u->id, "hogehoge", "ctd1", "ctk1");

    WritePermission *wp1 = CreateWritePermission(db, p->id, u->id);
    WritePermission *wp2 = ReadWritePermission(db, wp1->id);

    if (DEBUG) {
        debug_write_permission(wp1);
        debug_write_permission(wp2);
    }

    return 0;
}
