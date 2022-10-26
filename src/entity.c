#include "entity.h"

// User API

User *initialize_user() {
    User *u = INITIALIZE(User);

    if (u == NULL) {
        logging_error("Memory allocation is failed. - User");
        exit(1);
    }

    if (DEBUG) {
        u->data_public_key =
            alloc_string_debug(MAX_SIZE_PKE_KEY, "User::data_public_key");
        u->keyword_public_key =
            alloc_string_debug(MAX_SIZE_PKSE_KEY, "User::keyword_public_key");
    } else {
        u->data_public_key = alloc_string(MAX_SIZE_PKE_KEY);
        u->keyword_public_key = alloc_string(MAX_SIZE_PKSE_KEY);
    }

    return u;
}

void finalize_user(User *user) {
    free(user->data_public_key);
    free(user->keyword_public_key);
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
        u->data_public_key = copy_string(u->data_public_key, pkd);
    } else if (DEBUG) {
        logging_error("Arg pkd is NULL. - set_user_data_public_key");
    }
}

void set_user_keyword_public_key(User *u, const char *pkk) {
    if (pkk != NULL) {
        u->keyword_public_key = copy_string(u->keyword_public_key, pkk);
    } else if (DEBUG) {
        logging_error("Arg pkk is NULL. - set_user_keyword_public_key");
    }
}

void set_user_nonce(User *u, uint64_t nonce) { u->nonce = nonce; }

uint64_t increment_user_nonce(User *user) { return ++(user->nonce); }

json_t *decode_json_user(User *user) {
    json_t *json = json_object();

    if (json_object_set(json, "userId", json_integer(user->id)) < 0) {
        logging_error("Setting JSON is failed. - User::id");
        exit(1);
    }

    if (json_object_set(json, "dataPK", json_string(user->data_public_key)) <
        0) {
        logging_error("Setting JSON is failed. - User::data_public_key");
        exit(1);
    }

    if (json_object_set(json, "keywordPK",
                        json_string(user->keyword_public_key)) < 0) {
        logging_error("Setting JSON is failed. - User::keyword_public_key");
        exit(1);
    }

    if (json_object_set(json, "nonce", json_integer(user->nonce)) < 0) {
        logging_error("Setting JSON is failed. - User::nonce");
        exit(1);
    }

    return json;
}

// Path API

Path *initialize_path() {
    Path *p = INITIALIZE(Path);

    if (p == NULL) {
        logging_error("Memory allocation is failed. - Path");
        exit(1);
    }

    if (DEBUG) {
        p->permission_hash =
            alloc_string_debug(MAX_SIZE_HASH, "Path::permission_hash");
        p->data_cipher_text =
            alloc_string_debug(MAX_SIZE_PKE_CT, "Path::data_cipher_text");
        p->keyword_cipher_text = alloc_string_debug(
            MAX_SIZE_PKSE_KEYWORD_CT, "Path::keyword_cipher_text");
    } else {
        p->permission_hash = alloc_string(MAX_SIZE_HASH);
        p->data_cipher_text = alloc_string(MAX_SIZE_PKE_CT);
        p->keyword_cipher_text = alloc_string(MAX_SIZE_PKSE_KEYWORD_CT);
    }

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
    if (ph != NULL) {
        p->permission_hash = copy_string(p->permission_hash, ph);
    } else if (DEBUG) {
        logging_error("Arg ph is NULL. - set_path_permission_hash");
    }
}

void set_path_data_cipher_text(Path *p, const char *ctd) {
    if (ctd != NULL) {
        p->data_cipher_text = copy_string(p->data_cipher_text, ctd);
    } else if (DEBUG) {
        logging_error("Arg ctd is NULL. - set_path_data_cipher_text");
    }
}

void set_path_keyword_cipher_text(Path *p, const char *ctk) {
    if (ctk != NULL) {
        p->keyword_cipher_text = copy_string(p->keyword_cipher_text, ctk);
    } else if (DEBUG) {
        logging_error("Arg ctk is NULL. - set_path_keyword_cipher_text");
    }
}

json_t *decode_json_path(Path *path) {
    json_t *json = json_object();

    if (json_object_set(json, "pathId", json_integer(path->id)) < 0) {
        logging_error("Setting JSON is failed. - Path::id");
        exit(1);
    }

    if (json_object_set(json, "userId", json_integer(path->user_id)) < 0) {
        logging_error("Setting JSON is failed. - Path::user_id");
        exit(1);
    }

    if (json_object_set(json, "permissionHash",
                        json_string(path->permission_hash)) < 0) {
        logging_error("Setting JSON is failed. - Path::permission_hash");
        exit(1);
    }

    if (json_object_set(json, "dataCT", json_string(path->data_cipher_text)) <
        0) {
        logging_error("Setting JSON is failed. - Path::data_cipher_text");
        exit(1);
    }

    if (json_object_set(json, "keywordCT",
                        json_string(path->keyword_cipher_text)) < 0) {
        logging_error("Setting JSON is failed. - Path::keyword_cipher_text");
        exit(1);
    }

    return json;
}

// PathVector API

PathVector *initialize_path_vector() {
    PathVector *vec = INITIALIZE(PathVector);

    if (vec == NULL) {
        logging_error("Memory allocation is failed. - PathVector");
        exit(1);
    }

    vec->max_size = VECTOR_MAX_SIZE_DEFAULT;
    vec->length = 0;
    vec->buf = INITIALIZE_SIZE(Path *, vec->max_size);

    if (vec->buf == NULL) {
        logging_error("Memory allocation is failed. - PathVector::buf");
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
        if (VERBOSE) {
            logging_debug("resize_path_vector() called. - push_path_vector()");
        }

        resize_path_vector(vec);
    }

    vec->buf[vec->length] = initialize_path();

    set_path(vec->buf[vec->length], path->id, path->user_id,
             path->permission_hash, path->data_cipher_text,
             path->keyword_cipher_text);

    return ++vec->length;
}

size_t resize_path_vector(PathVector *vec) {
    size_t new_size = VECTOR_EXTEND_RATE * vec->max_size;
    vec->max_size = new_size;
    vec->buf = (Path **)realloc(vec->buf, new_size * refsizeof(Path));

    if (vec->buf == NULL) {
        logging_error("Memory re-allocation is failed. - PathVector::buf");
        exit(1);
    }

    return new_size;
}

json_t *decode_json_path_vector(PathVector *vec) {
    json_t *array = json_array();

    for (int i = 0; i < vec->length; i++) {
        Path *p = vec->buf[i];
        json_t *j = decode_json_path(p);
        json_array_append(array, j);
    }

    return array;
}

// SharedKey API

SharedKey *initialize_shared_key() {
    SharedKey *sk = INITIALIZE(SharedKey);

    if (sk == NULL) {
        logging_error("Memory allocation is failed. - SharedKey");
        exit(1);
    }

    if (DEBUG) {
        sk->shared_key_cipher_text = alloc_string_debug(
            MAX_SIZE_PKE_CT, "SharedKey::shared_key_cipher_text");
    } else {
        sk->shared_key_cipher_text = alloc_string(MAX_SIZE_PKE_CT);
    }

    return sk;
}

void finalize_shared_key(SharedKey *key) {
    free(key->shared_key_cipher_text);
    free(key);
}

void set_shared_key(SharedKey *sk, uint64_t id, uint64_t path_id,
                    const char *ctsk) {
    set_shared_key_id(sk, id);
    set_shared_key_path_id(sk, path_id);
    set_shared_key_share_key_cipher_text(sk, ctsk);
}

void set_shared_key_id(SharedKey *sk, uint64_t id) { sk->id = id; }

void set_shared_key_path_id(SharedKey *sk, uint64_t path_id) {
    sk->path_id = path_id;
}

void set_shared_key_share_key_cipher_text(SharedKey *sk, const char *ctsk) {
    if (ctsk != NULL) {
        sk->shared_key_cipher_text =
            copy_string(sk->shared_key_cipher_text, ctsk);
    } else if (DEBUG) {
        logging_debug(
            "Arg ctsk is NULL. - set_shared_key_share_key_cipher_text");
    }
}

json_t *decode_json_shared_key(SharedKey *sk) {
    json_t *json = json_object();

    if (json_object_set(json, "sharedKeyId", json_integer(sk->id)) < 0) {
        logging_error("Setting JSON is failed. - SharedKey::id");
        exit(1);
    }

    if (json_object_set(json, "pathId", json_integer(sk->path_id)) < 0) {
        logging_error("Setting JSON is failed. - SharedKey::path_id");
        exit(1);
    }

    if (json_object_set(json, "ct", json_string(sk->shared_key_cipher_text)) <
        0) {
        logging_error(
            "Setting JSON is failed. - SharedKey::shared_key_cipher_text");
        exit(1);
    }

    return json;
}

// AuthorizationSeed API

AuthorizationSeed *initialize_authorization_seed() {
    AuthorizationSeed *as = INITIALIZE(AuthorizationSeed);

    if (as == NULL) {
        logging_error("Memory allocation is failed. - AuthorizationSeed");
        exit(1);
    }

    if (DEBUG) {
        as->authorization_seed_cipher_text = alloc_string_debug(
            MAX_SIZE_PKE_CT,
            "AuthorizationSeed::authorization_seed_cipher_text");
    } else {
        as->authorization_seed_cipher_text = alloc_string(MAX_SIZE_PKE_CT);
    }

    return as;
}

void finalize_authorization_seed(AuthorizationSeed *as) {
    free(as->authorization_seed_cipher_text);
    free(as);
}

void set_authorization_seed(AuthorizationSeed *as, uint64_t id, uint64_t pid,
                            const char *ctas) {
    set_authorization_seed_id(as, id);
    set_authorization_seed_path_id(as, pid);
    set_authorization_seed_authorization_seed_cipher_text(as, ctas);
}

void set_authorization_seed_id(AuthorizationSeed *as, uint64_t id) {
    as->id = id;
}

void set_authorization_seed_path_id(AuthorizationSeed *as, uint64_t pid) {
    as->path_id = pid;
}

void set_authorization_seed_authorization_seed_cipher_text(
    AuthorizationSeed *as, const char *ctas) {
    if (ctas != NULL) {
        as->authorization_seed_cipher_text =
            copy_string(as->authorization_seed_cipher_text, ctas);
    } else if (DEBUG) {
        logging_debug(
            "Arg ctas is NULL. - "
            "set_authorization_seed_authorization_seed_cipher_text");
    }
}

json_t *decode_json_authorization_seed(AuthorizationSeed *as) {
    json_t *json = json_object();

    if (json_object_set(json, "authorizationId", json_integer(as->id)) < 0) {
        logging_error("Setting JSON is failed. - AuthorizationSeed::id");
        exit(1);
    }

    if (json_object_set(json, "pathId", json_integer(as->path_id)) < 0) {
        logging_error("Setting JSON is failed. - AuthorizationSeed::path_id");
        exit(1);
    }

    if (json_object_set(json, "ct",
                        json_string(as->authorization_seed_cipher_text)) < 0) {
        logging_error(
            "Setting JSON is failed. - "
            "AuthorizationSeed::authorization_seed_cipher_text");
        exit(1);
    }

    return json;
}

// Content API

Content *initialize_content() {
    Content *c = INITIALIZE(Content);

    if (c == NULL) {
        logging_error("Memory allocation is failed. - Content");
        exit(1);
    }

    if (DEBUG) {
        c->shared_key_hash =
            alloc_string_debug(MAX_SIZE_HASH, "Content::shared_key_hash");
        c->authorization_public_key = alloc_string_debug(
            MAX_SIZE_PKE_KEY, "Content::authorization_public_key");
        c->content_cipher_text = alloc_string_debug(
            MAX_SIZE_PKSE_CONTENT_CT, "Content::content_cipher_text");
    } else {
        c->shared_key_hash = alloc_string(MAX_SIZE_HASH);
        c->authorization_public_key = alloc_string(MAX_SIZE_PKE_KEY);
        c->content_cipher_text = alloc_string(MAX_SIZE_PKSE_CONTENT_CT);
    }

    return c;
}

void finalize_content(Content *content) {
    free(content->shared_key_hash);
    free(content->authorization_public_key);
    free(content->content_cipher_text);
    free(content);
}

void set_content(Content *c, uint64_t id, const char *skh, const char *pka,
                 uint64_t nonce, const char *ctc) {
    set_content_id(c, id);
    set_content_shared_key_hash(c, skh);
    set_content_authorization_public_key(c, pka);
    set_content_nonce(c, nonce);
    set_content_content_cipher_text(c, ctc);
}

void set_content_id(Content *c, uint64_t id) { c->id = id; }

void set_content_shared_key_hash(Content *c, const char *skh) {
    if (skh != NULL) {
        c->shared_key_hash = copy_string(c->shared_key_hash, skh);
    } else if (DEBUG) {
        logging_error("Arg skh is NULL. - set_content_shared_key_hash");
    }
}

void set_content_authorization_public_key(Content *c, const char *pka) {
    if (pka != NULL) {
        c->authorization_public_key =
            copy_string(c->authorization_public_key, pka);
    } else if (DEBUG) {
        logging_debug(
            "Arg pka is NULL. - set_content_authorization_public_key");
    }
}

void set_content_nonce(Content *c, uint64_t nonce) { c->nonce = nonce; }

void set_content_content_cipher_text(Content *c, const char *ctc) {
    if (ctc != NULL) {
        c->content_cipher_text = copy_string(c->content_cipher_text, ctc);
    } else if (DEBUG) {
        logging_error("Arg ctc is NULL. - set_content_content_cipher_text");
    }
}

uint64_t increment_content_nonce(Content *c) { return ++(c->nonce); }

json_t *decode_json_content(Content *c) {
    json_t *json = json_object();

    if (json_object_set(json, "contentsId", json_integer(c->id)) < 0) {
        logging_error("Setting JSON is failed. - Content::id");
        exit(1);
    }

    if (json_object_set(json, "sharedKeyHash",
                        json_string(c->shared_key_hash)) < 0) {
        logging_error("Setting JSON is failed. - Content::shared_key_hash");
        exit(1);
    }

    if (json_object_set(json, "authorizationPK",
                        json_string(c->authorization_public_key)) < 0) {
        logging_error(
            "Setting JSON is failed. - Content::authorization_public_key");
        exit(1);
    }

    if (json_object_set(json, "nonce", json_integer(c->nonce)) < 0) {
        logging_error("Setting JSON is failed. - Content::nonce");
        exit(1);
    }

    if (json_object_set(json, "ct", json_string(c->content_cipher_text)) < 0) {
        logging_error("Setting JSON is failed. - Content::content_cipher_text");
        exit(1);
    }

    return json;
}

// ContetnVector API

ContentVector *initialize_content_vector() {
    ContentVector *vec = INITIALIZE(ContentVector);

    if (vec == NULL) {
        logging_error("Memory allocation is failed. - ContentVector");
        exit(1);
    }

    vec->max_size = VECTOR_MAX_SIZE_DEFAULT;
    vec->length = 0;
    vec->buf = INITIALIZE_SIZE(Content *, vec->max_size);

    if (vec->buf == NULL) {
        logging_error("Memory allocation is failed. - ContentVector::buf");
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
        if (VERBOSE) {
            logging_debug(
                "resize_content_vector() called. - push_content_vector");
        }

        resize_content_vector(vec);
    }

    vec->buf[vec->length] = initialize_content();

    set_content(vec->buf[vec->length], content->id, content->shared_key_hash,
                content->authorization_public_key, content->nonce,
                content->content_cipher_text);

    return ++vec->length;
}

size_t resize_content_vector(ContentVector *vec) {
    size_t new_size = VECTOR_EXTEND_RATE * vec->max_size;
    vec->max_size = new_size;
    vec->buf = (Content **)realloc(vec->buf, new_size * refsizeof(Content));

    if (vec->buf == NULL) {
        logging_error("Memory re-allocation is failed. - ContentVector::buf");
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

WritePermission *initialize_write_permission() {
    WritePermission *wp = INITIALIZE(WritePermission);

    if (wp == NULL) {
        logging_error("Memory allocation is failed. - WritePermission");
        exit(1);
    }

    return wp;
}

void finalize_write_permission(WritePermission *wp) { free(wp); }

void set_write_permission(WritePermission *wp, uint64_t id, uint64_t path_id,
                          uint64_t user_id) {
    set_write_permission_id(wp, id);
    set_write_permission_path_id(wp, path_id);
    set_write_permission_user_id(wp, user_id);
}

void set_write_permission_id(WritePermission *wp, uint64_t id) { wp->id = id; }

void set_write_permission_path_id(WritePermission *wp, uint64_t path_id) {
    wp->path_id = path_id;
}

void set_write_permission_user_id(WritePermission *wp, uint64_t user_id) {
    wp->user_id = user_id;
}

json_t *decode_json_write_permission(WritePermission *wp) {
    json_t *json = json_object();

    if (json_object_set(json, "wPermissionId", json_integer(wp->id)) < 0) {
        logging_error("Setting JSON is failed. - WritePermission::id");
        exit(1);
    }

    if (json_object_set(json, "pathId", json_integer(wp->path_id)) < 0) {
        logging_error("Setting JSON is failed. - WritePermission::path_id");
        exit(1);
    }

    if (json_object_set(json, "userId", json_integer(wp->user_id)) < 0) {
        logging_error("Setting JSON is failed. - WritePermission::user_id");
        exit(1);
    }

    return json;
}

// debug API

#if DEBUG

void debug_user(User *u) {
    if (u != NULL) {
        if (VERBOSE) {
            char msg[256 + MAX_SIZE_PKE_KEY + MAX_SIZE_PKSE_KEY] = "";
            sprintf(msg, "<User id: %ld, pkd: '%s', pkk: '%s', nonce: %ld>",
                    u->id, u->data_public_key, u->keyword_public_key, u->nonce);
            logging_debug(msg);
        } else {
            char msg[256] = "";
            sprintf(msg, "<User id: %ld, nonce: %ld>", u->id, u->nonce);
            logging_debug(msg);
        }
    } else {
        logging_error("User is NULL.");
    }
}

void debug_path(Path *p) {
    if (p != NULL) {
        if (VERBOSE) {
            char msg[256 + MAX_SIZE_HASH + MAX_SIZE_PKE_CT +
                     MAX_SIZE_PKSE_KEYWORD_CT] = "";
            sprintf(msg,
                    "<Path id: %ld, user_id: %ld, ph: '%s', data_ct: '%s', "
                    "keyword_ct: "
                    "'%s'>",
                    p->id, p->user_id, p->permission_hash, p->data_cipher_text,
                    p->keyword_cipher_text);
            logging_debug(msg);
        } else {
            char msg[256] = "";
            sprintf(msg, "<Path id: %ld, uid: %ld>", p->id, p->user_id);
            logging_debug(msg);
        }
    } else {
        logging_error("Path is NULL.");
    }
}

void debug_shared_key(SharedKey *sk) {
    if (sk != NULL) {
        if (VERBOSE) {
            char msg[256 + MAX_SIZE_PKE_CT] = "";
            sprintf(msg,
                    "<SharedKey id: %ld, path_id: %ld, shared_key_ct: '%s'>",
                    sk->id, sk->path_id, sk->shared_key_cipher_text);
            logging_debug(msg);
        } else {
            char msg[256] = "";
            sprintf(msg, "<SharedKey id: %ld, pid: %ld>", sk->id, sk->path_id);
            logging_debug(msg);
        }
    } else {
        logging_error("SharedKey is NULL.");
    }
}

void debug_content(Content *c) {
    if (c != NULL) {
        if (VERBOSE) {
            char msg[256 + MAX_SIZE_HASH + MAX_SIZE_PKSE_CONTENT_CT] = "";
            sprintf(
                msg,
                "<Content id: %ld, shared_key_hash: '%s', content_ct: '%s'>",
                c->id, c->shared_key_hash, c->content_cipher_text);
            logging_debug(msg);
        } else {
            char msg[256] = "";
            sprintf(msg, "<Content id: %ld>", c->id);
            logging_debug(msg);
        }
    } else {
        logging_error("Content is NULL.");
    }
}

void debug_write_permission(WritePermission *wp) {
    if (wp != NULL) {
        char msg[256] = "";
        if (VERBOSE) {
            sprintf(msg,
                    "<WritePermission id: %ld, path_id: %ld, user_id: %ld>",
                    wp->id, wp->path_id, wp->user_id);
        } else {
            sprintf(msg, "<WritePermission id: %ld, pid: %ld, uid: %ld>",
                    wp->id, wp->path_id, wp->user_id);
        }
        logging_debug(msg);

    } else {
        logging_error("WritePermission is NULL.");
    }
}

void debug_path_vector(PathVector *);

void debug_content_vector(ContentVector *);

#endif
