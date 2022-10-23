#include "entity.h"

// User API

User *initialize_user(uint64_t id, const char *pkd, const char *pkk) {
    User *user = INITIALIZE(User);

    if (user == NULL) {
        errordebug("Memory allocation is failed. - User");
        exit(1);
    }

    user->id = id;

    // copy DataPublicKey
    user->data_public_key = INITIALIZE_STRING(MAX_SIZE_PKE_PK);

    if (user->data_public_key == NULL) {
        errordebug("Memory allocation is failed. - User::data_public_key");
        exit(1);
    }

    strcpy(user->data_public_key, pkd);

    // copy KeywordPublicKey
    user->keyword_public_key = INITIALIZE_STRING(MAX_SIZE_PKSE_PK);

    if (user->keyword_public_key == NULL) {
        errordebug("Memory allocation is failed. - User::keyword_public_key");
        exit(1);
    }

    strcpy(user->keyword_public_key, pkk);

    return user;
}

void finalize_user(User *user) {
    free(user->data_public_key);
    free(user->keyword_public_key);
    free(user);
}

void set_user(User *user, const char *pkd, const char *pkk) {
    if (pkd != NULL) {
        strcpy(user->data_public_key, pkd);
    }
    if (pkk != NULL) {
        strcpy(user->keyword_public_key, pkk);
    }
}

uint64_t increment_nonce(User *user) { return ++user->nonce; }

json_t *decode_json_user(User *user) {
    json_t *json = json_object();

    if (json_object_set(json, "id", json_integer(user->id)) < 0) {
        errordebug("Setting JSON is failed. - User::id");
        exit(1);
    }

    if (json_object_set(json, "data_public_key",
                        json_string(user->data_public_key)) < 0) {
        errordebug("Setting JSON is failed. - User::data_public_key");
        exit(1);
    }

    if (json_object_set(json, "keyword_public_key",
                        json_string(user->keyword_public_key)) < 0) {
        errordebug("Setting JSON is failed. - User::keyword_public_key");
        exit(1);
    }

    return json;
}

// Path API

Path *initialize_path(uint64_t id, uint64_t user_id, const char *ph,
                      const char *ctd, const char *ctk) {
    Path *path = INITIALIZE(Path);

    _initialize_path(path, id, user_id, ph, ctd, ctk);

    return path;
}

void _initialize_path(Path *path, uint64_t id, uint64_t user_id, const char *ph,
                      const char *ctd, const char *ctk) {
    if (path == NULL) {
        errordebug("Memory allocation is failed. - Path");
        exit(1);
    }

    path->id = id;
    path->user_id = user_id;

    // copy PermissionHash
    path->permission_hash = INITIALIZE_STRING(MAX_SIZE_HASH);

    if (path->permission_hash == NULL) {
        errordebug("Memory allocation is failed. - Path::permission_hash");
        exit(1);
    }

    strncpy(path->permission_hash, ph, strlen(ph) + 1);

    // copy DataCipherText
    // path->data_cipher_text = INITIALIZE_STRING(MAX_SIZE_PKSE_CT);
    path->data_cipher_text = INITIALIZE_STRING(sizeof(ctd));

    if (path->data_cipher_text == NULL) {
        errordebug("Memory allocation is failed. - Path::data_cipher_text");
        exit(1);
    }

    strncpy(path->data_cipher_text, ctd, strlen(ctd) + 1);

    // copy KeywordCipherText
    // path->keyword_cipher_text = INITIALIZE_STRING(MAX_SIZE_PKSE_CT);
    path->keyword_cipher_text = INITIALIZE_STRING(sizeof(ctk));

    if (path->keyword_cipher_text == NULL) {
        errordebug("Memory allocation is failed. - Path::keyword_cipher_text");
        exit(1);
    }

    strncpy(path->keyword_cipher_text, ctk, strlen(ctk) + 1);
}

void finalize_path(Path *path) {
    free(path->permission_hash);
    free(path->data_cipher_text);
    free(path->keyword_cipher_text);
    free(path);
}

void set_path(Path *path, uint64_t user_id, const char *ph, const char *ctd,
              const char *ctk) {
    if (user_id != path->user_id) {
        path->user_id = user_id;
    }
    if (ph != NULL) {
        strcpy(path->permission_hash, ph);
    }
    if (ctd != NULL) {
        strcpy(path->data_cipher_text, ctd);
    }
    if (ctk != NULL) {
        strcpy(path->keyword_cipher_text, ctk);
    }
}

json_t *decode_json_path(Path *path) {
    json_t *json = json_object();

    if (json_object_set(json, "id", json_integer(path->id)) < 0) {
        errordebug("Setting JSON is failed. - Path::id");
        exit(1);
    }

    if (json_object_set(json, "user_id", json_integer(path->user_id)) < 0) {
        errordebug("Setting JSON is failed. - Path::user_id");
        exit(1);
    }

    if (json_object_set(json, "permission_hash",
                        json_string(path->permission_hash)) < 0) {
        errordebug("Setting JSON is failed. - Path::permission_hash");
        exit(1);
    }

    if (json_object_set(json, "data_cipher_text",
                        json_string(path->data_cipher_text)) < 0) {
        errordebug("Setting JSON is failed. - Path::data_cipher_text");
        exit(1);
    }

    if (json_object_set(json, "keyword_cipher_text",
                        json_string(path->keyword_cipher_text)) < 0) {
        errordebug("Setting JSON is failed. - Path::keyword_cipher_text");
        exit(1);
    }

    return json;
}

// PathVector API

PathVector *initialize_path_vector() {
    PathVector *vec = INITIALIZE(PathVector);

    if (vec == NULL) {
        errordebug("Memory allocation is failed. - PathVector");
        exit(1);
    }

    vec->max_size = VECTOR_MAX_SIZE_DEFAULT;
    vec->length = 0;
    vec->buf = INITIALIZE_SIZE(Path *, vec->max_size);

    if (vec->buf == NULL) {
        errordebug("Memory allocation is failed. - PathVector::buf");
        exit(1);
    }

    return vec;
}

void finalize_path_vector(PathVector *vec) {
    free(vec->buf);
    free(vec);
}

size_t push_path_vector(PathVector *vec, Path *path) {
    if (vec->length == vec->max_size) {
        if (DEBUG) {
            printf(
                "[DEBUG] resize_path_vector() called. - "
                "push_path_vector()\n");
        }

        resize_path_vector(vec);
    }

    vec->buf[vec->length] = INITIALIZE(Path);

    _initialize_path(vec->buf[vec->length], path->id, path->user_id,
                     path->permission_hash, path->data_cipher_text,
                     path->keyword_cipher_text);

    return vec->length;
}

size_t resize_path_vector(PathVector *vec) {
    size_t new_size = VECTOR_EXTEND_RATE * vec->max_size;
    vec->max_size = new_size;
    vec->buf = (Path **)realloc(vec->buf, new_size * refsizeof(Path));

    if (vec->buf == NULL) {
        errordebug("Memory re-allocation is failed. - PathVector::buf");
        exit(1);
    }

    return new_size;
}

json_t *decode_json_path_vector(PathVector *vec) {
    json_t *array = json_array();

    for (int i = 0; i < vec->length; i++) {
        Path *p = vec->buf[i];
        json_t *j = decode_json_path(p);

        char *dumped = json_dumps(j, 0);
        printf("debug - %s\n", dumped);
        free(dumped);

        json_array_append(array, j);
    }

    return array;
}

// SharedKey API

SharedKey *initialize_shared_key(uint64_t id, uint64_t path_id,
                                 const char *ctsk) {
    SharedKey *shared_key = INITIALIZE(SharedKey);

    if (shared_key == NULL) {
        errordebug("Memory allocation is failed. - SharedKey");
        exit(1);
    }

    shared_key->id = id;
    shared_key->path_id = path_id;

    // copy SharedKeyCipherText
    shared_key->shared_key_cipher_text = INITIALIZE_STRING(MAX_SIZE_PKE_CT);

    if (shared_key->shared_key_cipher_text == NULL) {
        errordebug(
            "Memory allocation is failed. - "
            "SharedKey::shared_key_cipher_text");
        exit(1);
    }

    strcpy(shared_key->shared_key_cipher_text, ctsk);

    return shared_key;
}

void finalize_shared_key(SharedKey *key) {
    free(key->shared_key_cipher_text);
    free(key);
}

void set_shared_key(SharedKey *key, uint64_t path_id, const char *ctsk) {
    if (path_id != key->path_id) {
        key->path_id = path_id;
    }
    if (ctsk != NULL) {
        strcpy(key->shared_key_cipher_text, ctsk);
    }
}

json_t *decode_json_shared_key(SharedKey *sk) {
    json_t *json = json_object();

    if (json_object_set(json, "id", json_integer(sk->id)) < 0) {
        errordebug("Setting JSON is failed. - SharedKey::id");
        exit(1);
    }

    if (json_object_set(json, "path_id", json_integer(sk->path_id)) < 0) {
        errordebug("Setting JSON is failed. - SharedKey::path_id");
        exit(1);
    }

    if (json_object_set(json, "shared_key_cipher_text",
                        json_string(sk->shared_key_cipher_text)) < 0) {
        errordebug(
            "Setting JSON is failed. - SharedKey::shared_key_cipher_text");
        exit(1);
    }

    return json;
}

// Content API

Content *initialize_content(uint64_t id, const char *skh, const char *ctc) {
    Content *content = INITIALIZE(Content);

    _initialize_content(content, id, skh, ctc);

    return content;
}

void _initialize_content(Content *content, uint64_t id, const char *skh,
                         const char *ctc) {
    if (content == NULL) {
        errordebug("Memory allocation is failed. - Content");
        exit(1);
    }

    content->id = id;

    // copy SharedKeyHash
    content->shared_key_hash = INITIALIZE_STRING(sizeof(skh));

    if (content->shared_key_hash == NULL) {
        errordebug("Memory allocation is failed. - Content::shared_key_hash");
        exit(1);
    }

    strncpy(content->shared_key_hash, skh, strlen(skh) + 1);

    // copy ContentCipherText
    content->content_cipher_text = INITIALIZE_STRING(sizeof(ctc));

    if (content->content_cipher_text == NULL) {
        errordebug(
            "Memory allocation is failed. - Content::content_cipher_text");
        exit(1);
    }

    strncpy(content->content_cipher_text, ctc, strlen(ctc) + 1);
}

void finalize_content(Content *content) {
    free(content->shared_key_hash);
    free(content->content_cipher_text);
    free(content);
}

void set_content(Content *content, const char *skh, const char *ctc) {
    if (skh != NULL) {
        strcpy(content->shared_key_hash, skh);
    }
    if (ctc != NULL) {
        strcpy(content->content_cipher_text, ctc);
    }
}

json_t *decode_json_content(Content *c) {
    json_t *json = json_object();

    if (json_object_set(json, "id", json_integer(c->id)) < 0) {
        errordebug("Setting JSON is failed. - Content::id");
        exit(1);
    }

    if (json_object_set(json, "shared_key_hash",
                        json_string(c->shared_key_hash)) < 0) {
        errordebug("Setting JSON is failed. - Content::shared_key_hash");
        exit(1);
    }

    if (json_object_set(json, "content_cipher_text",
                        json_string(c->content_cipher_text)) < 0) {
        errordebug("Setting JSON is failed. - Content::content_cipher_text");
        exit(1);
    }

    return json;
}

// ContetnVector API

ContentVector *initialize_content_vector() {
    ContentVector *vec = INITIALIZE(ContentVector);

    if (vec == NULL) {
        errordebug("Memory allocation is failed. - ContentVector");
        exit(1);
    }

    vec->max_size = VECTOR_MAX_SIZE_DEFAULT;
    vec->length = 0;
    vec->buf = INITIALIZE_SIZE(Content *, vec->max_size);

    if (vec->buf == NULL) {
        errordebug("Memory allocation is failed. - ContentVector::buf");
        exit(1);
    }

    return vec;
}

void finalize_content_vector(ContentVector *vec) {
    free(vec->buf);
    free(vec);
}

size_t push_content_vector(ContentVector *vec, Content *content) {
    if (vec->length == vec->max_size) {
        if (DEBUG) {
            printf(
                "[DEBUG] resize_content_vector() called. - "
                "push_content_vector()\n");
        }

        resize_content_vector(vec);
    }

    vec->buf[vec->length] = INITIALIZE(Content);

    _initialize_content(vec->buf[vec->length], content->id,
                        content->shared_key_hash, content->content_cipher_text);

    return ++vec->length;
}

size_t resize_content_vector(ContentVector *vec) {
    size_t new_size = VECTOR_EXTEND_RATE * vec->max_size;
    vec->max_size = new_size;
    vec->buf = (Content **)realloc(vec->buf, new_size * refsizeof(Content));

    if (vec->buf == NULL) {
        errordebug("Memory re-allocation is failed. - ContentVector::buf");
        exit(1);
    }

    return new_size;
}

json_t *decode_json_content_vector(ContentVector *vec) {
    json_t *array = json_array();

    for (int i = 0; i < vec->length; i++) {
        Content *c = vec->buf[i];
        json_t *j = decode_json_content(c);
        json_array_append(array, j);
    }

    return array;
}

// WritePermission API

WritePermission *initialize_write_permission(uint64_t id, uint64_t path_id,
                                             uint64_t user_id) {
    WritePermission *wp = INITIALIZE(WritePermission);

    if (wp == NULL) {
        errordebug("Memory allocation is failed. - WritePermission");
        exit(1);
    }

    wp->id = id;
    wp->path_id = path_id;
    wp->user_id = user_id;

    return wp;
}

void finalize_write_permission(WritePermission *wp) { free(wp); }

void set_write_permission(WritePermission *wp, uint64_t path_id,
                          uint64_t user_id) {
    if (path_id != wp->path_id) {
        wp->path_id = path_id;
    }
    if (user_id != wp->user_id) {
        wp->user_id = user_id;
    }
}

json_t *decode_json_write_permission(WritePermission *wp) {
    json_t *json = json_object();

    if (json_object_set(json, "id", json_integer(wp->id)) < 0) {
        errordebug("Setting JSON is failed. - WritePermission::id");
        exit(1);
    }

    if (json_object_set(json, "path_id", json_integer(wp->path_id)) < 0) {
        errordebug("Setting JSON is failed. - WritePermission::path_id");
        exit(1);
    }

    if (json_object_set(json, "user_id", json_integer(wp->user_id)) < 0) {
        errordebug("Setting JSON is failed. - WritePermission::user_id");
        exit(1);
    }

    return json;
}

// debug API

#if DEBUG

void debug_user(User *);

void debug_path(Path *);

void debug_shared_key(SharedKey *);

void debug_content(Content *);

void debug_write_permission(WritePermission *);

void debug_path_vector(PathVector *);

void debug_content_vector(ContentVector *);

#endif
