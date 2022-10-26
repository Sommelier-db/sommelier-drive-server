#include "view.h"

char *request_method(struct mg_str s) {
    char *method = INITIALIZE_STRING(s.len + 1);
    strncpy(method, s.ptr, s.len);
    method[s.len] = '\0';

    if (DEBUG) {
        char msg[30] = "";
        sprintf(msg, "Request HTTP method: %s", method);
        logging_debug(msg);
    }

    return method;
}

int json_has_key(json_t *j, const char *key, int json_type) {
    json_t *v = json_object_get(j, key);
    int c = v != NULL && json_typeof(v) == json_type;

    return c;
}
