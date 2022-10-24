#!/usr/bin/bash

BIN=test/build/db_path

gcc -Wall -I./include -o $BIN test/db/path.c object/util.o object/entity.o object/orm.o -lsqlite3 -ljansson
$BIN
