#pragma once

#include "util.h"

// #define errordebug(__MSG__) fprintf(stderr, "[ERROR] %s\n", __MSG__)
// #define echodebug(__MSG__) fprintf(stdout, "[DEBUG] %s\n", __MSG__)

void logging_error(char *msg);
void logging_debug(char *msg);

#if DEBUG
void debug_string_pointer(const char *, const char *);
void debug_uint_pointer(const char *, uint64_t);
void debug_void_pointer(const char *, void *);
#endif