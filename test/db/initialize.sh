#!/usr/bin/bash

BIN=test/build/db_initialize

gcc -Wall -I./include -o $BIN test/db/initialize.c object/util.o object/orm.o -lsqlite3
$BIN
