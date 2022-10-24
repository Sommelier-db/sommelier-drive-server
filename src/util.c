#include "util.h"

char *initialize_string(const char *field) {
    char *p = INITIALIZE_STRING(1024);

    if (VERBOSE) {
        char _msg[128] = "";
        sprintf(_msg, "p (initialize_string: %s)", field);
        debug_string_pointer(_msg, p);
    }

    if (p == NULL) {
        char *msg = (char *)malloc(strlen(field) + 64);
        sprintf(msg, "Memory allocation is failed. - %s", field);
        errordebug(msg);
        free(msg);
        exit(1);
    }

    return p;
}

char *safe_string_copy(char *dst, const char *src) {
    if (VERBOSE) {
        debug_string_pointer("dst (safe_string_copy)", dst);
    }

    if (VERBOSE) {
        size_t msgsize = 64 + strlen(src) + strlen(dst);
        char *msg = (char *)malloc(msgsize * sizeof(char));
        sprintf(msg, "src: %ld - dst: %ld", strlen(src), strlen(dst));
        echodebug(msg);
    }

    // FIXME: 適切なresizeの実装

    // if (strlen(dst) < strlen(src) + 1) {
    //     if (VERBOSE) {
    //         echodebug("re-allocation occured.");
    //     }

    //     dst = (char *)realloc(dst, (strlen(src) + 1) * sizeof(char));
    // }

    char *ret = __STRING_COPY(dst, src);
    if (VERBOSE) {
        debug_string_pointer("ret (safe_string_copy)", ret);
    }
    return ret;
}

#if DEBUG
void debug_string_pointer(const char *field, const char *s) {
    printf("[DEBUG] debug char *: %s = \"%s\" (@ %p)\n", field, s, s);
    fflush(stdout);
}

void debug_uint_pointer(const char *field, uint64_t id) {
    printf("[DEBUG] debug int %s = %ld (%p)\n", field, id, &id);
    fflush(stdout);
}

void debug_void_pointer(const char *field, void *p) {
    printf("[DEBUG] debug void * %s @ %p\n", field, p);
    fflush(stdout);
}
#endif
