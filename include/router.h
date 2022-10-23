#include "util.h"

typedef void (*Route)();

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
