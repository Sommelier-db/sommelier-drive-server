#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 1
#define VERBOSE 0

#define refsizeof(__TYPE__) (sizeof(__TYPE__ *))
#define INITIALIZE(__TYPE__) ((__TYPE__ *)malloc(sizeof(__TYPE__)))
#define INITIALIZE_SIZE(__TYPE__, __SIZE__) \
    ((__TYPE__ *)malloc(__SIZE__ * sizeof(__TYPE__)))
#define INITIALIZE_STRING(__SIZE__) (INITIALIZE_SIZE(char, __SIZE__))

#define errordebug(__MSG__) fprintf(stderr, "[ERROR] %s\n", __MSG__)
#define echodebug(__MSG__) fprintf(stdout, "[DEBUG] %s\n", __MSG__)

#define __STRING_COPY(__DST__, __SRC__) \
    (strncpy(__DST__, __SRC__, strlen(__SRC__) + 1))

#define AS_U64(s) ((uint64_t)atoi(s))

char *initialize_string(size_t, const char *field);
char *safe_string_copy(char *dst, const char *src);

#if DEBUG
void debug_string_pointer(const char *, const char *);
void debug_uint_pointer(const char *, uint64_t);
void debug_void_pointer(const char *, void *);
#endif
