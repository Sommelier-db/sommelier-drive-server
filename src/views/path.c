#include "views/path.h"

json_t *get_api_path_request(struct mg_str s) {
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

json_t *post_api_path_request(struct mg_str s) {
    json_error_t err;
    json_t *j = json_loads(s.ptr, 0, &err);

    // writeUserId: int, readUserId: int, permissionHash: string,
    // dataCT: string, keywordCT: string
    int c = json_has_key(j, "writeUserId", JSON_INTEGER) &&
            json_has_key(j, "readUserId", JSON_INTEGER) &&
            json_has_key(j, "permissionHash", JSON_STRING) &&
            json_has_key(j, "dataCT", JSON_STRING) &&
            json_has_key(j, "keywordCT", JSON_STRING);

    if (c) {
        return j;
    } else {
        free(j);
        return NULL;
    }
}

// GET, POST /api/file-path
void api_path_view(struct mg_connection *c, struct mg_http_message *hm,
                   sqlite3 *db) {
    char *method = request_method(hm->method);

    if (strcmp(method, "GET") == 0) {
        json_t *body = get_api_user_request(hm->body);

        if (body != NULL) {
            uint64_t pathId =
                (uint64_t)json_integer_value(json_object_get(body, "pathId"));
            Path *p = ReadPath(db, pathId);

            if (p != NULL) {
                json_t *jp = decode_json_path(p);
                char *dumped = json_dumps(jp, 0);
                mg_http_reply(c, 200, "", "%s", dumped);

                free(dumped);
                free(jp);
                finalize_path(p);
            } else {
                __ERROR_REPLY(c);
            }
        } else {
            __ERROR_REPLY(c);
        }
    } else if (strcmp(method, "POST") == 0) {
        json_t *body = post_api_user_request(hm->body);

        if (body != NULL) {
            // uint64_t writeUserId = (uint64_t)json_integer_value(
            //     json_object_get(body, "writeUserId"));
            uint64_t readUserId = (uint64_t)json_integer_value(
                json_object_get(body, "readUserId"));
            char *permissionHash = (char *)json_string_value(
                json_object_get(body, "permissionHash"));
            char *dataCT =
                (char *)json_string_value(json_object_get(body, "dataCT"));
            char *keywordCT =
                (char *)json_string_value(json_object_get(body, "keywordCT"));

            Path *p =
                CreatePath(db, readUserId, permissionHash, dataCT, keywordCT);

            mg_http_reply(c, 200, "", "%d", p->id);

            free(permissionHash);
            free(dataCT);
            free(keywordCT);
            finalize_path(p);
        } else {
            __ERROR_REPLY(c);
        }
    } else {
        __ERROR_REPLY(c);
    }

    free(method);
}

// GET /api/file-path/children
json_t *get_api_path_children_request(struct mg_str s) {
    json_error_t err;
    json_t *j = json_loads(s.ptr, 0, &err);

    // permissionHash: string
    int c = json_has_key(j, "permissionHash", JSON_STRING);

    if (c) {
        return j;
    } else {
        free(j);
        return NULL;
    }
}

void api_path_children_view(struct mg_connection *, struct mg_http_message *,
                            sqlite3 *);

// GET /api/file-path/search
json_t *get_api_path_search_request(struct mg_str s) {
    json_error_t err;
    json_t *j = json_loads(s.ptr, 0, &err);

    // userId: int, trapdoor: string
    int c = json_has_key(j, "userId", JSON_INTEGER) &&
            json_has_key(j, "trapdoor", JSON_STRING);

    if (c) {
        return j;
    } else {
        free(j);
        return NULL;
    }
}

void api_path_search_view(struct mg_connection *, struct mg_http_message *,
                          sqlite3 *);
