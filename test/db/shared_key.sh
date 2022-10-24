#!/usr/bin/bash

BIN=test/build/db_shared_key

gcc -Wall -I./include -o $BIN test/db/shared_key.c object/util.o object/entity.o object/orm.o -lsqlite3 -ljansson
$BIN
