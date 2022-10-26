#include "util.h"

char *alloc_string(size_t size) {
    char *p = INITIALIZE_STRING(size);

    if (p == NULL) {
        char msg[128] = "";
        sprintf(msg, "Memory allocation is failed. - alloc_string(size=%ld)",
                size);
        logging_error(msg);

        exit(1);
    }

    return p;
}

char *copy_string(char *dst, const char *src) {
    char *ret = __STRING_COPY(dst, src);

    if (VERBOSE) {
        debug_string_pointer("ret (copy_string)", ret);
    }

    return ret;
}

char *alloc_string_debug(size_t size, const char *field) {
    char *p = INITIALIZE_STRING(size);

    if (VERBOSE) {
        char _msg[128] = "";
        sprintf(_msg, "p (alloc_string_debug: %s)", field);
        debug_string_pointer(_msg, p);
    }

    if (p == NULL) {
        char *msg = (char *)malloc(strlen(field) + 64);
        sprintf(msg, "Memory allocation is failed. - %s", field);
        logging_error(msg);

        free(msg);
        exit(1);
    }

    return p;
}
