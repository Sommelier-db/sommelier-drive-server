#!/usr/bin/bash

BIN=test/build/db_path

gcc -Wall -O0 -I./include -I./sommelier-drive-cryptos -I../sommelier-db-build/install/include test/db/path.c object/util.o object/entity.o object/orm.o -ljansson -L./sommelier-drive-cryptos/target/release -lsommelier_drive_cryptos -L../sommelier-db-build/install/lib -lsommelier-db -o $BIN
LD_LIBRARY_PATH=../sommelier-db-build/install/lib:./sommelier-drive-cryptos/target/release $BIN
