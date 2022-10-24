#pragma once

#include <sqlite3.h>

#include "mongoose.h"
#include "util.h"

#define ROUTER_DEFAULT_ROUTES 16
#define ROUTES_EXTEND_RATE 2

typedef void (*Route)(struct mg_connection *, struct mg_http_message *,
                      struct sqlite3 *);

typedef struct router {
    size_t max_size;
    size_t length;
    Route *routes;
    char **uris;
} Router;

Router *initialize_router();
void finalize_router(Router *);
size_t push_new_route(Router *, const char *, Route);
size_t resize_router(Router *);
char *get_uri(Router *, size_t);
Route get_route(Router *, size_t);
