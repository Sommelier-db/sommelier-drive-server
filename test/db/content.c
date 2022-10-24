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

    Content *c1 = CreateContent(db, "skh1", "pka1", "ctc1");
    Content *c2 = ReadContent(db, c1->id);
    Content *c3 = ReadContent(db, c1->id);

    if (DEBUG) {
        debug_content(c1);
        debug_content(c2);
        debug_content(c3);
    }

    printf("test content filter by SharedKeyHash:\n");

    Content *c4 = CreateContent(db, "skhx", "pka4", "ctc4");
    finalize_content(c4);
    Content *c5 = CreateContent(db, "skhy", "pka5", "ctc5");
    finalize_content(c5);
    Content *c6 = CreateContent(db, "skhx", "pka6", "ctc6");
    finalize_content(c6);
    Content *c7 = CreateContent(db, "skhy", "pka7", "ctc7");
    finalize_content(c7);

    ContentVector *v1 = FilterBySharedKeyHash(db, "skhx");

    if (DEBUG) {
        for (int i = 0; i < v1->length; i++) {
            debug_content(v1->buf[i]);
        }
    }

    return 0;
}
