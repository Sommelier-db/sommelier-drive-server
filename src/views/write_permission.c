#include "views/write_permission.h"

// GET, POST /api/write-permission
json_t *get_api_write_permission_request(struct mg_str s) {
    json_error_t err;
    json_t *j = json_loads(s.ptr, 0, &err);

    // pathId: int
    int c = json_has_key(j, "pathId", JSON_INTEGER);

    if (c) {
        return j;
    } else {
        free(j);
        return NULL;
    }
}

json_t *post_api_write_permission_request(struct mg_str s) {
    json_error_t err;
    json_t *j = json_loads(s.ptr, 0, &err);

    // writeUserId: int, pathId: int, ct: string
    int c = json_has_key(j, "writeUserId", JSON_INTEGER) &&
            json_has_key(j, "pathId", JSON_INTEGER) &&
            json_has_key(j, "userId", JSON_INTEGER);

    if (c) {
        return j;
    } else {
        free(j);
        return NULL;
    }
}

void api_write_permission_view(struct mg_connection *c,
                               struct mg_http_message *hm, SommelierDBMS *db) {
    char *method = request_method(hm);

    if (DEBUG) {
        char *uri = request_uri(hm);

        char msg[100] = "";
        sprintf(msg, "HTTP Request: %s %s", method, uri);
        logging_debug(msg);
        free(uri);
    }

    if (strcmp(method, "GET") == 0) {
        json_t *body = get_api_write_permission_request(hm->body);

        if (DEBUG) {
            char *dumped = json_dumps(body, 0);

            char _body[160] = "";
            strncpy(_body, dumped, 160);
            _body[159] = '\0';

            char msg[180] = "";
            char *suffix = strlen(dumped) > 160 ? "..." : "";
            sprintf(msg, "HTTP Body: %s%s", _body, suffix);

            logging_debug(msg);
            free(dumped);
        }

        if (body != NULL) {
            uint64_t pathId =
                (uint64_t)json_integer_value(json_object_get(body, "pathId"));

            WritePermission *wp = ReadWritePermission(db, pathId);

            if (wp != NULL) {
                json_t *jwp = decode_json_write_permission(wp);
                char *dumped = json_dumps(jwp, 0);
                mg_http_reply(c, 200, "", "%s", dumped);

                free(dumped);
                free(jwp);
                finalize_write_permission(wp);
            } else {
                __ERROR_REPLY(c);
            }
        } else {
            __ERROR_REPLY(c);
        }
    } else if (strcmp(method, "POST") == 0) {
        json_t *body = post_api_write_permission_request(hm->body);

        if (DEBUG) {
            char *dumped = json_dumps(body, 0);

            char _body[160] = "";
            strncpy(_body, dumped, 160);
            _body[159] = '\0';

            char msg[180] = "";
            char *suffix = strlen(dumped) > 160 ? "..." : "";
            sprintf(msg, "HTTP Body: %s%s", _body, suffix);

            logging_debug(msg);
            free(dumped);
        }

        if (body != NULL) {
            uint64_t writeUserId = (uint64_t)json_integer_value(
                json_object_get(body, "writeUserId"));
            uint64_t pathId =
                (uint64_t)json_integer_value(json_object_get(body, "pathId"));
            uint64_t userId =
                (uint64_t)json_integer_value(json_object_get(body, "userId"));

            // TODO: verify digital signature.
            User *writeUser = ReadUser(db, writeUserId);
            IncrementUserNonce(db, writeUser);

            WritePermission *wp = CreateWritePermission(db, pathId, userId);

            mg_http_reply(c, 200, "", "%d", wp->id);

            finalize_write_permission(wp);
        } else {
            __ERROR_REPLY(c);
        }
    } else {
        __ERROR_REPLY(c);
    }
    free(method);
}
