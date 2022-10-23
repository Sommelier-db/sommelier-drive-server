#include "entity.h"

void test_entity_user();
void test_entity_path();
void test_entity_shared_key();
void test_entity_content();
void test_entity_write_permission();
void test_entity_path_vector();
void test_entity_content_vector();

int main() {
    // test_entity_user();

    // FIXME: なぜかこいつらだけ失敗する
    // test_entity_path();

    test_entity_shared_key();

    test_entity_content();

    // test_entity_write_permission();

    // FIXME: なぜかこいつらだけ失敗する
    // test_entity_path_vector();

    test_entity_content_vector();

    return 0;
}

void test_entity_user() {
    printf("test_entity_user: User API usage.\n");

    // User *u = initialize_user(1, "pkd01", "pkk01");
    User *u = initialize_user();
    set_user(u, 1, "pkd01", "pkk02", 1);
    if (DEBUG) {
        debug_user(u);
    }

    json_t *j;
    char *dumped;

    j = decode_json_user(u);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("user json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(j);
    free(dumped);

    set_user_data_public_key(u, "pkd1101");
    set_user_keyword_public_key(u, "pkk2022");
    increment_nonce(u);
    if (DEBUG) {
        debug_user(u);
    }

    j = decode_json_user(u);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("user json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(j);
    free(dumped);

    finalize_user(u);
}

void test_entity_path() {
    printf("test_entity_path: Path API usage.\n");

    Path *p = initialize_path();
    set_path(p, 1, 1, "ph01", "ct_d01", "ct_k01");
    if (DEBUG) {
        debug_path(p);
    }

    json_t *j;
    char *dumped;

    j = decode_json_path(p);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("path json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(j);
    free(dumped);

    set_path_data_cipher_text(p, "ct_d0022");
    set_path_keyword_cipher_text(p, "ct_k0022");

    j = decode_json_path(p);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("path json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(j);
    free(dumped);

    finalize_path(p);
}

void test_entity_shared_key() {
    printf("test_entity_shared_key: SharedKey API usage.\n");

    SharedKey *sk = initialize_shared_key();
    set_shared_key(sk, 1, 2, "ctsk1");
    if (DEBUG) {
        debug_shared_key(sk);
    }

    json_t *j;
    char *dumped;

    j = decode_json_shared_key(sk);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("shared_key json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(j);
    free(dumped);

    set_shared_key_share_key_cipher_text(sk, "ctsk22");

    j = decode_json_shared_key(sk);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("shared_key json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(j);
    free(dumped);

    finalize_shared_key(sk);
}

void test_entity_content() {
    printf("test_entity_content: Content API usage.\n");

    Content *c = initialize_content();
    set_content(c, 1, "skh1", "ctc1");
    if (DEBUG) {
        debug_content(c);
    }

    json_t *j;
    char *dumped;

    j = decode_json_content(c);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("content json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(j);
    free(dumped);

    set_content_shared_key_hash(c, "skh2");

    j = decode_json_content(c);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("content json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(j);
    free(dumped);

    finalize_content(c);
}

void test_entity_write_permission() {
    printf("test_entity_write_permission: WritePermission API usage.\n");

    WritePermission *wp = initialize_write_permission(1, 2, 3);
    json_t *j;
    char *dumped;

    j = decode_json_write_permission(wp);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("write_permission json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(j);
    free(dumped);

    set_write_permission(wp, 4, 5);

    j = decode_json_write_permission(wp);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("write_permission json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(j);
    free(dumped);

    finalize_write_permission(wp);
}

void test_entity_path_vector() {
    printf("test_entity_path_vector: PathVector API usage.\n");

    PathVector *v = initialize_path_vector();

    for (int i = 0; i < 3; i++) {
        Path *p = initialize_path();
        set_path(p, i + 1, i + 101, "ph", "ctd", "ctk");
        push_path_vector(v, p);
    }

    json_t *a = decode_json_path_vector(v);
    char *dumped = json_dumps(a, 0);
    if (dumped != NULL) {
        printf("path_vector json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }

    free(a);
    free(dumped);
    finalize_path_vector(v);
}

void test_entity_content_vector() {
    printf("test_entity_content_vector: ContentVector API usage.\n");

    ContentVector *v = initialize_content_vector();

    for (int i = 0; i < 3; i++) {
        Content *c = initialize_content();
        set_content(c, i + 1, "xxx", "iii");
        push_content_vector(v, c);
    }

    json_t *a = decode_json_content_vector(v);
    char *dumped = json_dumps(a, 0);
    if (dumped != NULL) {
        printf("content_vector json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }

    free(a);
    free(dumped);
    finalize_content_vector(v);
}
