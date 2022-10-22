#pragma once

#include <jansson.h>
#include <sqlite3.h>
#include <stdlib.h>

#include "util.h"

typedef struct user_table_row {
    int id;
    char *data_public_key;
    char *keyword_public_key;
    int nonce;
} User;

User *initialize_user(const char *, const char *);
void finalize_user(User *);
void set_user(User *, const char *, const char *);
void increment_nonce(User *);
size_t decode_json_user(User *, json_t *);

typedef struct path_table_row {
    int id;
    int user_id;
    char *permission_hash;
    char *data_cipher_text;
    char *keyword_cipher_text;
} Path;

Path *initialize_path(int, const char *, const char *, const char *);
void finalize_path(Path *);
void set_path(Path *, int, const char *, const char *, const char *);
size_t decode_json_path(Path *, json_t *);

typedef struct shared_key_table_row {
    int id;
    int path_id;
    char *shared_key_cipher_text;
} SharedKey;

SharedKey *initialize_shared_key(int, const char *);
void finalize_shared_key(SharedKey *);
void set_shared_key(SharedKey *, int, const char *);
size_t decode_json_shared_key(SharedKey *, json_t *);

typedef struct content_table_row {
    int id;
    char *shared_key_hash;
    char *content_cipher_text;
} Content;

Content *initialize_content(const char *, const char *);
void finalize_content(Content *);
void set_content(Content *, const char *, const char *);
size_t decode_json_content(Content *, json_t *);

typedef struct write_permission_table_row {
    int id;
    int path_id;
    int user_id;
} WritePermission;

WritePermission *initialize_write_permission(int, int);
void finalize_write_permission(WritePermission *);
void set_write_permission(WritePermission *, int, int);
size_t decode_json_write_permission(WritePermission *, json_t *);

typedef struct path_table_vector {
    size_t max_size;
    size_t length;
    Path *vec;
} PathVector;

PathVector *initialize_path_vector();
void finalize_path_vector(PathVector *);
int push_path_vector(PathVector *, Path *);
int resize_path_vector(PathVector *);

typedef struct content_table_vector {
    size_t max_size;
    size_t length;
    Content *vec;
} ContentVector;

#if DEBUG
void debug_user(User *);
void debug_path(Path *);
void debug_shared_key(SharedKey *);
void debug_content(Content *);
void debug_write_permission(WritePermission *);
void debug_path_vector(PathVector *);
void debug_content_vector(ContentVector *);
#endif