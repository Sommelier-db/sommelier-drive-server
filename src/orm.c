#include "orm.h"

// TODO: execのerror-handling.

void __exec_simple_sql(sqlite3 *db, const char *sql, sqlite3_callback cb,
                       void *obj) {
    if (DEBUG) {
        printf("  sql - %s\n", sql);
    }

    char *zErrMsg = NULL;
    int rc = sqlite3_exec(db, sql, cb, obj, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error (%d): %s\n", rc, zErrMsg);
        sqlite3_free(zErrMsg);
    }
}

uint64_t __exec_simple_insert_sql(sqlite3 *db, const char *sql,
                                  sqlite3_callback cb, void *obj) {
    __exec_simple_sql(db, sql, cb, obj);
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
            PathID INTEGER NOT NULL UNIQUE,\
            SharedKeyCipherText TEXT NOT NULL,\
            CONSTRAINT path_id_ref_to_path_table FOREIGN KEY (PathID) REFERENCES path_table (PathID)\
        );",                          // (3) create shared_key table
        "CREATE TABLE authorization_seed_table (\
            AuthorizationSeedID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
            PathID INTEGER NOT NULL UNIQUE,\
            AuthorizationSeedCipherText TEXT NOT NULL,\
            CONSTRAINT path_id_ref_to_path_table FOREIGN KEY (PathID) REFERENCES path_table (PathID)\
        );",                          // (4) create authorization_seed table
        "CREATE TABLE content_table (\
            ContentID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
            SharedKeyHash TEXT NOT NULL,\
            AuthorizationPublicKey TEXT NOT NULL,\
            Nonce INTEGER NOT NULL,\
            ContentCipherText TEXT NOT NULL\
        );",                          // (5) create content table
        "CREATE TABLE write_permission_table (\
            WritePermissionID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
            PathID INTEGER NOT NULL UNIQUE,\
            UserID INTEGER NOT NULL,\
            CONSTRAINT content_path_id_ref_to_path_table FOREIGN KEY (PathID) REFERENCES path_table (PathID),\
            CONSTRAINT content_user_id_ref_to_user_table FOREIGN KEY (UserID) REFERENCES user_table (UserID)\
        );",                          // (6) create write_permission table
        "PRAGMA foreign_keys=true;",  // (7) enable forign key
    };

    for (int i = 0; i < SOMMELIER_DRIVE_INITIALIZE_SQL; i++) {
        __exec_simple_sql(db, sql[i], 0, 0);
    }
}

// Create User API.

User *CreateUser(sqlite3 *db, char *pkd, char *pkk) {
    char sql[MAX_SIZE_SQL_CREATE_USER] = "";
    sprintf(sql,
            "INSERT INTO user_table (DataPublicKey, KeywordPublicKey, Nonce) "
            "values ('%s', '%s', 1);",
            pkd, pkk);

    uint64_t id = __exec_simple_insert_sql(db, sql, 0, 0);
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
        printf("  sql - %s\n", sql);
    }

    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        if (DEBUG) {
            char msg[100] = "";
            sprintf(msg, "sqlite3_prepare_v2 is failed. (err_code=%d)\n", rc);
            errordebug(msg);
        }
        return NULL;
    }

    User *u = initialize_user();

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        uint64_t id = (uint64_t)sqlite3_column_int(stmt, 0);
        char *pkd = (char *)sqlite3_column_text(stmt, 1);
        char *pkk = (char *)sqlite3_column_text(stmt, 2);
        uint64_t nonce = (uint64_t)sqlite3_column_int(stmt, 3);

        set_user(u, id, pkd, pkk, nonce);
        return u;
    } else {
        if (DEBUG) {
            errordebug("Some error encountered. - sqlite3_step");
        }

        finalize_user(u);
        return NULL;
    }

    return NULL;
}

void IncrementUserNonce(sqlite3 *db, User *u) {
    uint64_t n = increment_user_nonce(u);
    char sql[MAX_SIZE_SQL_PLANE_TEXT] = "";
    sprintf(sql, "UPDATE user_table SET Nonce=%ld WHERE UserID=%ld;", n, u->id);

    __exec_simple_sql(db, sql, 0, 0);
}

// Create Path API.

Path *CreatePath(sqlite3 *db, uint64_t uid, char *ph, char *ctd, char *ctk) {
    char sql[MAX_SIZE_SQL_CREATE_PATH] = "";
    sprintf(sql,
            "INSERT INTO path_table (UserID, PermissionHash, DataCipherText, "
            "KeywordCipherText) values (%ld, '%s', '%s', '%s');",
            uid, ph, ctd, ctk);

    uint64_t id = __exec_simple_insert_sql(db, sql, 0, 0);
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
        printf("  sql - %s\n", sql);
    }

    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        if (DEBUG) {
            char msg[100] = "";
            sprintf(msg, "sqlite3_prepare_v2 is failed. (err_code=%d)\n", rc);
            errordebug(msg);
        }
        return NULL;
    }

    Path *p = initialize_path();

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        uint64_t id = (uint64_t)sqlite3_column_int(stmt, 0);
        uint64_t uid = (uint64_t)sqlite3_column_int(stmt, 1);
        char *ph = (char *)sqlite3_column_text(stmt, 2);
        char *ctd = (char *)sqlite3_column_text(stmt, 3);
        char *ctk = (char *)sqlite3_column_text(stmt, 4);

        set_path(p, id, uid, ph, ctd, ctk);
        return p;
    } else {
        if (DEBUG) {
            errordebug("Some error encountered. - sqlite3_step");
        }

        finalize_path(p);
        return NULL;
    }

    return NULL;
}

static int callback_path_row(void *vec, int argc, char **argv,
                             char **azColName) {
    Path p = {AS_U64(argv[0]), AS_U64(argv[1]), argv[2], argv[3], argv[4]};

    if (DEBUG) {
        debug_path(&p);
    }

    push_path_vector((PathVector *)vec, &p);

    return 0;
}

// depends on Sommelier-DB
PathVector *SearchEncryptedPath(sqlite3 *db, uint64_t uid, char *trapdoor) {
    char sql[MAX_SIZE_SQL_FILTER_BY_PREMISSION_HASH] = "";
    sprintf(sql,
            "SELECT PathID, UserID, PermissionHash, DataCipherText, "
            "KeywordCipherText FROM path_table WHERE "
            "test_cipher(KeywordCipherText, '%s') = 1;",
            trapdoor);

    PathVector *vec = initialize_path_vector();

    __exec_simple_sql(db, sql, callback_path_row, (void *)vec);

    return vec;
}

PathVector *FilterByPermissionHash(sqlite3 *db, char *ph) {
    char sql[MAX_SIZE_SQL_FILTER_BY_PREMISSION_HASH] = "";
    sprintf(sql,
            "SELECT PathID, UserID, PermissionHash, DataCipherText, "
            "KeywordCipherText FROM path_table WHERE PermissionHash = '%s';",
            ph);

    PathVector *vec = initialize_path_vector();

    __exec_simple_sql(db, sql, callback_path_row, (void *)vec);

    return vec;
}

SharedKey *CreateSharedKey(sqlite3 *db, uint64_t pid, char *ctsk) {
    char sql[MAX_SIZE_SQL_CREATE_SHARED_KEY] = "";
    sprintf(sql,
            "INSERT INTO shared_key_table (PathID, SharedKeyCipherText) values "
            "(%ld, '%s')",
            pid, ctsk);

    uint64_t id = __exec_simple_insert_sql(db, sql, 0, 0);
    SharedKey *sk = initialize_shared_key();
    set_shared_key(sk, id, pid, ctsk);

    return sk;
}

SharedKey *ReadSharedKey(sqlite3 *db, uint64_t pid) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT SharedKeyID, PathID, SharedKeyCipherText FROM "
            "shared_key_table WHERE PathID = %ld;",
            pid);

    if (DEBUG) {
        printf("  sql - %s\n", sql);
    }

    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        if (DEBUG) {
            char msg[100] = "";
            sprintf(msg, "sqlite3_prepare_v2 is failed. (err_code=%d)\n", rc);
            errordebug(msg);
        }
        return NULL;
    }

    SharedKey *sk = initialize_shared_key();

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        uint64_t id = (uint64_t)sqlite3_column_int(stmt, 0);
        uint64_t pid = (uint64_t)sqlite3_column_int(stmt, 1);
        char *ctsk = (char *)sqlite3_column_text(stmt, 2);

        set_shared_key(sk, id, pid, ctsk);
        return sk;
    } else {
        if (DEBUG) {
            errordebug("Some error encountered. - sqlite3_step");
        }

        finalize_shared_key(sk);
        return NULL;
    }

    return NULL;
}

AuthorizationSeed *CreateAuthorizationSeed(sqlite3 *db, uint64_t pid,
                                           char *ctas) {
    char sql[MAX_SIZE_SQL_CREATE_AUTHORIZATION_SEED] = "";
    sprintf(sql,
            "INSERT INTO authorization_seed_table (PathID, "
            "AuthorizationSeedCipherText) values (%ld, '%s');",
            pid, ctas);

    uint64_t id = __exec_simple_insert_sql(db, sql, 0, 0);
    AuthorizationSeed *as = initialize_authorization_seed();
    set_authorization_seed(as, id, pid, ctas);

    return as;
}

AuthorizationSeed *ReadAuthorizationSeed(sqlite3 *db, uint64_t pid) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT AuthorizationSeedID, PathID, AuthorizationSeedCipherText "
            "FROM authorization_seed_table WHERE PathID = %ld;",
            pid);

    if (DEBUG) {
        printf("  sql - %s\n", sql);
    }

    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        if (DEBUG) {
            char msg[100] = "";
            sprintf(msg, "sqlite3_prepare_v2 is failed. (err_code=%d)\n", rc);
            errordebug(msg);
        }
        return NULL;
    }

    AuthorizationSeed *as = initialize_authorization_seed();

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        uint64_t id = (uint64_t)sqlite3_column_int(stmt, 0);
        uint64_t pid = (uint64_t)sqlite3_column_int(stmt, 1);
        char *ctas = (char *)sqlite3_column_text(stmt, 2);

        set_authorization_seed(as, id, pid, ctas);
        return as;
    } else {
        if (DEBUG) {
            errordebug("Some error encountered. - sqlite3_step");
        }

        finalize_authorization_seed(as);
        return NULL;
    }

    return NULL;
}

Content *CreateContent(sqlite3 *db, char *skh, char *pka, char *ctc) {
    char sql[MAX_SIZE_SQL_CREATE_CONTENT] = "";
    sprintf(sql,
            "INSERT INTO content_table (SharedKeyHash, AuthorizationPublicKey, "
            "Nonce, ContentCipherText) values ('%s', '%s', 1, '%s');",
            skh, pka, ctc);

    uint64_t id = __exec_simple_insert_sql(db, sql, 0, 0);
    Content *c = initialize_content();
    set_content(c, id, skh, pka, 1, ctc);

    return c;
}

Content *ReadContent(sqlite3 *db, uint64_t id) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT ContentID, SharedKeyHash, AuthorizationPublicKey, Nonce, "
            "ContentCipherText FROM content_table WHERE ContentID = %ld;",
            id);

    if (DEBUG) {
        printf("  sql - %s\n", sql);
    }

    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        if (DEBUG) {
            char msg[100] = "";
            sprintf(msg, "sqlite3_prepare_v2 is failed. (err_code=%d)\n", rc);
            errordebug(msg);
        }
        return NULL;
    }

    Content *c = initialize_content();

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        uint64_t id = (uint64_t)sqlite3_column_int(stmt, 0);
        char *skh = (char *)sqlite3_column_text(stmt, 1);
        char *pka = (char *)sqlite3_column_text(stmt, 2);
        uint64_t nonce = (uint64_t)sqlite3_column_int(stmt, 3);
        char *ctc = (char *)sqlite3_column_text(stmt, 4);

        set_content(c, id, skh, pka, nonce, ctc);
        return c;
    } else {
        if (DEBUG) {
            errordebug("Some error encountered. - sqlite3_step");
        }

        finalize_content(c);
        return NULL;
    }
}

Content *ReadContentBySharedKeyHash(sqlite3 *db, char *skh) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT ContentID, SharedKeyHash, AuthorizationPublicKey, Nonce, "
            "ContentCipherText FROM content_table WHERE SharedKeyHash = '%s';",
            skh);

    if (DEBUG) {
        printf("  sql - %s\n", sql);
    }

    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        if (DEBUG) {
            char msg[100] = "";
            sprintf(msg, "sqlite3_prepare_v2 is failed. (err_code=%d)\n", rc);
            errordebug(msg);
        }
        return NULL;
    }

    Content *c = initialize_content();

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        uint64_t id = (uint64_t)sqlite3_column_int(stmt, 0);
        char *skh = (char *)sqlite3_column_text(stmt, 1);
        char *pka = (char *)sqlite3_column_text(stmt, 2);
        uint64_t nonce = (uint64_t)sqlite3_column_int(stmt, 3);
        char *ctc = (char *)sqlite3_column_text(stmt, 4);

        set_content(c, id, skh, pka, nonce, ctc);
        return c;
    } else {
        if (DEBUG) {
            errordebug("Some error encountered. - sqlite3_step");
        }

        finalize_content(c);
        return NULL;
    }
}

void UpdateContent(sqlite3 *db, Content *c) {
    char sql[MAX_SIZE_SQL_PLANE_TEXT] = "";
    sprintf(sql,
            "UPDATE content_table SET Nonce=%ld, ContentCipherText='%s' WHERE "
            "ContentID=%ld;",
            c->nonce, c->content_cipher_text, c->id);

    __exec_simple_sql(db, sql, 0, 0);
}

void IncrementContentNonce(sqlite3 *db, Content *c) {
    uint64_t n = increment_content_nonce(c);
    char sql[MAX_SIZE_SQL_PLANE_TEXT] = "";
    sprintf(sql, "UPDATE content_table SET Nonce=%ld WHERE ContentID=%ld;", n,
            c->id);

    __exec_simple_sql(db, sql, 0, 0);
}

static int callback_content_row(void *vec, int argc, char **argv,
                                char **azColName) {
    Content c = {AS_U64(argv[0]), argv[1], argv[2], AS_U64(argv[3]), argv[4]};

    if (DEBUG) {
        debug_content(&c);
    }

    push_content_vector((ContentVector *)vec, &c);

    return 0;
}

ContentVector *FilterBySharedKeyHash(sqlite3 *db, char *skh) {
    char sql[MAX_SIZE_SQL_CREATE_WRITE_PERMISSION] = "";
    sprintf(sql,
            "SELECT ContentID, SharedKeyHash, AuthorizationPublicKey, Nonce,  "
            "ContentCipherText FROM content_table WHERE SharedKeyHash = '%s';",
            skh);

    ContentVector *vec = initialize_content_vector();

    __exec_simple_sql(db, sql, callback_content_row, (void *)vec);

    return vec;
}

WritePermission *CreateWritePermission(sqlite3 *db, uint64_t pid,
                                       uint64_t uid) {
    char sql[MAX_SIZE_SQL_CREATE_WRITE_PERMISSION] = "";
    sprintf(sql,
            "INSERT INTO write_permission_table (PathID, UserID) "
            "values (%ld, %ld);",
            pid, uid);

    uint64_t id = __exec_simple_insert_sql(db, sql, 0, 0);
    WritePermission *wp = initialize_write_permission();
    set_write_permission(wp, id, pid, uid);

    return wp;
}

WritePermission *ReadWritePermission(sqlite3 *db, uint64_t pid) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT WritePermissionID, PathID, UserID FROM "
            "write_permission_table WHERE PathID = %ld;",
            pid);

    if (DEBUG) {
        printf("  sql - %s\n", sql);
    }

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
