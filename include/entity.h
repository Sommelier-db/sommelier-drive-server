#pragma once

#include <sqlite3.h>

typedef struct user_table_row {
    int id;
    char *data_public_key;
    char *keyword_public_key;
    int nonce;
} User;

typedef struct path_table_row {
    int id;
    int user_id;
    char *permission_hash;
    char *data_cipher_text;
    char *keyword_cipher_text;
} Path;

typedef struct shared_key_table_row {
    int id;
    int path_id;
    char *shared_key_cipher_text;
} SharedKey;

typedef struct content_table_row {
    int id;
    char *shared_key_hash;
    char *content_cipher_text;
} Content;

typedef struct write_permission_table_row {
    int id;
    int path_id;
    int user_id;
} WritePermission;
