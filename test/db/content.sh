#!/usr/bin/bash

BIN=test/build/db_content

gcc -Wall -I./include -o $BIN test/db/content.c object/util.o object/entity.o object/orm.o -lsqlite3 -ljansson
$BIN
