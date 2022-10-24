#!/usr/bin/bash

BIN=test/build/router_funcarray

gcc -Wall -I./include -o $BIN test/router/funcarray.c object/util.o
$BIN
