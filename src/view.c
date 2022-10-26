#include "view.h"

char *__mg_str_split_string(struct mg_str s) {
    char *str = INITIALIZE_STRING(s.len + 1);
    strncpy(str, s.ptr, s.len);
    str[s.len] = '\0';

    return str;
}

char *request_uri(struct mg_http_message *hm) {
    return __mg_str_split_string(hm->uri);
}

char *request_method(struct mg_http_message *hm) {
    return __mg_str_split_string(hm->method);
}

int json_has_key(json_t *j, const char *key, int json_type) {
    json_t *v = json_object_get(j, key);
    int c = v != NULL && json_typeof(v) == json_type;

    return c;
}

void logging_http_body(struct mg_http_message *hm) {
    char _body[160] = "";
    strncpy(_body, hm->body.ptr, 160);
    _body[159] = '\0';

    char msg[180] = "";
    char *suffix = strlen(hm->body.ptr) > 160 ? "..." : "";
    sprintf(msg, "HTTP Body: %s%s", _body, suffix);

    logging_debug(msg);
}
