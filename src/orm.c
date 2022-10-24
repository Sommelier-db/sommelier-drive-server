#include "orm.h"

void __exec_simple_sql(sqlite3 *db, const char *sql) {
    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}

uint64_t __exec_simple_insert_sql(sqlite3 *db, const char *sql) {
    __exec_simple_sql(db, sql);
    return (uint64_t)sqlite3_last_insert_rowid(db);
}

void InitalizeDatabase(sqlite3 *db) {
    // TODO: already exists対策

    const char sql[SOMMELIER_DRIVE_INITIALIZE_SQL][MAX_SIZE_SQL_PLANE_TEXT] = {
        "CREATE TABLE user_table (\
            UserID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
            DataPublicKey TEXT NOT NULL,\
            KeywordPublicKey TEXT NOT NULL,\
            Nonce INTEGER NOT NULL\
        );",                          // (1) create user table
        "CREATE TABLE path_table (\
            PathID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
            UserID INTEGER NOT NULL,\
            PermissionHash TEXT NOT NULL,\
            DataCipherText TEXT NOT NULL,\
            KeywordCipherText TEXT NOT NULL,\
            CONSTRAINT path_user_id_ref_to_user_table FOREIGN KEY (UserID) REFERENCES user_table (UserID)\
        );",                          // (2) create path table
        "CREATE TABLE shared_key_table (\
            SharedKeyID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
            PathID INTEGER NOT NULL,\
            SharedKeyCipherText TEXT NOT NULL,\
            CONSTRAINT path_id_ref_to_path_table FOREIGN KEY (PathID) REFERENCES path_table (PathID)\
        );",                          // (3) create shared_key table
        "CREATE TABLE content_table (\
            ContentID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
            SharedKeyHash TEXT NOT NULL,\
            ContentCipherText TEXT NOT NULL\
        );",                          // (4) create content table
        "CREATE TABLE write_permission_table (\
            WritePermissionID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
            PathID INTEGER NOT NULL,\
            UserID INTEGER NOT NULL,\
            CONSTRAINT content_path_id_ref_to_path_table FOREIGN KEY (PathID) REFERENCES path_table (PathID),\
            CONSTRAINT content_user_id_ref_to_user_table FOREIGN KEY (UserID) REFERENCES user_table (UserID)\
        );",                          // (5) create write_permission table
        "PRAGMA foreign_keys=true;",  // (6) enable forign key
    };

    for (int i = 0; i < SOMMELIER_DRIVE_INITIALIZE_SQL; i++) {
        __exec_simple_sql(db, sql[i]);
    }
}

User *CreateUser(sqlite3 *db, char *pkd, char *pkk) {
    char sql[MAX_SIZE_SQL_CREATE_USER] = "";
    sprintf(sql,
            "INSERT INTO user_table (DataPublicKey, KeywordPublicKey, Nonce) "
            "values ('%s', '%s', 1)",
            pkd, pkk);

    if (DEBUG) {
        printf("    sql - %s\n", sql);
    }

    uint64_t id = __exec_simple_insert_sql(db, sql);
    User *u = initialize_user();
    set_user(u, id, pkd, pkk, 1);

    return u;
}

static int callback_get_user(void *row, int argc, char **argv,
                             char **azColName) {
    if (DEBUG) {
        printf("    select - id: %s, pkd: %s, pkk: %s, nonce: %s\n", argv[0],
               argv[1], argv[2], argv[3]);
    }

    set_user((User *)row, AS_U64(argv[0]), argv[1], argv[2], AS_U64(argv[3]));

    return 0;
}

User *ReadUser(sqlite3 *db, uint64_t id) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT UserID, DataPublicKey, KeywordPublicKey, Nonce FROM "
            "user_table WHERE UserID = %ld;",
            id);

    User *row = initialize_user();

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, sql, callback_get_user, (void *)row, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    return row;
}

void IncrementUserNonce(sqlite3 *, User *);

SharedKey *CreateSharedKey(sqlite3 *db, uint64_t pid, char *ctsk) {
    char sql[MAX_SIZE_SQL_CREATE_SHARED_KEY] = "";
    sprintf(sql,
            "INSERT INTO shared_key_table (PathID, SharedKeyCipherText) values "
            "(%ld, '%s')",
            pid, ctsk);

    if (DEBUG) {
        printf("    sql - %s\n", sql);
    }

    uint64_t id = __exec_simple_insert_sql(db, sql);
    SharedKey *sk = initialize_shared_key();
    set_shared_key(sk, id, pid, ctsk);

    return sk;
}

static int callback_get_shared_key(void *row, int argc, char **argv,
                                   char **azColName) {
    if (DEBUG) {
        printf("    select - id: %s, pid: %s, ctsk: %s\n", argv[0], argv[1],
               argv[2]);
    }

    set_shared_key((SharedKey *)row, AS_U64(argv[0]), AS_U64(argv[1]), argv[2]);

    return 0;
}

SharedKey *ReadSharedKey(sqlite3 *db, uint64_t id) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT SharedKeyID, PathID, SharedKeyCipherText FROM "
            "shared_key_table WHERE SharedKeyID = %ld;",
            id);

    SharedKey *row = initialize_shared_key();

    char *zErrMsg = 0;
    int rc =
        sqlite3_exec(db, sql, callback_get_shared_key, (void *)row, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    return row;
}

Content *CreateContent(sqlite3 *db, char *skh, char *ctc) {
    char sql[MAX_SIZE_SQL_CREATE_CONTENT] = "";
    sprintf(sql,
            "INSERT INTO content_table (SharedKeyHash, ContentCipherText) "
            "values ('%s', '%s')",
            skh, ctc);

    if (DEBUG) {
        printf("    sql - %s\n", sql);
    }

    uint64_t id = __exec_simple_insert_sql(db, sql);
    Content *c = initialize_content();
    set_content(c, id, skh, ctc);

    return c;
}

static int callback_get_content(void *row, int argc, char **argv,
                                char **azColName) {
    if (DEBUG) {
        printf("    select - id: %s, skh: %s, ctc: %s\n", argv[0], argv[1],
               argv[2]);
    }

    set_content((Content *)row, AS_U64(argv[0]), argv[1], argv[2]);

    return 0;
}

Content *ReadContent(sqlite3 *db, uint64_t id) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT ContentID, SharedKeyHash, ContentCipherText FROM "
            "content_table WHERE ContentID = %ld;",
            id);

    Content *row = initialize_content();
    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, sql, callback_get_content, (void *)row, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    return row;
}

ContentVector *FilterBySharedKeyHash(sqlite3 *db, char *skh);

WritePermission *CreateWritePermission(sqlite3 *db, uint64_t pid,
                                       uint64_t uid) {
    char sql[MAX_SIZE_SQL_CREATE_WRITE_PERMISSION] = "";
    sprintf(sql,
            "INSERT INTO write_permission_table (PathID, UserID) "
            "values (%ld, %ld)",
            pid, uid);

    if (DEBUG) {
        printf("    sql - %s\n", sql);
    }

    uint64_t id = __exec_simple_insert_sql(db, sql);
    WritePermission *wp = initialize_write_permission();
    set_write_permission(wp, id, pid, uid);

    return wp;
}

static int callback_get_write_permission(void *row, int argc, char **argv,
                                         char **azColName) {
    if (DEBUG) {
        printf("    select - id: %s, pid: %s, uid: %s\n", argv[0], argv[1],
               argv[2]);
    }

    set_write_permission((WritePermission *)row, AS_U64(argv[0]),
                         AS_U64(argv[1]), AS_U64(argv[2]));

    return 0;
}

WritePermission *ReadWritePermission(sqlite3 *db, uint64_t id) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT WritePermissionID, PathID, UserID FROM "
            "write_permission_table WHERE WritePermissionID = %ld;",
            id);

    WritePermission *row = initialize_write_permission();

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, sql, callback_get_write_permission, (void *)row,
                          &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    return row;
}
