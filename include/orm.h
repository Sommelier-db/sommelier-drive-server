#pragma once

#include <sqlite3.h>

#include "entity.h"

#define SOMMELIER_DRIVE_N_TABLES 5
#define SOMMELIER_DRIVE_INITIALIZE_SQL 7
#define MAX_SIZE_SQL_PLANE_TEXT 500

#define MAX_SIZE_SQL_CREATE_USER \
    MAX_SIZE_SQL_PLANE_TEXT + MAX_SIZE_PKE_KEY + MAX_SIZE_PKSE_KEY
#define MAX_SIZE_SQL_CREATE_PATH                                \
    MAX_SIZE_SQL_PLANE_TEXT + MAX_SIZE_HASH + MAX_SIZE_PKE_CT + \
        MAX_SIZE_PKSE_KEYWORD_CT
#define MAX_SIZE_SQL_CREATE_SHARED_KEY MAX_SIZE_SQL_PLANE_TEXT + MAX_SIZE_PKE_CT
#define MAX_SIZE_SQL_CREATE_AUTHORIZATION_SEED \
    MAX_SIZE_SQL_PLANE_TEXT + MAX_SIZE_PKE_CT
#define MAX_SIZE_SQL_CREATE_CONTENT                              \
    MAX_SIZE_SQL_PLANE_TEXT + MAX_SIZE_HASH + MAX_SIZE_PKE_KEY + \
        MAX_SIZE_PKSE_CONTENT_CT
#define MAX_SIZE_SQL_CREATE_WRITE_PERMISSION MAX_SIZE_SQL_PLANE_TEXT

#define MAX_SIZE_SQL_READ_BY_ID MAX_SIZE_SQL_PLANE_TEXT
#define MAX_SIZE_SQL_FILTER_BY_PREMISSION_HASH \
    MAX_SIZE_SQL_PLANE_TEXT + MAX_SIZE_HASH

void InitalizeDatabase(sqlite3 *);

User *CreateUser(sqlite3 *, char *, char *);

User *ReadUser(sqlite3 *, uint64_t);

void IncrementUserNonce(sqlite3 *, User *);

Path *CreatePath(sqlite3 *, uint64_t, char *, char *, char *);

Path *ReadPath(sqlite3 *, uint64_t);

PathVector *FilterByPermissionHash(sqlite3 *, char *);

// depends on Sommelier-DB
PathVector *SearchEncryptedPath(sqlite3 *, uint64_t, char *);

SharedKey *CreateSharedKey(sqlite3 *, uint64_t, char *);

SharedKey *ReadSharedKey(sqlite3 *, uint64_t);

AuthorizationSeed *CreateAuthorizationSeed(sqlite3 *, uint64_t, char *);

AuthorizationSeed *ReadAuthorizationSeed(sqlite3 *, uint64_t);

Content *CreateContent(sqlite3 *, char *, char *, char *);

Content *ReadContent(sqlite3 *, uint64_t);

void IncrementContentNonce(sqlite3 *, Content *);

ContentVector *FilterBySharedKeyHash(sqlite3 *, char *);

WritePermission *CreateWritePermission(sqlite3 *, uint64_t, uint64_t);

WritePermission *ReadWritePermission(sqlite3 *, uint64_t);
