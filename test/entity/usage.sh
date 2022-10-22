#!/usr/bin/bash

SRC=test/entity/usage.c
BIN=test/build/entity_usage

gcc -Wall -I./include -o $BIN $SRC object/entity.o -lsqlite3 -ljansson
$BIN
