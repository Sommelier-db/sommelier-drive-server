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
    Content *c3 = ReadContent(db, c1->id);

    if (DEBUG) {
        debug_content(c1);
        debug_content(c2);
        debug_content(c3);
    }

    printf("test content filter by SharedKeyHash:\n");

    Content *c4 = CreateContent(db, "skhx", "ctc4");
    Content *c5 = CreateContent(db, "skh", "ctc5");
    Content *c6 = CreateContent(db, "skhx", "ctc6");
    Content *c7 = CreateContent(db, "skh", "ctc7");

    ContentVector *v1 = FilterBySharedKeyHash(db, "skhx");

    if (DEBUG) {
        for (int i = 0; i < v1->length; i++) {
            debug_content(v1->buf[i]);
        }
    }

    ContentVector *v2 = FilterBySharedKeyHash(db, "skh");

    if (DEBUG) {
        for (int i = 0; i < v2->length; i++) {
            debug_content(v2->buf[i]);
        }
    }

    ContentVector *v3 = FilterBySharedKeyHash(db, "skhx");

    if (DEBUG) {
        for (int i = 0; i < v3->length; i++) {
            debug_content(v3->buf[i]);
        }
    }

    return 0;
}
