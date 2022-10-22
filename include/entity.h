#pragma once

#include <jansson.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

#define MAX_SIZE_PKE_PK 1024
#define MAX_SIZE_PKSE_PK 1024
#define MAX_SIZE_SKE_KEY 1024
#define MAX_SIZE_HASH 1024
#define MAX_SIZE_PKE_CT 1024
#define MAX_SIZE_PKSE_CT 1024

typedef struct user_table_row {
    uint64_t id;
    char *data_public_key;
    char *keyword_public_key;
    uint64_t nonce;
} User;

User *initialize_user(uint64_t, const char *, const char *);
void finalize_user(User *);
void set_user(User *, const char *, const char *);
uint64_t increment_nonce(User *);
void decode_json_user(User *, json_t *);

typedef struct path_table_row {
    uint64_t id;
    uint64_t user_id;
    char *permission_hash;
    char *data_cipher_text;
    char *keyword_cipher_text;
} Path;

Path *initialize_path(uint64_t, uint64_t, const char *, const char *,
                      const char *);
void finalize_path(Path *);
void set_path(Path *, uint64_t, const char *, const char *, const char *);
void decode_json_path(Path *, json_t *);

typedef struct shared_key_table_row {
    uint64_t id;
    uint64_t path_id;
    char *shared_key_cipher_text;
} SharedKey;

SharedKey *initialize_shared_key(uint64_t, uint64_t, const char *);
void finalize_shared_key(SharedKey *);
void set_shared_key(SharedKey *, uint64_t, const char *);
void decode_json_shared_key(SharedKey *, json_t *);

typedef struct content_table_row {
    uint64_t id;
    char *shared_key_hash;
    char *content_cipher_text;
} Content;

Content *initialize_content(uint64_t, const char *, const char *);
void finalize_content(Content *);
void set_content(Content *, const char *, const char *);
void decode_json_content(Content *, json_t *);

typedef struct write_permission_table_row {
    uint64_t id;
    uint64_t path_id;
    uint64_t user_id;
} WritePermission;

WritePermission *initialize_write_permission(uint64_t, uint64_t, uint64_t);
void finalize_write_permission(WritePermission *);
void set_write_permission(WritePermission *, uint64_t, uint64_t);
void decode_json_write_permission(WritePermission *, json_t *);

typedef struct path_table_vector {
    size_t max_size;
    size_t length;
    Path *buf;
} PathVector;

PathVector *initialize_path_vector();
void finalize_path_vector(PathVector *);
size_t push_path_vector(PathVector *, Path *);
size_t resize_path_vector(PathVector *);

typedef struct content_table_vector {
    size_t max_size;
    size_t length;
    Content *buf;
} ContentVector;

ContentVector *initialize_content_vector();
void finalize_content_vector(ContentVector *);
size_t push_content_vector(ContentVector *, Content *);
size_t resize_content_vector(ContentVector *);

#if DEBUG
void debug_user(User *);
void debug_path(Path *);
void debug_shared_key(SharedKey *);
void debug_content(Content *);
void debug_write_permission(WritePermission *);
void debug_path_vector(PathVector *);
void debug_content_vector(ContentVector *);
#endif