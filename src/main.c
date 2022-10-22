#include <jansson.h>

#include "mongoose.h"
#include "setting.h"
#include "util.h"

static const char *s_http_addr = HTTP_SERVICE_URL;  // HTTP port
// static const char *s_https_addr = "https://0.0.0.0:8443";  // HTTPS port
static const char *s_root_dir = ".";

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    // setting for HTTPS is unable.
    // if (ev == MG_EV_ACCEPT && fn_data != NULL) {
    //     struct mg_tls_opts opts = {
    //         //.ca = "ca.pem",         // Uncomment to enable two-way SSL
    //         .cert = "server.pem",     // Certificate PEM file
    //         .certkey = "server.pem",  // This pem contains both cert and key
    //     };
    //     mg_tls_init(c, &opts);
    // } else
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        if (mg_http_match_uri(hm, "/api/stats")) {
            // Print some statistics about currently established connections
            mg_printf(c,
                      "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
            mg_http_printf_chunk(c,
                                 "ID PROTO TYPE      LOCAL           REMOTE\n");
            for (struct mg_connection *t = c->mgr->conns; t != NULL;
                 t = t->next) {
                char loc[40], rem[40];
                mg_http_printf_chunk(c, "%-3lu %4s %s %-15s %s\n", t->id,
                                     t->is_udp ? "UDP" : "TCP",
                                     t->is_listening  ? "LISTENING"
                                     : t->is_accepted ? "ACCEPTED "
                                                      : "CONNECTED",
                                     mg_straddr(&t->loc, loc, sizeof(loc)),
                                     mg_straddr(&t->rem, rem, sizeof(rem)));
            }
            mg_http_printf_chunk(c, "");  // Don't forget the last empty chunk
        } else if (mg_http_match_uri(hm, "/api/f2/*")) {
            mg_http_reply(c, 200, "", "{\"result\": \"%.*s\"}\n",
                          (int)hm->uri.len, hm->uri.ptr);
        } else {
            struct mg_http_serve_opts opts = {.root_dir = s_root_dir};
            mg_http_serve_dir(c, ev_data, &opts);
        }
    }
    (void)fn_data;
}

int main(void) {
    struct mg_mgr mgr;  // Event manager

    // Set log level
    mg_log_set(VERBOSE ? MG_LL_VERBOSE : (DEBUG ? MG_LL_DEBUG : MG_LL_INFO));

    mg_mgr_init(&mgr);                            // Initialise event manager
    mg_http_listen(&mgr, s_http_addr, fn, NULL);  // Create HTTP listener
    // mg_http_listen(&mgr, s_https_addr, fn, (void *)1);  // HTTPS listener

    for (;;) mg_mgr_poll(&mgr, 1000);  // Infinite event loop

    mg_mgr_free(&mgr);

    return 0;
}
