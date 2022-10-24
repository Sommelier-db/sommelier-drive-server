#include "views/authorization_seed.h"

json_t *get_api_authorization_seed_request(struct mg_str s) {
    json_error_t err;
    json_t *j = json_loads(s.ptr, 0, &err);

    // pathId: int
    if (json_has_key(j, "pathId", JSON_INTEGER)) {
        return j;
    } else {
        free(j);
        return NULL;
    }
}

json_t *post_api_authorization_seed_request(struct mg_str s) {
    json_error_t err;
    json_t *j = json_loads(s.ptr, 0, &err);

    // writeUserId: int, pathId: int, ct: string
    int c = json_has_key(j, "writeUserId", JSON_INTEGER) &&
            json_has_key(j, "pathId", JSON_INTEGER) &&
            json_has_key(j, "ct", JSON_STRING);

    if (c) {
        return j;
    } else {
        free(j);
        return NULL;
    }
}

void api_authorization_seed_view(struct mg_connection *c,
                                 struct mg_http_message *hm, sqlite3 *db) {
    char *method = request_method(hm->method);

    if (strcmp(method, "GET") == 0) {
        json_t *body = get_api_authorization_seed_request(hm->body);

        if (body != NULL) {
        } else {
            __ERROR_REPLY(c);
        }
    } else if (strcmp(method, "POST") == 0) {
        json_t *body = post_api_authorization_seed_request(hm->body);

        if (body != NULL) {
        } else {
            __ERROR_REPLY(c);
        }
    }

    free(method);
}
