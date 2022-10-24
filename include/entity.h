#pragma once

#include <jansson.h>
#include <stdint.h>

#include "util.h"

#define MAX_SIZE_PKE_KEY 720
#define MAX_SIZE_PKSE_KEY 47112
#define MAX_SIZE_HASH 32
#define MAX_SIZE_PKE_CT 512
#define MAX_SIZE_PKSE_KEYWORD_CT 2900000
#define MAX_SIZE_PKSE_CONTENT_CT 4194360

#define VECTOR_MAX_SIZE_DEFAULT 16
#define VECTOR_EXTEND_RATE 2

typedef struct user_table_row {
    uint64_t id;
    char *data_public_key;
    char *keyword_public_key;
    uint64_t nonce;
} User;

User *initialize_user();
void finalize_user(User *);
void set_user(User *, uint64_t, const char *, const char *, uint64_t);
void set_user_id(User *, uint64_t);
void set_user_data_public_key(User *, const char *);
void set_user_keyword_public_key(User *, const char *);
void set_user_nonce(User *, uint64_t);
uint64_t increment_nonce(User *);
json_t *decode_json_user(User *);

typedef struct path_table_row {
    uint64_t id;
    uint64_t user_id;
    char *permission_hash;
    char *data_cipher_text;
    char *keyword_cipher_text;
} Path;

Path *initialize_path();
void finalize_path(Path *);
void set_path(Path *, uint64_t, uint64_t, const char *, const char *,
              const char *);
void set_path_id(Path *, uint64_t);
void set_path_user_id(Path *, uint64_t);
void set_path_permission_hash(Path *, const char *);
void set_path_data_cipher_text(Path *, const char *);
void set_path_keyword_cipher_text(Path *, const char *);
json_t *decode_json_path(Path *);

typedef struct shared_key_table_row {
    uint64_t id;
    uint64_t path_id;
    char *shared_key_cipher_text;
} SharedKey;

SharedKey *initialize_shared_key();
void finalize_shared_key(SharedKey *);
void set_shared_key(SharedKey *, uint64_t, uint64_t, const char *);
void set_shared_key_id(SharedKey *, uint64_t);
void set_shared_key_path_id(SharedKey *, uint64_t);
void set_shared_key_share_key_cipher_text(SharedKey *, const char *);
json_t *decode_json_shared_key(SharedKey *);

typedef struct authorization_seed_table_row {
    uint64_t id;
    uint64_t path_id;
    char *authorization_seed_cipher_text;
} AuthorizationSeed;

AuthorizationSeed *initialize_authorization_seed();
void finalize_authorization_seed(AuthorizationSeed *);
void set_authorization_seed(AuthorizationSeed *, uint64_t, uint64_t,
                            const char *);
void set_authorization_seed_id(AuthorizationSeed *, uint64_t);
void set_authorization_seed_path_id(AuthorizationSeed *, uint64_t);
void set_authorization_seed_authorization_seed_cipher_text(AuthorizationSeed *,
                                                           const char *);
json_t *decode_json_authorization_seed(AuthorizationSeed *);

typedef struct content_table_row {
    uint64_t id;
    char *shared_key_hash;
    char *content_cipher_text;
} Content;

Content *initialize_content();
void finalize_content(Content *);
void set_content(Content *, uint64_t, const char *, const char *);
void set_content_id(Content *, uint64_t);
void set_content_shared_key_hash(Content *, const char *);
void set_content_content_cipher_text(Content *, const char *);
json_t *decode_json_content(Content *);

typedef struct write_permission_table_row {
    uint64_t id;
    uint64_t path_id;
    uint64_t user_id;
} WritePermission;

WritePermission *initialize_write_permission();
void finalize_write_permission(WritePermission *);
void set_write_permission(WritePermission *, uint64_t, uint64_t, uint64_t);
void set_write_permission_id(WritePermission *, uint64_t);
void set_write_permission_path_id(WritePermission *, uint64_t);
void set_write_permission_user_id(WritePermission *, uint64_t);
json_t *decode_json_write_permission(WritePermission *);

typedef struct path_table_vector {
    size_t max_size;
    size_t length;
    Path **buf;
} PathVector;

PathVector *initialize_path_vector();
void finalize_path_vector(PathVector *);
size_t push_path_vector(PathVector *, Path *);
size_t resize_path_vector(PathVector *);
json_t *decode_json_path_vector(PathVector *);

typedef struct content_table_vector {
    size_t max_size;
    size_t length;
    Content **buf;
} ContentVector;

ContentVector *initialize_content_vector();
void finalize_content_vector(ContentVector *);
size_t push_content_vector(ContentVector *, Content *);
size_t resize_content_vector(ContentVector *);
json_t *decode_json_content_vector(ContentVector *);

#if DEBUG
void debug_user(User *);
void debug_path(Path *);
void debug_shared_key(SharedKey *);
void debug_content(Content *);
void debug_write_permission(WritePermission *);
void debug_path_vector(PathVector *);
void debug_content_vector(ContentVector *);
#endif
