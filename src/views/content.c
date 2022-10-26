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
        json_t *body = get_api_content_request(hm->body);

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
            char *skh = (char *)json_string_value(
                json_object_get(body, "sharedKeyHash"));
            Content *con = ReadContentBySharedKeyHash(db, skh);

            if (con != NULL) {
                json_t *jcon = decode_json_content(con);
                char *dumped = json_dumps(jcon, 0);
                mg_http_reply(c, 200, "", "%s", dumped);

                free(dumped);
                free(jcon);
                finalize_content(con);
            } else {
                __ERROR_REPLY(c);
            }
            free(body);
        } else {
            __ERROR_REPLY(c);
        }
    } else if (strcmp(method, "POST") == 0) {
        json_t *body = post_api_content_request(hm->body);

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
            char *skh = (char *)json_string_value(
                json_object_get(body, "sharedKeyHash"));
            char *apk = (char *)json_string_value(
                json_object_get(body, "authorizationPK"));
            char *ct = (char *)json_string_value(json_object_get(body, "ct"));

            Content *con = CreateContent(db, skh, apk, ct);

            if (con != NULL) {
                mg_http_reply(c, 200, "", "%d", con->id);
                finalize_content(con);
            } else {
                __ERROR_REPLY(c);
            }

            free(body);
        } else {
            __ERROR_REPLY(c);
        }
    } else if (strcmp(method, "PUT") == 0) {
        json_t *body = put_api_content_request(hm->body);

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
            char *skh = (char *)json_string_value(
                json_object_get(body, "sharedKeyHash"));
            char *ct = (char *)json_string_value(json_object_get(body, "ct"));

            Content *con = ReadContentBySharedKeyHash(db, skh);

            if (con != NULL) {
                set_content_content_cipher_text(con, ct);
                increment_content_nonce(con);
                UpdateContent(db, con);

                mg_http_reply(c, 200, "", "%d", con->id);

                finalize_content(con);
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
