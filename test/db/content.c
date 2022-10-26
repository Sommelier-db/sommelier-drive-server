#include <stdlib.h>

#include "orm.h"
#include "sommelier-db.h"
#include "util.h"

#define DBFILE "data/_test_db_content.db"

int main() {
    printf("test content create and read:\n");

    SommelierDBMS *dbms = initialize_sommelier_dbms();
    OpenSommelierDB(dbms, DBFILE);
    InitalizeDatabase(dbms);
    StartTransaction(dbms);

    Content *c1 = CreateContent(dbms, "skh1", "pka1", "ctc1");
    Content *c2 = ReadContent(dbms, c1->id);
    Content *c3 = ReadContent(dbms, c1->id);

    if (DEBUG) {
        debug_content(c1);
        debug_content(c2);
        debug_content(c3);
    }

    printf("test content filter by SharedKeyHash:\n");

    Content *c4 = CreateContent(dbms, "skhx", "pka4", "ctc4");
    finalize_content(c4);
    Content *c5 = CreateContent(dbms, "skhy", "pka5", "ctc5");
    finalize_content(c5);
    Content *c6 = CreateContent(dbms, "skhx", "pka6", "ctc6");
    finalize_content(c6);
    Content *c7 = CreateContent(dbms, "skhy", "pka7", "ctc7");
    finalize_content(c7);

    ContentVector *v1 = FilterBySharedKeyHash(dbms, "skhx");

    if (DEBUG) {
        for (int i = 0; i < v1->length; i++) {
            debug_content(v1->buf[i]);
        }
    }

    CloseSommelierDB(dbms);

    return 0;
}
