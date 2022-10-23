#include "orm.h"

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

    int rc;
    char *zErrMsg = 0;

    for (int i = 0; i < SOMMELIER_DRIVE_INITIALIZE_SQL; i++) {
        if (DEBUG) {
            printf("    sql - %s\n", sql[i]);
        }

        rc = sqlite3_exec(db, sql[i], 0, 0, &zErrMsg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
    }
}

User *CreateUser(sqlite3 *, char *, char *);

User *ReadUser(sqlite3 *, int);

void IncrementUserNonce(sqlite3 *, User *);

SharedKey *CreateSharedKey(sqlite3 *, int, char *);

SharedKey *ReadSharedKey(sqlite3 *, int);

Content *CreateContent(sqlite3 *, char *, char *);

Content *ReadContent(sqlite3 *, int);

ContentVector *FilterBySharedKeyHash(sqlite3 *, char *);

WritePermission *CreateWritePermission(sqlite3 *, int, int);

WritePermission *ReadWritePermission(sqlite3 *, int);