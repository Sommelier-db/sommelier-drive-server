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
                   SommelierDBMS *db) {
    char *method = request_method(hm);

    if (DEBUG) {
        char *uri = request_uri(hm);

        char msg[100] = "";
        sprintf(msg, "HTTP Request: %s %s", method, uri);
        logging_debug(msg);
        free(uri);
    }

    if (strcmp(method, "GET") == 0) {
        json_t *body = get_api_path_request(hm->body);

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

            free(body);
        } else {
            __ERROR_REPLY(c);
        }
    } else if (strcmp(method, "POST") == 0) {
        json_t *body = post_api_path_request(hm->body);

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
            uint64_t readUserId = (uint64_t)json_integer_value(
                json_object_get(body, "readUserId"));
            char *permissionHash = (char *)json_string_value(
                json_object_get(body, "permissionHash"));
            char *dataCT =
                (char *)json_string_value(json_object_get(body, "dataCT"));
            char *keywordCT =
                (char *)json_string_value(json_object_get(body, "keywordCT"));

            // TODO: verify digital signature.
            User *writeUser = ReadUser(db, writeUserId);

            if (writeUser != NULL) {
                IncrementUserNonce(db, writeUser);

                Path *p = CreatePath(db, readUserId, permissionHash, dataCT,
                                     keywordCT);

                if (p != NULL) {
                    mg_http_reply(c, 200, "", "%d", p->id);

                    finalize_path(p);
                } else {
                    __ERROR_REPLY(c);
                }

                finalize_user(writeUser);
            } else {
                __ERROR_REPLY(c);
            }

            free(body);
        } else {
            __ERROR_REPLY(c);
        }
    } else {
        __ERROR_REPLY(c);
    }

    free(method);
}

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

// GET /api/file-path/children
void api_path_children_view(struct mg_connection *c, struct mg_http_message *hm,
                            SommelierDBMS *db) {
    char *method = request_method(hm);

    if (DEBUG) {
        char *uri = request_uri(hm);

        char msg[100] = "";
        sprintf(msg, "HTTP Request: %s %s", method, uri);
        logging_debug(msg);
        free(uri);
    }

    if (strcmp(method, "GET") == 0) {
        json_t *body = get_api_path_children_request(hm->body);

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
            char *ph = (char *)json_string_value(
                json_object_get(body, "permissionHash"));

            PathVector *pv = FilterByPermissionHash(db, ph);

            if (pv != NULL) {
                json_t *jpa = decode_json_path_vector(pv);
                char *dumped = json_dumps(jpa, 0);

                mg_http_reply(c, 200, "", "%s", dumped);

                free(dumped);
                free(jpa);
                free(ph);

                finalize_path_vector(pv);
            } else {
                __ERROR_REPLY(c);
            }

            free(body);
        }
    } else {
        __ERROR_REPLY(c);
    }

    free(method);
}

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

void api_path_search_view(struct mg_connection *c, struct mg_http_message *hm,
                          SommelierDBMS *db) {
    char *method = request_method(hm);

    if (DEBUG) {
        char *uri = request_uri(hm);

        char msg[100] = "";
        sprintf(msg, "HTTP Request: %s %s", method, uri);
        logging_debug(msg);
        free(uri);
    }

    if (strcmp(method, "GET") == 0) {
        json_t *body = get_api_path_search_request(hm->body);

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
            uint64_t userId =
                (uint64_t)json_integer_value(json_object_get(body, "userId"));
            char *td =
                (char *)json_string_value(json_object_get(body, "trapdoor"));

            PathVector *pv = SearchEncryptedPath(db, userId, td);

            if (pv != NULL) {
                json_t *jpa = decode_json_path_vector(pv);
                char *dumped = json_dumps(jpa, 0);

                mg_http_reply(c, 200, "", "%s", dumped);

                free(dumped);
                free(jpa);

                finalize_path_vector(pv);
            } else {
                __ERROR_REPLY(c);
            }

            free(body);
        }
    } else {
        __ERROR_REPLY(c);
    }

    free(method);
}
