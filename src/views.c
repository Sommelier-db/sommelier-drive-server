#include "views.h"

#include <jansson.h>

#include "mongoose.h"
#include "orm.h"
#include "util.h"

#define __ERROR_REPLY(c) \
    (mg_http_reply(c, 500, "", "{\"msg\": \"internal error\"}\n"))

char *_mg_method(struct mg_str s) {
    char *method = INITIALIZE_STRING(s.len + 1);
    strncpy(method, s.ptr, s.len);
    method[s.len] = '\0';
    return method;
}

json_t *_mg_json_body(struct mg_str s, int *status) {
    json_error_t err;
    json_t *j = json_loads(s.ptr, 0, &err);

    if (j == NULL) {
        *status = 1;
        return NULL;
    } else {
        *status = 0;
        return j;
    }
}

void main_view(struct mg_connection *c, struct mg_http_message *hm,
               sqlite3 *_db) {
    int status = 0;
    json_t *body = _mg_json_body(hm->body, &status);
    if (status == 0) {
        char *dumped = json_dumps(body, 0);
        mg_http_reply(c, 200, "", "%s\n", dumped);
        free(dumped);
    } else {
        __ERROR_REPLY(c);
    }
}

void api_users_view(struct mg_connection *c, struct mg_http_message *hm,
                    sqlite3 *db) {
    char *method = _mg_method(hm->method);
    int status = 0;

    if (strcmp(method, "GET") == 0) {
        json_t *body = _mg_json_body(hm->body, &status);

        if (status == 0) {
            json_t *uid = json_object_get(body, "userId");

            if (uid != NULL) {
                uint64_t user_id = (uint64_t)json_integer_value(uid);
                User *u = ReadUser(db, user_id);
                json_t *ju = decode_json_user(u);
                char *dumped = json_dumps(ju, 0);
                mg_http_reply(c, 200, "", "%s\n", dumped);
                free(dumped);
                finalize_user(u);
                free(ju);
                free(uid);
            }
        }
        __ERROR_REPLY(c);
    } else if (strcmp(method, "POST") == 0) {
        mg_http_reply(c, 200, "", "{\"result\": \"84\"}\n");
    } else {
        __ERROR_REPLY(c);
        exit(1);
    }

    free(method);
}
