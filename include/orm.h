#pragma once

#include "dbms.h"
#include "entity.h"

#define SOMMELIER_DRIVE_N_TABLES 5
#define SOMMELIER_DRIVE_INITIALIZE_SQL 7
#define MAX_SIZE_SQL_PLANE_TEXT 500
#define MAX_SIZE_TRAPDOOR 23001

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
#define MAX_SIZE_SQL_SEARCH_ENCRYPTED_PATH \
    MAX_SIZE_SQL_PLANE_TEXT + MAX_SIZE_TRAPDOOR

void InitalizeDatabase(SommelierDBMS *);

User *CreateUser(SommelierDBMS *, char *, char *);

User *ReadUser(SommelierDBMS *, uint64_t);

void IncrementUserNonce(SommelierDBMS *, User *);

Path *CreatePath(SommelierDBMS *, uint64_t, char *, char *, char *);

Path *ReadPath(SommelierDBMS *, uint64_t);

PathVector *FilterByPermissionHash(SommelierDBMS *, char *);

// depends on Sommelier-DB
PathVector *SearchEncryptedPath(SommelierDBMS *, uint64_t, char *);

SharedKey *CreateSharedKey(SommelierDBMS *, uint64_t, char *);

SharedKey *ReadSharedKey(SommelierDBMS *, uint64_t);

/*
// deprecate: AuthorizationSeed

AuthorizationSeed *CreateAuthorizationSeed(SommelierDBMS *, uint64_t, char *);

AuthorizationSeed *ReadAuthorizationSeed(SommelierDBMS *, uint64_t);
*/

// Content *CreateContent(SommelierDBMS *, char *, char *, char *);
Content *CreateContent(SommelierDBMS *, char *, char *);

Content *ReadContent(SommelierDBMS *, uint64_t);

Content *ReadContentBySharedKeyHash(SommelierDBMS *, char *);

void UpdateContent(SommelierDBMS *, Content *);

void IncrementContentNonce(SommelierDBMS *, Content *);

ContentVector *FilterBySharedKeyHash(SommelierDBMS *, char *);

/*
// deprecate: WritePermission

WritePermission *CreateWritePermission(SommelierDBMS *, uint64_t, uint64_t);

WritePermission *ReadWritePermission(SommelierDBMS *, uint64_t);
*/