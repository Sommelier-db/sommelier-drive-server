#include "util.h"

char *safe_string_copy(char *dst, const char *src) {
    if (VERBOSE) {
        size_t msgsize = 64 + strlen(src) + strlen(dst);
        char *msg = (char *)malloc(msgsize * sizeof(char));
        sprintf(msg, "src: %ld - dst: %ld", strlen(src), strlen(dst));
        debug(msg);
    }

    if (strlen(dst) < strlen(src)) {
        if (VERBOSE) {
            debug("re-allocation occured.");
        }

        dst = (char *)realloc(dst, strlen(src) + 1);
    }

    __STRING_COPY(dst, src);

    return dst;
}
