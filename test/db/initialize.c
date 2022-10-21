#include <sqlite3.h>
#include <stdlib.h>

#include "util.h"

#define DBFILE "data/_test_db_initialize.db"

int main() {
    sqlite3 *db = NULL;
    int err = sqlite3_open(DBFILE, &db);

    return 0;
}
