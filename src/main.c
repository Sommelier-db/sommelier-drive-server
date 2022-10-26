#include <jansson.h>

#include "dbms.h"
#include "mongoose.h"
#include "orm.h"
#include "router.h"
#include "setting.h"
#include "util.h"
#include "view.h"

static const char *s_http_addr = HTTP_SERVICE_URL;  // HTTP port
// static const char *s_https_addr = "https://0.0.0.0:8443";  // HTTPS port

static Router *router;
static SommelierDBMS *dbms = NULL;

void not_found_view(struct mg_connection *c) { __ERROR_REPLY(c); }

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    int not_found = 1;

    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        if (mg_http_match_uri(hm, "/api/test")) {
            char msg[256] = "";
            char method[8] = "";
            strncpy(method, hm->method.ptr, hm->method.len);
            sprintf(msg, "{\"METHOD\": \"%s\", \"METHODLEN\": \"%ld\"}\n",
                    method, hm->method.len);
            mg_http_reply(c, 200, "", msg);
        }

        for (size_t i = 0; i < router->length; i++) {
            if (mg_http_match_uri(hm, get_uri(router, i))) {
                get_route(router, i)(c, hm, dbms);
                not_found = 0;
                break;
            }
        }

        if (not_found) {
            not_found_view(c);
            not_found = 1;
        }
    }
    (void)fn_data;
}

int main(void) {
    struct mg_mgr mgr;  // Event manager

    // int err = sqlite3_open(DBFILE, &(dbms->db));
    // if (err) {
    //     fprintf(stderr, "Can't open database: %s\n",
    //     sqlite3_errmsg(dbms->db)); sqlite3_close(dbms->db); exit(1);
    // }

    dbms = initialize_sommelier_dbms();
    OpenSommelierDB(dbms, DBFILE);

    InitalizeDatabase(dbms);
    StartTransaction(dbms);

    router = initialize_router();

    push_new_route(router, "/api/user", api_user_view);
    push_new_route(router, "/api/file-path/children", api_path_children_view);
    push_new_route(router, "/api/file-path/search", api_path_search_view);
    push_new_route(router, "/api/file-path", api_path_view);
    push_new_route(router, "/api/shared-key", api_shared_key_view);
    push_new_route(router, "/api/authorization-seed",
                   api_authorization_seed_view);
    push_new_route(router, "/api/contents", api_content_view);
    push_new_route(router, "/api/write-permission", api_write_permission_view);

    // Set log level
    mg_log_set(VERBOSE ? MG_LL_VERBOSE : (DEBUG ? MG_LL_DEBUG : MG_LL_INFO));

    mg_mgr_init(&mgr);                            // Initialise event manager
    mg_http_listen(&mgr, s_http_addr, fn, NULL);  // Create HTTP listener

    for (;;) mg_mgr_poll(&mgr, 1000);  // Infinite event loop

    mg_mgr_free(&mgr);

    finalize_router(router);

    return 0;
}
