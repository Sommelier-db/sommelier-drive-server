#include "entity.h"

void test_entity_user();
void test_entity_path();
void test_entity_shared_key();
void test_entity_content();
void test_entity_write_permission();
void test_entity_path_vector();
void test_entity_content_vector();

int main() {
    test_entity_user();

    // FIXME: なぜかこいららだけ失敗する
    // test_entity_path();

    test_entity_shared_key();

    test_entity_content();

    test_entity_write_permission();

    // FIXME: なぜかこいつらだけ失敗する
    // test_entity_path_vector();

    test_entity_content_vector();

    return 0;
}

void test_entity_user() {
    printf("test_entity_user: User API usage.\n");

    User *u = initialize_user(1, "pkd01", "pkk01");
    json_t *j;
    char *dumped;

    j = decode_json_user(u);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("user json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);
    free(j);

    set_user(u, "pkdx1", "pkkx1");

    j = decode_json_user(u);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("user json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);
    free(j);

    finalize_user(u);
}

void test_entity_path() {
    printf("test_entity_path: Path API usage.\n");

    Path *p = initialize_path(111, 222, "ph111", "dct111", "kct111");
    json_t *j;
    char *dumped;

    j = decode_json_path(p);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("path json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);
    free(j);

    set_path(p, 333, "ph000", "dct000", "kct000");

    j = decode_json_path(p);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("path json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);
    free(j);

    finalize_path(p);
}

void test_entity_shared_key() {
    printf("test_entity_shared_key: SharedKey API usage.\n");

    SharedKey *sk = initialize_shared_key(1, 2, "ctsk1");
    json_t *j;
    char *dumped;

    j = decode_json_shared_key(sk);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("shared_key json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);
    free(j);

    set_shared_key(sk, 3, "ctsk22");

    j = decode_json_shared_key(sk);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("shared_key json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);
    free(j);

    finalize_shared_key(sk);
}

void test_entity_content() {
    printf("test_entity_content: Content API usage.\n");

    Content *c = initialize_content(1, "skh1", "ctc1");
    json_t *j;
    char *dumped;

    j = decode_json_content(c);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("content json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);
    free(j);

    set_content(c, "skh2", "ctc2");

    j = decode_json_content(c);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("content json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);
    free(j);

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
    free(dumped);
    free(j);

    set_write_permission(wp, 4, 5);

    j = decode_json_write_permission(wp);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("write_permission json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);
    free(j);

    finalize_write_permission(wp);
}

void test_entity_path_vector() {
    printf("test_entity_path_vector: PathVector API usage.\n");

    printf("1. initialize\n");
    PathVector *v = initialize_path_vector();

    printf("2. contents\n");
    for (int i = 0; i < 3; i++) {
        Path *p = initialize_path(i + 1, i + 101, "ph", "ctd", "ctk");
        push_path_vector(v, p);
    }

    printf("3. decode json\n");
    json_t *a = decode_json_path_vector(v);
    printf("4. dumped json\n");
    char *dumped = json_dumps(a, 0);
    if (dumped != NULL) {
        printf("path_vector json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);
    free(a);

    finalize_path_vector(v);
}

void test_entity_content_vector() {
    printf("test_entity_content_vector: ContentVector API usage.\n");

    printf("1. initialize\n");
    ContentVector *v = initialize_content_vector();

    printf("2. contents\n");
    for (int i = 0; i < 3; i++) {
        Content *c = initialize_content(i + 1, "xxx", "iii");
        push_content_vector(v, c);
    }

    printf("3. decode json\n");
    json_t *a = decode_json_content_vector(v);
    printf("4. dumped json\n");
    char *dumped = json_dumps(a, 0);
    if (dumped != NULL) {
        printf("content_vector json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);
    free(a);

    finalize_content_vector(v);
}
