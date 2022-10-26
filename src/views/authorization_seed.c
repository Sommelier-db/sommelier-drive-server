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
    char *method = request_method(hm);

    if (DEBUG) {
        char *uri = request_uri(hm);

        char msg[100] = "";
        sprintf(msg, "HTTP Request: %s %s", method, uri);
        logging_debug(msg);
        free(uri);
    }

    if (strcmp(method, "GET") == 0) {
        json_t *body = get_api_authorization_seed_request(hm->body);

        if (body != NULL) {
            uint64_t pathId =
                (uint64_t)json_integer_value(json_object_get(body, "pathId"));
            AuthorizationSeed *as = ReadAuthorizationSeed(db, pathId);

            if (as != NULL) {
                json_t *jas = decode_json_authorization_seed(as);
                char *dumped = json_dumps(jas, 0);
                mg_http_reply(c, 200, "", "%s", dumped);

                free(dumped);
                free(jas);
                finalize_authorization_seed(as);
            } else {
                __ERROR_REPLY(c);
            }
        } else {
            __ERROR_REPLY(c);
        }
    } else if (strcmp(method, "POST") == 0) {
        json_t *body = post_api_authorization_seed_request(hm->body);

        if (body != NULL) {
            uint64_t writeUserId = (uint64_t)json_integer_value(
                json_object_get(body, "writeUserId"));
            uint64_t pathId =
                (uint64_t)json_integer_value(json_object_get(body, "pathId"));
            char *ct = (char *)json_string_value(json_object_get(body, "ct"));

            // TODO: verify digital signature.
            User *writeUser = ReadUser(db, writeUserId);
            IncrementUserNonce(db, writeUser);

            AuthorizationSeed *as = CreateAuthorizationSeed(db, pathId, ct);

            mg_http_reply(c, 200, "", "%d", as->id);

            finalize_authorization_seed(as);
        } else {
            __ERROR_REPLY(c);
        }
    }

    free(method);
}
