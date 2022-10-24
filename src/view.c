#include "view.h"

char *request_method(struct mg_str s) {
    char *method = INITIALIZE_STRING(s.len + 1);
    strncpy(method, s.ptr, s.len);
    method[s.len] = '\0';

    if (VERBOSE) {
        echodebug("call request_method().");
        echodebug(method);
    }

    return method;
}

int json_has_key(json_t *j, const char *key, int json_type) {
    json_t *v = json_object_get(j, key);
    int c = v != NULL && json_typeof(v) == json_type;

    return c;
}