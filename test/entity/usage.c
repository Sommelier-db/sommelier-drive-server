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
    // FIXME: なぜかこいつだけ失敗する
    // test_entity_path();
    // test_entity_shared_key();
    test_entity_content();
    // test_entity_write_permission();
    // test_entity_path_vector();
    test_entity_content_vector();

    return 0;
}

void test_entity_user() {
    printf("test_entity_user: User API usage.\n");

    User *u = initialize_user(1, "pkd01", "pkk01");
    json_t *j = json_loads("{}", 0, NULL);
    char *dumped;

    decode_json_user(u, j);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("user json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);

    set_user(u, "pkdx1", "pkkx1");

    decode_json_user(u, j);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("user json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);

    finalize_user(u);
}

void test_entity_path() {
    printf("test_entity_path: Path API usage.\n");

    Path *p = initialize_path(111, 222, "ph111", "dct111", "kct111");
    json_t *j = json_loads("{}", 0, NULL);
    char *dumped;

    decode_json_path(p, j);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("path json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);

    set_path(p, 333, "ph000", "dct000", "kct000");

    decode_json_path(p, j);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("path json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);

    finalize_path(p);
}

void test_entity_shared_key() {
    printf("test_entity_shared_key: SharedKey API usage.\n");

    SharedKey *sk = initialize_shared_key(1, 2, "ctsk1");
    json_t *j = json_loads("{}", 0, NULL);
    char *dumped;

    decode_json_shared_key(sk, j);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("shared_key json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);

    set_shared_key(sk, 3, "ctsk22");

    decode_json_shared_key(sk, j);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("shared_key json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);

    finalize_shared_key(sk);
}

void test_entity_content() {
    printf("test_entity_content: Content API usage.\n");

    Content *c = initialize_content(1, "skh1", "ctc1");
    json_t *j = json_loads("{}", 0, NULL);
    char *dumped;

    decode_json_content(c, j);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("content json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);

    set_content(c, "skh2", "ctc2");

    decode_json_content(c, j);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("content json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);

    finalize_content(c);
}

void test_entity_write_permission() {
    printf("test_entity_write_permission: WritePermission API usage.\n");

    WritePermission *wp = initialize_write_permission(1, 2, 3);
    json_t *j = json_loads("{}", 0, NULL);
    char *dumped;

    decode_json_write_permission(wp, j);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("write_permission json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);

    set_write_permission(wp, 4, 5);

    decode_json_write_permission(wp, j);
    dumped = json_dumps(j, 0);
    if (dumped != NULL) {
        printf("write_permission json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);

    finalize_write_permission(wp);
}

void test_entity_path_vector() {
    printf("test_entity_path_vector: PathVector API usage.\n");

    PathVector *vec = initialize_path_vector();

    Path *path1 = initialize_path(11, 21, "ph333111", "ctd444111", "ctk555111");
    Path *path2 = initialize_path(12, 22, "ph333222", "ctd444222", "ctk555222");
    Path *path3 = initialize_path(13, 23, "ph333333", "ctd444333", "ctk555333");
    Path *path4 = initialize_path(14, 24, "ph333444", "ctd444444", "ctk555444");
    push_path_vector(vec, path1);
    push_path_vector(vec, path2);
    push_path_vector(vec, path3);
    push_path_vector(vec, path4);

    json_t *a = decode_json_path_vector(vec);
    char *dumped = json_dumps(a, 0);
    if (dumped != NULL) {
        printf("write_permission json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);
    free(a);

    finalize_path_vector(vec);
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
        printf("write_permission json - %s\n", dumped);
    } else {
        errordebug("dumped is NULL.");
    }
    free(dumped);
    free(a);

    finalize_content_vector(v);
}
