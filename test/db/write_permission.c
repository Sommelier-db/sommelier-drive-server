#include <stdlib.h>

#include "orm.h"
#include "sommelier-db.h"
#include "util.h"

#define DBFILE "data/_test_db_write_permission.db"

int main() {
    printf("test write_permission create and read:\n");

    SommelierDBMS *dbms = initialize_sommelier_dbms();
    OpenSommelierDB(dbms, DBFILE);
    InitalizeDatabase(dbms);
    StartTransaction(dbms);

    User *u = CreateUser(dbms, "dpk1", "kpk1");
    Path *p = CreatePath(dbms, u->id, "hogehoge", "ctd1", "ctk1");

    WritePermission *wp1 = CreateWritePermission(dbms, p->id, u->id);
    WritePermission *wp2 = ReadWritePermission(dbms, wp1->id);

    if (DEBUG) {
        debug_write_permission(wp1);
        debug_write_permission(wp2);
    }

    CloseSommelierDB(dbms);

    return 0;
}
