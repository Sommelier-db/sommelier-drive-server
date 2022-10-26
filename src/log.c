#include "log.h"

void logging_error(char *msg) {
    fprintf(stderr, "[ERROR] %s\n", msg);
    fflush(stderr);
}

void logging_debug(char *msg) {
    fprintf(stdout, "[DEBUG] %s\n", msg);
    fflush(stdout);
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
