#include <stdlib.h>

#include "orm.h"
#include "sommelier-db.h"
#include "util.h"

#define DBFILE "data/_test_db_initialize.db"

int main() {
    sqlite3 *db = NULL;
    int err = sqlite3_open(DBFILE, &db);
    if (err) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    InitalizeDatabase(db);

    return 0;
}
