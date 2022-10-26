#include <stdlib.h>

#include "orm.h"
#include "sommelier-db.h"
#include "util.h"

#define DBFILE "data/_test_db_user.db"

int main() {
    printf("test user create and read:\n");

    sqlite3 *db = NULL;
    int err = sqlite3_open(DBFILE, &db);
    if (err) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    InitalizeDatabase(db);

    printf("1. create user u.\n");
    User *u1 = CreateUser(db, "pkd1", "pkk1");

    printf("2. read user (id = %ld).\n", u1->id);
    User *u2 = ReadUser(db, u1->id);

    if (DEBUG) {
        logging_debug("show u1 and u2");
        logging_debug("u1:");
        debug_user(u1);
        logging_debug("u2:");
        debug_user(u2);
    }

    printf("3. increment nonce of user (id = %ld).\n", u1->id);
    IncrementUserNonce(db, u1);

    printf("4. read user (id = %ld) again, nonce must be incremented.\n",
           u1->id);
    User *u3 = ReadUser(db, u1->id);

    if (DEBUG) {
        logging_debug("show u1, u2 and u3");
        logging_debug("u1:");
        debug_user(u1);
        logging_debug("u2:");
        debug_user(u2);
        logging_debug("u3:");
        debug_user(u3);
    }

    printf("4. read user (id = %ld) must be failed.\n", u1->id + 100);
    User *u4 = ReadUser(db, u1->id + 100);
    logging_debug(u4 == NULL ? "u4 is NULL (test success)"
                             : "u4 is not NULL (test fail)");

    printf("5. create user with invalid text must be failed.\n");
    User *u5 = CreateUser(db, "'hoge'", "'fuga'");
    logging_debug(u5 == NULL ? "u5 is NULL (test success)"
                             : "u5 is not NULL (test fail)");

    return 0;
}
