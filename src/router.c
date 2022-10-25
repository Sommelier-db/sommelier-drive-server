#include "router.h"

Router *initialize_router() {
    Router *r = INITIALIZE(Router);

    if (r == NULL) {
        errordebug("Memory allocation is failed. - Router");
        exit(1);
    }

    r->length = 0;
    r->max_size = ROUTER_DEFAULT_ROUTES;

    r->routes = INITIALIZE_SIZE(Route, ROUTER_DEFAULT_ROUTES);

    if (r->routes == NULL) {
        errordebug("Memory allocation is failed. - Router::routes");
        exit(1);
    }

    r->uris = INITIALIZE_SIZE(char *, ROUTER_DEFAULT_ROUTES);

    if (r->uris == NULL) {
        errordebug("Memory allocation is failed. - Router::uris");
        exit(1);
    }

    return r;
}

void finalize_router(Router *r) {
    for (int i = 0; i < r->length; i++) {
        free(r->uris[i]);
    }
    free(r->routes);
    free(r);
}

size_t push_new_route(Router *r, const char *uri, Route fn) {
    if (r->length == r->max_size) {
        if (VERBOSE) {
            echodebug("resize_path_vector() called. - push_new_route()");
        }

        resize_router(r);
    }

    char *str = INITIALIZE_STRING(strlen(uri) + 1);

    if (str == NULL) {
        errordebug("Memory allocation is failed. - Route::uris[i]");
        exit(1);
    }

    r->uris[r->length] = str;
    __STRING_COPY(r->uris[r->length], uri);
    r->routes[r->length] = fn;

    return ++(r->length);
}

size_t resize_router(Router *r) {
    size_t new_size = ROUTES_EXTEND_RATE * r->max_size;
    r->max_size = new_size;

    r->routes = (Route *)realloc(r->routes, new_size * sizeof(Route));
    if (r->routes == NULL) {
        errordebug("Memory re-allocation is failed. - Route::routes");
        exit(1);
    }

    r->uris = (char **)realloc(r->uris, new_size * sizeof(char *));
    if (r->uris == NULL) {
        errordebug("Memory re-allocation is failed. - Route::uris");
        exit(1);
    }

    return new_size;
}

char *get_uri(Router *r, size_t i) {
    // TODO: iのrange check
    return r->uris[i];
}

Route get_route(Router *r, size_t i) {
    // TODO: iのrange check
    return r->routes[i];
}
