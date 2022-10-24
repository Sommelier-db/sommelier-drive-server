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

    InitalizeDatabase(db);

    Content *c1 = CreateContent(db, "skh", "ctc1");
    Content *c2 = ReadContent(db, c1->id);

    if (DEBUG) {
        debug_content(c1);
        debug_content(c2);
    }

    printf("test content filter by SharedKeyHash:\n");

    Content *c3 = CreateContent(db, "skh", "ctc2");
    Content *c4 = CreateContent(db, "skhx", "ctc3");
    Content *c5 = CreateContent(db, "skh", "ctc4");
    Content *c6 = CreateContent(db, "skhx", "ctc5");
    Content *c7 = CreateContent(db, "skh", "ctc6");

    ContentVector *v = FilterBySharedKeyHash(db, "skhx");

    if (DEBUG) {
        for (int i = 0; i < v->length; i++) {
            debug_content(v->buf[i]);
        }
    }

    finalize_content(c1);
    finalize_content(c2);
    finalize_content(c3);
    finalize_content(c4);
    finalize_content(c5);
    finalize_content(c6);
    finalize_content(c7);

    return 0;
}
