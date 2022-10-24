#include "orm.h"

// TODO: execのerror-handling.

void __exec_simple_sql(sqlite3 *db, const char *sql) {
    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
    }
    sqlite3_free(zErrMsg);
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

// Create User API.

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

User *ReadUser(sqlite3 *db, uint64_t id) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT UserID, DataPublicKey, KeywordPublicKey, Nonce FROM "
            "user_table WHERE UserID = %ld;",
            id);

    if (DEBUG) {
        printf("    sql - %s\n", sql);
    }

    sqlite3_stmt *stmt = NULL;
    int return_value = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (return_value) {
        printf("sqlite3_prepare_v2 is failed. (err_code=%d)\n", return_value);
        exit(return_value);
    }

    User *u = initialize_user();

    return_value = sqlite3_step(stmt);
    if (return_value == SQLITE_ROW) {
        uint64_t id = (uint64_t)sqlite3_column_int(stmt, 0);
        char *pkd = (char *)sqlite3_column_text(stmt, 1);
        char *pkk = (char *)sqlite3_column_text(stmt, 2);
        uint64_t nonce = (uint64_t)sqlite3_column_int(stmt, 3);

        set_user(u, id, pkd, pkk, nonce);
    } else {
        printf("Some error encountered.\n");
        exit(1);
    }

    return u;
}

void IncrementUserNonce(sqlite3 *db, User *u) {
    uint64_t n = increment_nonce(u);

    char sql[MAX_SIZE_SQL_PLANE_TEXT] = "";
    sprintf(sql, "UPDATE user_table SET Nonce=%ld WHERE UserID=%ld;", n, u->id);

    __exec_simple_sql(db, sql);
}

// Create Path API.

Path *CreatePath(sqlite3 *db, uint64_t uid, char *ph, char *ctd, char *ctk) {
    char sql[MAX_SIZE_SQL_CREATE_PATH] = "";
    sprintf(sql,
            "INSERT INTO path_table (UserID, PermissionHash, DataCipherText, "
            "KeywordCipherText) values (%ld, '%s', '%s', '%s')",
            uid, ph, ctd, ctk);

    if (DEBUG) {
        printf("    sql - %s\n", sql);
    }

    uint64_t id = __exec_simple_insert_sql(db, sql);
    Path *p = initialize_path();
    set_path(p, id, uid, ph, ctd, ctk);

    return p;
}

Path *ReadPath(sqlite3 *db, uint64_t id) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT PathID, UserID, PermissionHash, DataCipherText, "
            "KeywordCipherText FROM path_table WHERE PathID = %ld;",
            id);

    if (DEBUG) {
        printf("    sql - %s\n", sql);
    }

    sqlite3_stmt *stmt = NULL;
    int return_value = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (return_value) {
        printf("sqlite3_prepare_v2 is failed. (err_code=%d)\n", return_value);
        exit(return_value);
    }

    Path *p = initialize_path();

    return_value = sqlite3_step(stmt);
    if (return_value == SQLITE_ROW) {
        uint64_t id = (uint64_t)sqlite3_column_int(stmt, 0);
        uint64_t uid = (uint64_t)sqlite3_column_int(stmt, 1);
        char *ph = (char *)sqlite3_column_text(stmt, 2);
        char *ctd = (char *)sqlite3_column_text(stmt, 3);
        char *ctk = (char *)sqlite3_column_text(stmt, 4);

        set_path(p, id, uid, ph, ctd, ctk);
    } else {
        printf("Some error encountered.\n");
        exit(1);
    }

    return p;
}

static int callback_filter_path_by_permission_hash(void *vec, int argc,
                                                   char **argv,
                                                   char **azColName) {
    if (DEBUG) {
        printf("    select - id: %s, skh: %s, ctc: %s\n", argv[0], argv[1],
               argv[2]);
    }

    Path *p = initialize_path();
    set_path(p, AS_U64(argv[0]), AS_U64(argv[1]), argv[2], argv[3], argv[4]);
    push_path_vector((PathVector *)vec, p);
    finalize_path(p);

    return 0;
}

PathVector *FilterByPermissionHash(sqlite3 *db, char *ph) {
    char sql[MAX_SIZE_SQL_FILTER_BY_PREMISSION_HASH] = "";
    sprintf(sql,
            "SELECT PathID, UserID, PermissionHash, DataCipherText, "
            "KeywordCipherText FROM path_table WHERE PermissionHash = '%s';",
            ph);

    if (DEBUG) {
        printf("    sql - %s\n", sql);
    }

    PathVector *vec = initialize_path_vector();
    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, sql, callback_filter_path_by_permission_hash,
                          (void *)vec, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    return vec;
}

// depends on Sommelier-DB
PathVector *SearchEncryptedPath(sqlite3 *db, uint64_t, char *);

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

SharedKey *ReadSharedKey(sqlite3 *db, uint64_t id) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT SharedKeyID, PathID, SharedKeyCipherText FROM "
            "shared_key_table WHERE SharedKeyID = %ld;",
            id);

    sqlite3_stmt *stmt = NULL;
    int return_value = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (return_value) {
        printf("sqlite3_prepare_v2 is failed. (err_code=%d)\n", return_value);
        exit(return_value);
    }

    SharedKey *sk = initialize_shared_key();

    return_value = sqlite3_step(stmt);
    if (return_value == SQLITE_ROW) {
        uint64_t id = (uint64_t)sqlite3_column_int(stmt, 0);
        uint64_t pid = (uint64_t)sqlite3_column_int(stmt, 1);
        char *ctsk = (char *)sqlite3_column_text(stmt, 2);

        set_shared_key(sk, id, pid, ctsk);
    } else {
        printf("Some error encountered.\n");
        exit(1);
    }

    return sk;
}

Content *CreateContent(sqlite3 *db, char *skh, char *ctc) {
    char sql[MAX_SIZE_SQL_CREATE_CONTENT] = "";
    sprintf(sql,
            "INSERT INTO content_table (SharedKeyHash, ContentCipherText) "
            "values ('%s', '%s');",
            skh, ctc);

    if (DEBUG) {
        printf("    sql - %s\n", sql);
    }

    uint64_t id = __exec_simple_insert_sql(db, sql);
    Content *c = initialize_content();
    set_content(c, id, skh, ctc);

    return c;
}

Content *ReadContent(sqlite3 *db, uint64_t id) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT ContentID, SharedKeyHash, ContentCipherText FROM "
            "content_table WHERE ContentID = %ld;",
            id);

    sqlite3_stmt *stmt = NULL;
    int return_value = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (return_value) {
        printf("sqlite3_prepare_v2 is failed. (err_code=%d)\n", return_value);
        exit(return_value);
    }

    Content *c = initialize_content();

    return_value = sqlite3_step(stmt);
    if (return_value == SQLITE_ROW) {
        uint64_t id = (uint64_t)sqlite3_column_int(stmt, 0);
        char *skh = (char *)sqlite3_column_text(stmt, 1);
        char *ctc = (char *)sqlite3_column_text(stmt, 2);

        set_content(c, id, skh, ctc);
    } else {
        printf("Some error encountered.\n");
        exit(1);
    }

    return c;
}

static int callback_filter_contents_by_shared_key_hash(void *vec, int argc,
                                                       char **argv,
                                                       char **azColName) {
    if (DEBUG) {
        printf("    select - id: %s, skh: %s, ctc: %s\n", argv[0], argv[1],
               argv[2]);
    }

    Content *c = initialize_content();
    set_content(c, AS_U64(argv[0]), argv[1], argv[2]);
    push_content_vector((ContentVector *)vec, c);
    finalize_content(c);

    return 0;
}

ContentVector *FilterBySharedKeyHash(sqlite3 *db, char *skh) {
    char sql[MAX_SIZE_SQL_CREATE_WRITE_PERMISSION] = "";
    sprintf(sql,
            "SELECT ContentID, SharedKeyHash, ContentCipherText FROM "
            "content_table WHERE SharedKeyHash = '%s';",
            skh);

    ContentVector *vec = initialize_content_vector();
    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, sql, callback_filter_contents_by_shared_key_hash,
                          (void *)vec, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    return vec;
}

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

WritePermission *ReadWritePermission(sqlite3 *db, uint64_t id) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT WritePermissionID, PathID, UserID FROM "
            "write_permission_table WHERE WritePermissionID = %ld;",
            id);

    sqlite3_stmt *stmt = NULL;
    int return_value = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (return_value) {
        printf("sqlite3_prepare_v2 is failed. (err_code=%d)\n", return_value);
        exit(return_value);
    }

    WritePermission *wp = initialize_write_permission();

    return_value = sqlite3_step(stmt);
    if (return_value == SQLITE_ROW) {
        uint64_t id = (uint64_t)sqlite3_column_int(stmt, 0);
        uint64_t pid = (uint64_t)sqlite3_column_int(stmt, 1);
        uint64_t uid = (uint64_t)sqlite3_column_int(stmt, 2);

        set_write_permission(wp, id, pid, uid);
    } else {
        printf("Some error encountered.\n");
        exit(1);
    }

    return wp;
}
