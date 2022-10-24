#include <sqlite3.h>
#include <stdlib.h>

#include "orm.h"
#include "util.h"

#define DBFILE "data/_test_db_content.db"

int main() {
    printf("test content create and read:\n");

    sqlite3 *db = NULL;
    int err = sqlite3_open(DBFILE, &db);
    if (err) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    // InitalizeDatabase(db);

    Content *c1 = CreateContent(db, "skh1", "ctc1");
    Content *c2 = ReadContent(db, c1->id);

    if (DEBUG) {
        debug_content(c1);
        debug_content(c2);
    }

    return 0;
}
