#include <stdlib.h>

#include "orm.h"
#include "sommelier-db.h"
#include "util.h"

#define DBFILE "data/_test_db_shared_key.db"

int main() {
    printf("test shared_key create and read:\n");

    SommelierDBMS *dbms = initialize_sommelier_dbms();
    OpenSommelierDB(dbms, DBFILE);
    InitalizeDatabase(dbms);
    StartTransaction(dbms);

    User *u = CreateUser(dbms, "dpk1", "kpk1");
    Path *p = CreatePath(dbms, u->id, "ph1", "data_ct1", "keyword_ct1");

    SharedKey *sk1 = CreateSharedKey(dbms, p->id, "ctsk");
    SharedKey *sk2 = ReadSharedKey(dbms, sk1->id);
    SharedKey *sk3 = ReadSharedKey(dbms, sk1->id);

    if (DEBUG) {
        debug_shared_key(sk1);
        debug_shared_key(sk2);
        debug_shared_key(sk3);
    }

    CloseSommelierDB(dbms);

    return 0;
}
