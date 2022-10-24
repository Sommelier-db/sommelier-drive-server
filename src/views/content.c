#include "views/content.h"

json_t *get_api_content_request(struct mg_str s) {
    json_error_t err;
    json_t *j = json_loads(s.ptr, 0, &err);

    // sharedKeyHash: string
    if (json_has_key(j, "sharedKeyHash", JSON_STRING)) {
        return j;
    } else {
        free(j);
        return NULL;
    }
}

json_t *post_api_content_request(struct mg_str s) {
    json_error_t err;
    json_t *j = json_loads(s.ptr, 0, &err);

    // sharedKeyHash: string, authorizationPK: string, ct: string
    int c = json_has_key(j, "sharedKeyHash", JSON_STRING) &&
            json_has_key(j, "authorizationPK", JSON_STRING) &&
            json_has_key(j, "ct", JSON_STRING);

    if (c) {
        return j;
    } else {
        free(j);
        return NULL;
    }
}

json_t *put_api_content_request(struct mg_str s) {
    json_error_t err;
    json_t *j = json_loads(s.ptr, 0, &err);

    // sharedKeyHash: string, ct: string
    int c = json_has_key(j, "sharedKeyHash", JSON_STRING) &&
            json_has_key(j, "ct", JSON_STRING);

    if (c) {
        return j;
    } else {
        free(j);
        return NULL;
    }
}

void api_content_view(struct mg_connection *c, struct mg_http_message *hm,
                      sqlite3 *db) {
    char *method = request_method(hm->method);

    if (strcmp(method, "GET") == 0) {
        json_t *body = get_api_content_request(hm->body);

        if (body != NULL) {
        } else {
            __ERROR_REPLY(c);
        }
    } else if (strcmp(method, "POST") == 0) {
        json_t *body = post_api_content_request(hm->body);

        if (body != NULL) {
        } else {
            __ERROR_REPLY(c);
        }
    } else if (strcmp(method, "PUT") == 0) {
        json_t *body = put_api_content_request(hm->body);

        if (body != NULL) {
        } else {
            __ERROR_REPLY(c);
        }
    } else {
        __ERROR_REPLY(c);
    }

    free(method);
}
