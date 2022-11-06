#include "orm.h"

uint64_t __last_inserted_id(sqlite3 *db) {
    return (uint64_t)sqlite3_last_insert_rowid(db);
}

void InitalizeDatabase(SommelierDBMS *dbms) {
    // TODO: already exists対策

    const char sql[SOMMELIER_DRIVE_INITIALIZE_SQL][MAX_SIZE_SQL_PLANE_TEXT] = {
        "CREATE TABLE user_table (\
            UserID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
            DataPublicKey TEXT NOT NULL,\
            KeywordPublicKey TEXT NOT NULL,\
            Nonce INTEGER NOT NULL\
        );",  // (1) create user table
        "CREATE TABLE path_table (\
            PathID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
            UserID INTEGER NOT NULL,\
            PermissionHash TEXT NOT NULL,\
            DataCipherText TEXT NOT NULL,\
            KeywordCipherText TEXT NOT NULL,\
            CONSTRAINT path_user_id_ref_to_user_table FOREIGN KEY (UserID) REFERENCES user_table (UserID)\
        );",  // (2) create path table
        "CREATE TABLE shared_key_table (\
            SharedKeyID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
            PathID INTEGER NOT NULL UNIQUE,\
            SharedKeyCipherText TEXT NOT NULL,\
            CONSTRAINT path_id_ref_to_path_table FOREIGN KEY (PathID) REFERENCES path_table (PathID)\
        );",  // (3) create shared_key table
        "CREATE TABLE content_table (\
            ContentID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
            SharedKeyHash TEXT NOT NULL,\
            Nonce INTEGER NOT NULL,\
            ContentCipherText TEXT NOT NULL\
        );",  // (5) create content table
        /*"CREATE TABLE authorization_seed_table (\
            AuthorizationSeedID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
            PathID INTEGER NOT NULL UNIQUE,\
            AuthorizationSeedCipherText TEXT NOT NULL,\
            CONSTRAINT path_id_ref_to_path_table FOREIGN KEY (PathID) REFERENCES
        path_table (PathID)\
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
            CONSTRAINT content_path_id_ref_to_path_table FOREIGN KEY (PathID)
        REFERENCES path_table (PathID),\
            CONSTRAINT content_user_id_ref_to_user_table FOREIGN KEY (UserID)
        REFERENCES user_table (UserID)\
        );",                          // (6) create write_permission table */
        "PRAGMA foreign_keys=true;",  // (7) enable forign key
    };

    for (int i = 0; i < SOMMELIER_DRIVE_INITIALIZE_SQL; i++) {
        orm_execute_sql(sommelier_connection(dbms), sql[i], 0, 0);
    }
}

// Create User API.

User *CreateUser(SommelierDBMS *dbms, char *pkd, char *pkk) {
    char sql[MAX_SIZE_SQL_CREATE_USER] = "";
    sprintf(sql,
            "INSERT INTO user_table (DataPublicKey, KeywordPublicKey, Nonce) "
            "values ('%s', '%s', 1);",
            pkd, pkk);

    int rc = orm_execute_sql(sommelier_connection_with_insert(dbms), sql, 0, 0);

    if (rc == SQLITE_OK) {
        uint64_t id = __last_inserted_id(dbms->db);
        User *u = initialize_user();
        set_user(u, id, pkd, pkk, 1);

        if (DEBUG) {
            debug_user(u);
        }

        return u;
    } else {
        return NULL;
    }
}

static int callback_set_user(void *user, int argc, char **argv,
                             char **azColName) {
    set_user((User *)user, AS_U64(argv[0]), argv[1], argv[2], AS_U64(argv[3]));

    if (DEBUG) {
        debug_user((User *)user);
    }

    return 0;
}

User *ReadUser(SommelierDBMS *dbms, uint64_t id) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT UserID, DataPublicKey, KeywordPublicKey, Nonce FROM "
            "user_table WHERE UserID = %ld;",
            id);

    User *user = initialize_user();
    // for error-handling, set `id = MAX` if no row get by execute SELECT.
    set_user_id(user, UINT64_MAX);
    int rc = orm_execute_sql(sommelier_connection(dbms), sql, callback_set_user,
                             (void *)user);

    if (rc != SQLITE_OK || user->id == UINT64_MAX) {
        finalize_user(user);
        user = NULL;
    }

    return user;
}

void IncrementUserNonce(SommelierDBMS *dbms, User *u) {
    uint64_t n = increment_user_nonce(u);
    char sql[MAX_SIZE_SQL_PLANE_TEXT] = "";
    sprintf(sql, "UPDATE user_table SET Nonce=%ld WHERE UserID=%ld;", n, u->id);

    orm_execute_sql(sommelier_connection(dbms), sql, 0, 0);
}

// Create Path API.

Path *CreatePath(SommelierDBMS *dbms, uint64_t uid, char *ph, char *ctd,
                 char *ctk) {
    char sql[MAX_SIZE_SQL_CREATE_PATH] = "";
    sprintf(sql,
            "INSERT INTO path_table (UserID, PermissionHash, DataCipherText, "
            "KeywordCipherText) values (%ld, '%s', '%s', '%s');",
            uid, ph, ctd, ctk);

    int rc = orm_execute_sql(sommelier_connection_with_insert(dbms), sql, 0, 0);

    if (rc == SQLITE_OK) {
        uint64_t id = __last_inserted_id(dbms->db);
        Path *p = initialize_path();
        set_path(p, id, uid, ph, ctd, ctk);

        if (DEBUG) {
            debug_path(p);
        }

        return p;
    } else {
        return NULL;
    }
}

static int callback_set_path(void *path, int argc, char **argv,
                             char **azColName) {
    set_path((Path *)path, AS_U64(argv[0]), AS_U64(argv[1]), argv[2], argv[3],
             argv[4]);

    if (DEBUG) {
        debug_path((Path *)path);
    }

    return 0;
}

Path *ReadPath(SommelierDBMS *dbms, uint64_t id) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT PathID, UserID, PermissionHash, DataCipherText, "
            "KeywordCipherText FROM path_table WHERE PathID = %ld;",
            id);

    Path *p = initialize_path();
    set_path_id(p, UINT64_MAX);
    int rc = orm_execute_sql(sommelier_connection(dbms), sql, callback_set_path,
                             (void *)p);

    if (rc != SQLITE_OK || p->id == UINT64_MAX) {
        finalize_path(p);
        p = NULL;
    }

    return p;
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
PathVector *SearchEncryptedPath(SommelierDBMS *dbms, uint64_t uid,
                                char *trapdoor) {
    char sql[MAX_SIZE_SQL_SEARCH_ENCRYPTED_PATH] = "";
    sprintf(sql,
            "SELECT PathID, UserID, PermissionHash, DataCipherText, "
            "KeywordCipherText FROM path_table WHERE "
            "test_cipher(KeywordCipherText, '%s') = 1;",
            trapdoor);

    PathVector *vec = initialize_path_vector();

    orm_execute_sql(sommelier_connection(dbms), sql, callback_path_row,
                    (void *)vec);

    return vec;
}

PathVector *FilterByPermissionHash(SommelierDBMS *dbms, char *ph) {
    char sql[MAX_SIZE_SQL_FILTER_BY_PREMISSION_HASH] = "";
    sprintf(sql,
            "SELECT PathID, UserID, PermissionHash, DataCipherText, "
            "KeywordCipherText FROM path_table WHERE PermissionHash = '%s';",
            ph);

    PathVector *vec = initialize_path_vector();

    orm_execute_sql(sommelier_connection(dbms), sql, callback_path_row,
                    (void *)vec);

    return vec;
}

SharedKey *CreateSharedKey(SommelierDBMS *dbms, uint64_t pid, char *ctsk) {
    char sql[MAX_SIZE_SQL_CREATE_SHARED_KEY] = "";
    sprintf(sql,
            "INSERT INTO shared_key_table (PathID, SharedKeyCipherText) values "
            "(%ld, '%s')",
            pid, ctsk);

    int rc = orm_execute_sql(sommelier_connection_with_insert(dbms), sql, 0, 0);

    if (rc == SQLITE_OK) {
        uint64_t id = __last_inserted_id(dbms->db);
        SharedKey *sk = initialize_shared_key();
        set_shared_key(sk, id, pid, ctsk);

        if (DEBUG) {
            debug_shared_key(sk);
        }

        return sk;
    } else {
        return NULL;
    }
}

static int callback_set_shared_key(void *sk, int argc, char **argv,
                                   char **azColName) {
    set_shared_key((SharedKey *)sk, AS_U64(argv[0]), AS_U64(argv[1]), argv[2]);

    if (DEBUG) {
        debug_shared_key((SharedKey *)sk);
    }

    return 0;
}

SharedKey *ReadSharedKey(SommelierDBMS *dbms, uint64_t pid) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT SharedKeyID, PathID, SharedKeyCipherText FROM "
            "shared_key_table WHERE PathID = %ld;",
            pid);

    SharedKey *sk = initialize_shared_key();
    set_shared_key_id(sk, UINT64_MAX);
    int rc = orm_execute_sql(sommelier_connection(dbms), sql,
                             callback_set_shared_key, (void *)sk);

    if (rc != SQLITE_OK || sk->id == UINT64_MAX) {
        finalize_shared_key(sk);
        sk = NULL;
    }

    return sk;
}

/*
// deprecate: WritePermission

AuthorizationSeed *CreateAuthorizationSeed(SommelierDBMS *dbms, uint64_t pid,
                                           char *ctas) {
    char sql[MAX_SIZE_SQL_CREATE_AUTHORIZATION_SEED] = "";
    sprintf(sql,
            "INSERT INTO authorization_seed_table (PathID, "
            "AuthorizationSeedCipherText) values (%ld, '%s');",
            pid, ctas);

    int rc = orm_execute_sql(sommelier_connection_with_insert(dbms), sql, 0, 0);

    if (rc == SQLITE_OK) {
        uint64_t id = __last_inserted_id(dbms->db);
        AuthorizationSeed *as = initialize_authorization_seed();
        set_authorization_seed(as, id, pid, ctas);

        if (DEBUG) {
            decode_json_authorization_seed(as);
        }

        return as;
    } else {
        return NULL;
    }
}

static int callback_set_authorization_seed(void *as, int argc, char **argv,
                                           char **azColName) {
    set_authorization_seed((AuthorizationSeed *)as, AS_U64(argv[0]),
                           AS_U64(argv[1]), argv[2]);

    if (DEBUG) {
        debug_authorization_seed((AuthorizationSeed *)as);
    }

    return 0;
}

AuthorizationSeed *ReadAuthorizationSeed(SommelierDBMS *dbms, uint64_t pid) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT AuthorizationSeedID, PathID, AuthorizationSeedCipherText "
            "FROM authorization_seed_table WHERE PathID = %ld;",
            pid);

    AuthorizationSeed *as = initialize_authorization_seed();
    set_authorization_seed_id(as, UINT64_MAX);
    int rc = orm_execute_sql(sommelier_connection(dbms), sql,
                             callback_set_authorization_seed, (void *)as);

    if (rc != SQLITE_OK || as->id == UINT64_MAX) {
        finalize_authorization_seed(as);
        as = NULL;
    }

    return as;
}

*/

// Content *CreateContent(SommelierDBMS *dbms, char *skh, char *pka, char *ctc)
// {
//     char sql[MAX_SIZE_SQL_CREATE_CONTENT] = "";
//     sprintf(sql,
//             "INSERT INTO content_table (SharedKeyHash,
//             AuthorizationPublicKey, " "Nonce, ContentCipherText) values
//             ('%s', '%s', 1, '%s');", skh, pka, ctc);

//     int rc = orm_execute_sql(sommelier_connection_with_insert(dbms), sql, 0,
//     0);

//     if (rc == SQLITE_OK) {
//         uint64_t id = __last_inserted_id(dbms->db);
//         Content *c = initialize_content();
//         set_content(c, id, skh, pka, 1, ctc);

//         if (DEBUG) {
//             decode_json_content(c);
//         }

//         return c;
//     } else {
//         return NULL;
//     }
// }

Content *CreateContent(SommelierDBMS *dbms, char *skh, char *ctc) {
    char sql[MAX_SIZE_SQL_CREATE_CONTENT] = "";
    sprintf(sql,
            "INSERT INTO content_table (SharedKeyHash, Nonce, "
            "ContentCipherText) values ('%s', 1, '%s');",
            skh, ctc);

    int rc = orm_execute_sql(sommelier_connection_with_insert(dbms), sql, 0, 0);

    if (rc == SQLITE_OK) {
        uint64_t id = __last_inserted_id(dbms->db);
        Content *c = initialize_content();
        set_content(c, id, skh, 1, ctc);

        if (DEBUG) {
            decode_json_content(c);
        }

        return c;
    } else {
        return NULL;
    }
}

static int callback_set_content(void *c, int argc, char **argv,
                                char **azColName) {
    // set_content((Content *)c, AS_U64(argv[0]), argv[1], argv[2],
    //             AS_U64(argv[3]), argv[4]);
    set_content((Content *)c, AS_U64(argv[0]), argv[1], AS_U64(argv[2]),
                argv[3]);

    if (DEBUG) {
        debug_content((Content *)c);
    }

    return 0;
}

Content *ReadContent(SommelierDBMS *dbms, uint64_t id) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT ContentID, SharedKeyHash, Nonce, "
            "ContentCipherText FROM content_table WHERE ContentID = %ld;",
            id);

    Content *c = initialize_content();
    set_content_id(c, UINT64_MAX);
    int rc = orm_execute_sql(sommelier_connection(dbms), sql,
                             callback_set_content, (void *)c);

    if (rc != SQLITE_OK || c->id == UINT64_MAX) {
        finalize_content(c);
        c = NULL;
    }

    return c;
}

Content *ReadContentBySharedKeyHash(SommelierDBMS *dbms, char *skh) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT ContentID, SharedKeyHash, Nonce, "
            "ContentCipherText FROM content_table WHERE SharedKeyHash = '%s';",
            skh);

    Content *c = initialize_content();
    set_content_id(c, UINT64_MAX);
    int rc = orm_execute_sql(sommelier_connection(dbms), sql,
                             callback_set_content, (void *)c);

    if (rc != SQLITE_OK || c->id == UINT64_MAX) {
        finalize_content(c);
        c = NULL;
    }

    return c;
}

void UpdateContent(SommelierDBMS *dbms, Content *c) {
    char sql[MAX_SIZE_SQL_PLANE_TEXT] = "";
    sprintf(sql,
            "UPDATE content_table SET Nonce=%ld, ContentCipherText='%s' WHERE "
            "ContentID=%ld;",
            c->nonce, c->content_cipher_text, c->id);

    orm_execute_sql(sommelier_connection(dbms), sql, 0, 0);
}

void IncrementContentNonce(SommelierDBMS *dbms, Content *c) {
    uint64_t n = increment_content_nonce(c);
    char sql[MAX_SIZE_SQL_PLANE_TEXT] = "";
    sprintf(sql, "UPDATE content_table SET Nonce=%ld WHERE ContentID=%ld;", n,
            c->id);

    orm_execute_sql(sommelier_connection(dbms), sql, 0, 0);
}

static int callback_content_row(void *vec, int argc, char **argv,
                                char **azColName) {
    // Content c = {AS_U64(argv[0]), argv[1], argv[2], AS_U64(argv[3]),
    // argv[4]};
    Content c = {AS_U64(argv[0]), argv[1], AS_U64(argv[2]), argv[3]};

    if (DEBUG) {
        debug_content(&c);
    }

    push_content_vector((ContentVector *)vec, &c);

    return 0;
}

ContentVector *FilterBySharedKeyHash(SommelierDBMS *dbms, char *skh) {
    char sql[MAX_SIZE_SQL_CREATE_WRITE_PERMISSION] = "";
    sprintf(sql,
            "SELECT ContentID, SharedKeyHash, Nonce,  "
            "ContentCipherText FROM content_table WHERE SharedKeyHash = '%s';",
            skh);

    ContentVector *vec = initialize_content_vector();

    orm_execute_sql(sommelier_connection(dbms), sql, callback_content_row,
                    (void *)vec);

    return vec;
}

/*
// deprecate: WritePermission

WritePermission *CreateWritePermission(SommelierDBMS *dbms, uint64_t pid,
                                       uint64_t uid) {
    char sql[MAX_SIZE_SQL_CREATE_WRITE_PERMISSION] = "";
    sprintf(sql,
            "INSERT INTO write_permission_table (PathID, UserID) "
            "values (%ld, %ld);",
            pid, uid);

    int rc = orm_execute_sql(sommelier_connection_with_insert(dbms), sql, 0, 0);

    if (rc == SQLITE_OK) {
        uint64_t id = __last_inserted_id(dbms->db);
        WritePermission *wp = initialize_write_permission();
        set_write_permission(wp, id, pid, uid);

        if (DEBUG) {
            decode_json_write_permission(wp);
        }

        return wp;
    } else {
        return NULL;
    }
}

static int callback_set_write_permission(void *wp, int argc, char **argv,
                                         char **azColName) {
    set_write_permission((WritePermission *)wp, AS_U64(argv[0]),
                         AS_U64(argv[1]), AS_U64(argv[2]));

    if (DEBUG) {
        debug_write_permission((WritePermission *)wp);
    }

    return 0;
}

WritePermission *ReadWritePermission(SommelierDBMS *dbms, uint64_t pid) {
    char sql[MAX_SIZE_SQL_READ_BY_ID] = "";
    sprintf(sql,
            "SELECT WritePermissionID, PathID, UserID FROM "
            "write_permission_table WHERE PathID = %ld;",
            pid);

    WritePermission *wp = initialize_write_permission();
    set_write_permission_id(wp, UINT64_MAX);
    int rc = orm_execute_sql(sommelier_connection(dbms), sql,
                             callback_set_write_permission, (void *)wp);

    if (rc != SQLITE_OK || wp->id == UINT64_MAX) {
        finalize_write_permission(wp);
        wp = NULL;
    }

    return wp;
}
*/
