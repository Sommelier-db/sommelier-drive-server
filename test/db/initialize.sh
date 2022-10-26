#!/usr/bin/bash

SRC=test/db/initialize.c
BIN=test/build/db_initialize
OBJECTS="object/log.o object/util.o object/entity.o object/dbms.o object/orm.o"
C_UTIL_FLAGS="-Wall -O0"
C_INCLUDE_FLAGS="-I./include -I./sommelier-drive-cryptos -I../sommelier-db-build/install/include"
C_LINK_FLAGS="-ljansson -L./sommelier-drive-cryptos/target/release -lsommelier_drive_cryptos -L../sommelier-db-build/install/lib -lsommelier-db"
RUNTIME_PATH="../sommelier-db-build/install/lib:./sommelier-drive-cryptos/target/release"

gcc $C_UTIL_FLAGS $C_INCLUDE_FLAGS $SRC $OBJECTS $C_LINK_FLAGS -o $BIN
LD_LIBRARY_PATH=$RUNTIME_PATH $BIN
