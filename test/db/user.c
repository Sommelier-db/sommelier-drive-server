#include <sqlite3.h>
#include <stdlib.h>

#include "orm.h"
#include "util.h"

#define DBFILE "data/_test_db_user.db"

int main() {
    printf("test user create and read:\n");

    sqlite3 *db = NULL;
    int err = sqlite3_open(DBFILE, &db);
    if (err) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    InitalizeDatabase(db);

    User *u1 = CreateUser(db, "pkd1", "pkk1");
    User *u2 = ReadUser(db, u1->id);

    if (DEBUG) {
        debug_user(u1);
        debug_user(u2);
    }

    IncrementUserNonce(db, u1);

    User *u3 = ReadUser(db, u1->id);

    if (DEBUG) {
        debug_user(u1);
        debug_user(u2);
        debug_user(u3);
    }

    return 0;
}
