#!/usr/bin/bash

BIN=test/build/router_usage

gcc -Wall -I./include -o $BIN test/router/usage.c object/util.o object/mongoose.o object/router.o -ljansson
$BIN
