#include <stdlib.h>

#include "orm.h"
#include "sommelier-db.h"
#include "util.h"

#define DBFILE "data/_test_db_initialize.db"

int main() {
    SommelierDBMS *dbms = initialize_sommelier_dbms();
    OpenSommelierDB(dbms, DBFILE);

    InitalizeDatabase(dbms);

    logging_debug("success: test_db_initialize");

    return 0;
}
