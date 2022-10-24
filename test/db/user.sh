#!/usr/bin/bash

BIN=test/build/db_user

gcc -Wall -I./include -o $BIN test/db/user.c object/util.o object/entity.o object/orm.o -lsqlite3 -ljansson
$BIN
