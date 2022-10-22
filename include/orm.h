#pragma once

#include <sqlite3.h>

#include "entity.h"

void InitalizeDatabase(sqlite3 *);

User *CreateUser(sqlite3 *, char *, char *);

User *ReadUser(sqlite3 *, int);

void IncrementUserNonce(sqlite3 *, User *);

Path *CreatePath(sqlite3 *, int, char *, char *, char *);

Path *ReadPath(sqlite3 *, int);

PathVector *FilterByPermissionHash(sqlite3 *, char *);

// depends on Sommelier-DB
PathVector *SearchEncryptedPath(sqlite3 *, int, char *);

SharedKey *CreateSharedKey(sqlite3 *, int, char *);

SharedKey *ReadSharedKey(sqlite3 *, int);

Content *CreateContent(sqlite3 *, char *, char *);

Content *ReadContent(sqlite3 *, int);

ContentVector *FilterBySharedKeyHash(sqlite3 *, char *);

WritePermission *CreateWritePermission(sqlite3 *, int, int);

WritePermission *ReadWritePermission(sqlite3 *, int);
