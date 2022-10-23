#include "entity.h"

// User API

User *initialize_user() {
    User *u = INITIALIZE(User);

    if (u == NULL) {
        errordebug("Memory allocation is failed. - User");
        exit(1);
    }

    u->data_public_key = initialize_string("User::data_public_key");
    u->keyword_public_key = initialize_string("User::keyword_public_key");

    return u;
}

void finalize_user(User *user) {
    // FIXME: 適切なfinalizeの実装

    // debug_string_pointer("free user->data_public_key",
    //     user->data_public_key);
    // free(user->data_public_key);
    // debug_string_pointer("free user->keyword_public_key",
    //     user->keyword_public_key);
    // free(user->keyword_public_key);
    // debug_void_pointer("free user", (void *)user);
    free(user);
}

void set_user(User *u, uint64_t id, const char *pkd, const char *pkk,
              uint64_t nonce) {
    set_user_id(u, id);
    set_user_data_public_key(u, pkd);
    set_user_keyword_public_key(u, pkk);
    set_user_nonce(u, nonce);
}

void set_user_id(User *u, uint64_t id) { u->id = id; }

void set_user_data_public_key(User *u, const char *pkd) {
    if (pkd != NULL) {
        u->data_public_key = safe_string_copy(u->data_public_key, pkd);
    } else if (DEBUG) {
        echodebug("Arg pkd is NULL. - set_user_data_public_key");
    }
}

void set_user_keyword_public_key(User *u, const char *pkk) {
    if (pkk != NULL) {
        u->keyword_public_key = safe_string_copy(u->keyword_public_key, pkk);
    } else if (DEBUG) {
        echodebug("Arg pkd is NULL. - set_user_keyword_public_key");
    }
}

void set_user_nonce(User *u, uint64_t nonce) { u->nonce = nonce; }

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

    if (json_object_set(json, "nonce", json_integer(user->nonce)) < 0) {
        errordebug("Setting JSON is failed. - User::nonce");
        exit(1);
    }

    return json;
}

// Path API

Path *initialize_path() {
    Path *p = INITIALIZE(Path);

    if (p == NULL) {
        errordebug("Memory allocation is failed. - Path");
        exit(1);
    }

    p->permission_hash = initialize_string("Path::permission_hash");
    p->data_cipher_text = initialize_string("Path::data_cipher_text");
    p->keyword_cipher_text = initialize_string("Path::keyword_cipher_text");

    return p;
}

void finalize_path(Path *path) {
    free(path->permission_hash);
    free(path->data_cipher_text);
    free(path->keyword_cipher_text);
    free(path);
}

void set_path(Path *p, uint64_t id, uint64_t user_id, const char *ph,
              const char *ctd, const char *ctk) {
    set_path_id(p, id);
    set_path_user_id(p, user_id);
    set_path_permission_hash(p, ph);
    set_path_data_cipher_text(p, ctd);
    set_path_keyword_cipher_text(p, ctk);
}

void set_path_id(Path *p, uint64_t id) { p->id = id; }

void set_path_user_id(Path *p, uint64_t user_id) { p->user_id = user_id; }

void set_path_permission_hash(Path *p, const char *ph) {
    if (VERBOSE) {
        debug_string_pointer("set_path_permission_hash: p->ph",
                             p->permission_hash);
        debug_string_pointer("set_path_permission_hash: ph", ph);

        char *dst = __STRING_COPY(p->permission_hash, ph);

        debug_string_pointer("set_path_permission_hash2: dst", dst);
        debug_string_pointer("set_path_permission_hash2: p->ph",
                             p->permission_hash);
        debug_string_pointer("set_path_permission_hash2: ph", ph);
    }

    if (ph != NULL) {
        p->permission_hash = safe_string_copy(p->permission_hash, ph);
    } else if (DEBUG) {
        echodebug("Arg pkd is NULL. - set_path_permission_hash");
    }
}

void set_path_data_cipher_text(Path *p, const char *ctd) {
    if (ctd != NULL) {
        p->data_cipher_text = safe_string_copy(p->data_cipher_text, ctd);
    } else if (DEBUG) {
        echodebug("Arg pkd is NULL. - set_path_data_cipher_text");
    }
}

void set_path_keyword_cipher_text(Path *p, const char *ctk) {
    if (ctk != NULL) {
        p->keyword_cipher_text = safe_string_copy(p->keyword_cipher_text, ctk);
    } else if (DEBUG) {
        echodebug("Arg pkd is NULL. - set_path_keyword_cipher_text");
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

    vec->buf[vec->length] = initialize_path();

    set_path(vec->buf[vec->length], path->id, path->user_id,
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
    shared_key->shared_key_cipher_text = INITIALIZE_STRING(sizeof(ctsk));

    if (shared_key->shared_key_cipher_text == NULL) {
        errordebug(
            "Memory allocation is failed. - "
            "SharedKey::shared_key_cipher_text");
        exit(1);
    }

    shared_key->shared_key_cipher_text =
        safe_string_copy(shared_key->shared_key_cipher_text, ctsk);

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
        key->shared_key_cipher_text =
            safe_string_copy(key->shared_key_cipher_text, ctsk);
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

    content->shared_key_hash = safe_string_copy(content->shared_key_hash, skh);

    // copy ContentCipherText
    content->content_cipher_text = INITIALIZE_STRING(sizeof(ctc));

    if (content->content_cipher_text == NULL) {
        errordebug(
            "Memory allocation is failed. - Content::content_cipher_text");
        exit(1);
    }

    content->content_cipher_text =
        safe_string_copy(content->content_cipher_text, ctc);
}

void finalize_content(Content *content) {
    free(content->shared_key_hash);
    free(content->content_cipher_text);
    free(content);
}

void set_content(Content *content, const char *skh, const char *ctc) {
    if (skh != NULL) {
        content->shared_key_hash =
            safe_string_copy(content->shared_key_hash, skh);
    }
    if (ctc != NULL) {
        content->content_cipher_text =
            safe_string_copy(content->content_cipher_text, ctc);
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

void debug_user(User *u) {
    fprintf(stdout, "<User id: %ld, pkd: %s, pkk: %s, nonce: %ld>\n", u->id,
            u->data_public_key, u->keyword_public_key, u->nonce);
    fflush(stdout);
}

void debug_path(Path *p) {
    fprintf(stdout, "<Path id: %ld, uid: %ld, ph: %s, ctd: %s, ctk: %s>\n",
            p->id, p->user_id, p->permission_hash, p->data_cipher_text,
            p->keyword_cipher_text);
    fflush(stdout);
}

void debug_shared_key(SharedKey *);

void debug_content(Content *);

void debug_write_permission(WritePermission *);

void debug_path_vector(PathVector *);

void debug_content_vector(ContentVector *);

#endif
