#include <stdlib.h>

#include "orm.h"
#include "sommelier-db.h"
#include "util.h"

#define DBFILE "data/_test_db_path.db"

int main() {
    printf("test path create and read:\n");

    SommelierDBMS *dbms = initialize_sommelier_dbms();
    OpenSommelierDB(dbms, DBFILE);
    InitalizeDatabase(dbms);

    User *u = CreateUser(dbms, "pkepk1111", "pksepk1111");
    Path *p = CreatePath(dbms, u->id, "ph1111", "ctd1111", "ctk1111");
    Path *p1 = ReadPath(dbms, p->id);
    Path *p2 = ReadPath(dbms, p->id);

    if (DEBUG) {
        debug_path(p);
        debug_path(p1);
        debug_path(p2);
    }

    for (int i = 0; i < 3; i++) {
        Path *p = CreatePath(dbms, u->id, "ph", "ctd", "ctk");
        finalize_path(p);
    }

    PathVector *vec = FilterByPermissionHash(dbms, "ph");

    // PathVector *vec2 = SearchEncryptedPath(dbms, u->id, "trapdoor");

    printf("vector size: %ld\n", vec->length);
    for (int i = 0; i < vec->length; i++) {
        debug_path(vec->buf[i]);
    }
}