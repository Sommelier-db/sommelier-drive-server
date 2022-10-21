#!/usr/bin/bash

BIN=test/build/db_initialize

gcc -Wall -I./include -o $BIN test/db/initialize.c -lsqlite3
$BIN
