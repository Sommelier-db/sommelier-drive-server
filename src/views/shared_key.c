#include "views/shared_key.h"

// GET, POST /api/shared-key
json_t *get_api_shared_key_request(struct mg_str s) {
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

json_t *post_api_shared_key_request(struct mg_str s) {
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

void api_shared_key_view(struct mg_connection *, struct mg_http_message *,
                         sqlite3 *);
