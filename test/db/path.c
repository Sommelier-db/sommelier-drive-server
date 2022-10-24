#include <sqlite3.h>
#include <stdlib.h>

#include "orm.h"
#include "util.h"

#define DBFILE "data/_test_db_path.db"

int main() {
    printf("test path create and read:\n");

    sqlite3 *db = NULL;
    int err = sqlite3_open(DBFILE, &db);
    if (err) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    InitalizeDatabase(db);

    User *u = CreateUser(db, "pkepk1111", "pksepk1111");
    Path *p = CreatePath(db, u->id, "ph1111", "ctd1111", "ctk1111");
    Path *p1 = ReadPath(db, p->id);
    Path *p2 = ReadPath(db, p->id);

    if (DEBUG) {
        debug_path(p);
        debug_path(p1);
        debug_path(p2);
    }

    for (int i = 0; i < 3; i++) {
        Path *p = CreatePath(db, u->id, "ph", "ctd", "ctk");
        finalize_path(p);
    }

    PathVector *vec = FilterByPermissionHash(db, "ph");

    printf("vector size: %ld\n", vec->length);
    for (int i = 0; i < vec->length; i++) {
        debug_path(vec->buf[i]);
    }
}