#include "router.h"

static void sample_func1(struct mg_connection *c, struct mg_http_message *hm) {
    mg_http_reply(c, 200, "", "{\"r1\": \"%.*s\"}\n", (int)hm->uri.len,
                  hm->uri.ptr);
}

static void sample_func2(struct mg_connection *c, struct mg_http_message *hm) {
    mg_http_reply(c, 200, "", "{\"r2\": \"%.*s\"}\n", (int)hm->uri.len,
                  hm->uri.ptr);
}

int main() {
    Route *rs = (Route *)malloc(sizeof(Route) * 2);
    rs[0] = sample_func1;
    rs[1] = sample_func2;

    Router *r = initialize_router();

    printf("router filled: (%ld / %ld)\n", r->length, r->max_size);

    char uri[10] = "";

    for (int i = 0; i < 300; i++) {
        sprintf(uri, "/api/%d", i + 1);
        push_new_route(r, uri, sample_func1);
    }

    for (int i = 0; i < 3; i++) {
        printf("%d: %s\n", i, get_uri(r, i));
    }

    printf("router filled: (%ld / %ld)\n", r->length, r->max_size);

    finalize_router(r);

    return 0;
}
