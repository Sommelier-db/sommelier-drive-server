#!/usr/bin/bash

BIN=test/build/entity_json

gcc -Wall -I./include -o $BIN test/entity/json.c -lsqlite3 -ljansson
$BIN
