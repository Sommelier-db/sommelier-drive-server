#pragma once

#include <sqlite3.h>

#include "entity.h"

#define SOMMELIER_DRIVE_N_TABLES 5
#define SOMMELIER_DRIVE_INITIALIZE_SQL 6
#define MAX_SIZE_SQL_PLANE_TEXT 1024

void InitalizeDatabase(sqlite3 *);

User *CreateUser(sqlite3 *, char *, char *);

User *ReadUser(sqlite3 *, int);

void IncrementUserNonce(sqlite3 *, User *);

// Path API cannot be used.
Path *CreatePath(sqlite3 *, int, char *, char *, char *);

// Path API cannot be used.
Path *ReadPath(sqlite3 *, int);

// Path API cannot be used.
PathVector *FilterByPermissionHash(sqlite3 *, char *);

// Path API cannot be used.
// depends on Sommelier-DB
PathVector *SearchEncryptedPath(sqlite3 *, int, char *);

SharedKey *CreateSharedKey(sqlite3 *, int, char *);

SharedKey *ReadSharedKey(sqlite3 *, int);

Content *CreateContent(sqlite3 *, char *, char *);

Content *ReadContent(sqlite3 *, int);

ContentVector *FilterBySharedKeyHash(sqlite3 *, char *);

WritePermission *CreateWritePermission(sqlite3 *, int, int);

WritePermission *ReadWritePermission(sqlite3 *, int);
