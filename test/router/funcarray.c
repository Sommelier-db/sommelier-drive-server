#include "util.h"

typedef void (*Func)(int);

typedef struct router {
    size_t max_size;
    size_t length;
    Func *routes;
    char **uris;
} Router;

Router *initialize_router();
void finalize_router(Router *);
size_t push_new_route(Router *, const char *, Func);
size_t resize_router(Router *);
char *get_uri(Router *, size_t);
Func get_route(Router *, size_t);

Router *initialize_router() {
    Router *r = INITIALIZE(Router);

    if (r == NULL) {
        errordebug("Memory allocation is failed. - Router");
        exit(1);
    }

    r->length = 0;
    r->max_size = 16;

    r->routes = INITIALIZE_SIZE(Func, 16);

    if (r->routes == NULL) {
        errordebug("Memory allocation is failed. - Router::routes");
        exit(1);
    }

    r->uris = INITIALIZE_SIZE(char *, 16);

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

size_t push_new_route(Router *r, const char *uri, Func fn) {
    if (r->length == r->max_size) {
        if (DEBUG) {
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
    size_t new_size = 2 * r->max_size;
    r->max_size = new_size;

    r->routes = (Func *)realloc(r->routes, new_size * sizeof(Func));
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

    char *uri = r->uris[i];

    if (VERBOSE) {
        printf("at get_uri\n");
        echodebug(uri);
    }

    return uri;
}

Func get_route(Router *r, size_t i) {
    // TODO: iのrange check

    Func route = r->routes[i];

    if (VERBOSE) {
        printf("at get_route\n");
        echodebug(get_uri(r, i));
    }

    return route;
}

static void fn1(int x) { printf("fn1 - %d\n", x); }
static void fn2(int x) { printf("fn2 - %d\n", x); }
static void fn3(int x) { printf("fn3 - %d\n", x); }
static void fn4(int x) { printf("fn4 - %d\n", x); }
static void fn5(int x) { printf("fn5 - %d\n", x); }

int main() {
    Router *r = initialize_router();

    printf("router filled: (%ld / %ld)\n", r->length, r->max_size);

    push_new_route(r, "/api/1", fn1);
    push_new_route(r, "/api/2", fn2);
    push_new_route(r, "/api/3", fn3);
    push_new_route(r, "/api/4", fn4);
    push_new_route(r, "/api/5", fn5);

    for (int i = 0; i < 3; i++) {
        printf("%d: %s\n", i, get_uri(r, i));
        printf("    ");
        get_route(r, i)((i + 1) * 42);
    }

    printf("router filled: (%ld / %ld)\n", r->length, r->max_size);

    finalize_router(r);

    return 0;
}
