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

// GET, POST /api/file-path
void api_path_view(struct mg_connection *, struct mg_http_message *, sqlite3 *);

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
