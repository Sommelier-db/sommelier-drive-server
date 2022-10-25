#include "views/user.h"

json_t *get_api_user_request(struct mg_str s) {
    json_error_t err;
    json_t *j = json_loads(s.ptr, 0, &err);

    // userId: int
    if (json_has_key(j, "userId", JSON_INTEGER)) {
        return j;
    } else {
        free(j);
        return NULL;
    }
}

json_t *post_api_user_request(struct mg_str s) {
    json_error_t err;
    json_t *j = json_loads(s.ptr, 0, &err);

    // dataPK: string, keywordPK: string, permissionHash: string,
    // dataCT: string, keywordCT: string
    int c = json_has_key(j, "dataPK", JSON_STRING) &&
            json_has_key(j, "keywordPK", JSON_STRING) &&
            // json_has_key(j, "permissionHash", JSON_STRING) &&
            json_has_key(j, "dataCT", JSON_STRING) &&
            json_has_key(j, "keywordCT", JSON_STRING);

    if (c) {
        return j;
    } else {
        free(j);
        return NULL;
    }
}

// GET, POST /api/user
void api_user_view(struct mg_connection *c, struct mg_http_message *hm,
                   sqlite3 *db) {
    char *method = request_method(hm->method);

    if (strcmp(method, "GET") == 0) {
        json_t *body = get_api_user_request(hm->body);

        if (body != NULL) {
            uint64_t userId =
                (uint64_t)json_integer_value(json_object_get(body, "userId"));
            User *u = ReadUser(db, userId);

            if (u != NULL) {
                json_t *ju = decode_json_user(u);
                char *dumped = json_dumps(ju, 0);
                mg_http_reply(c, 200, "", "%s", dumped);

                free(dumped);
                free(ju);
                finalize_user(u);
            } else {
                __ERROR_REPLY(c);
            }
        } else {
            __ERROR_REPLY(c);
        }
    } else if (strcmp(method, "POST") == 0) {
        json_t *body = post_api_user_request(hm->body);

        if (body != NULL) {
            char *dpk =
                (char *)json_string_value(json_object_get(body, "dataPK"));
            char *kpk =
                (char *)json_string_value(json_object_get(body, "keywordPK"));
            char *dct =
                (char *)json_string_value(json_object_get(body, "dataCT"));
            char *kct =
                (char *)json_string_value(json_object_get(body, "keywordCT"));

            // TODO: NULL check?
            User *u = CreateUser(db, dpk, kpk);

            char *ph = computePermissionHash(u->id, "/");

            Path *p = CreatePath(db, u->id, ph, dct, kct);

            WritePermission *wp = CreateWritePermission(db, p->id, u->id);

            if (wp != NULL) {
                mg_http_reply(c, 200, "", "%d", u->id);

                finalize_user(u);
                finalize_path(p);
                finalize_write_permission(wp);
            } else {
                __ERROR_REPLY(c);
            }
        } else {
            __ERROR_REPLY(c);
        }
    } else {
        __ERROR_REPLY(c);
    }

    free(method);
}
