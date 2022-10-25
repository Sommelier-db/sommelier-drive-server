#!/usr/bin/bash

BIN=test/build/db_write_permission

gcc -Wall -I./include test/db/write_permission.c object/util.o object/entity.o object/orm.o -lsqlite3 -ljansson -o $BIN 
$BIN
