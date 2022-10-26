#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"

#define DEBUG (0 || VERBOSE)
#define VERBOSE 0

#define refsizeof(__TYPE__) (sizeof(__TYPE__ *))
#define INITIALIZE(__TYPE__) ((__TYPE__ *)malloc(sizeof(__TYPE__)))
#define INITIALIZE_SIZE(__TYPE__, __SIZE__) \
    ((__TYPE__ *)malloc(__SIZE__ * sizeof(__TYPE__)))
#define INITIALIZE_STRING(__SIZE__) (INITIALIZE_SIZE(char, __SIZE__))

#define __STRING_COPY(__DST__, __SRC__) \
    (strncpy(__DST__, __SRC__, strlen(__SRC__) + 1))

#define AS_U64(s) ((uint64_t)atoi(s))

char *alloc_string(size_t);
char *copy_string(char *dst, const char *src);

char *alloc_string_debug(size_t, const char *field);
