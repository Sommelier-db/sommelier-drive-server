#include "views.h"

#include "mongoose.h"
#include "util.h"

char *_mg_method(struct mg_str s) {
    char *method = INITIALIZE_STRING(s.len + 1);
    strncpy(method, s.ptr, s.len);
    method[s.len] = '\0';
    return method;
}

void main_view(struct mg_connection *c, struct mg_http_message *hm) {
    mg_http_reply(c, 200, "", "{\"result\": \"42\"}\n");
}

void api_users_view(struct mg_connection *c, struct mg_http_message *hm) {
    char *method = _mg_method(hm->method);

    if (strcmp(method, "GET") == 0) {
        mg_http_reply(c, 200, "", "{\"result\": \"42\"}\n");
    } else if (strcmp(method, "POST") == 0) {
        mg_http_reply(c, 200, "", "{\"result\": \"84\"}\n");
    } else {
        mg_http_reply(c, 500, "", "{\"msg\": \"internal error\"}\n");
        exit(1);
    }

    free(method);
}
