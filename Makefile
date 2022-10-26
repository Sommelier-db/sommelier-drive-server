SOURCE = src
OBJECT = object
BUILD = build
LOG = log
DATA = data

VIEWS = $(OBJECT)/views/authorization_seed.o $(OBJECT)/views/content.o $(OBJECT)/views/path.o $(OBJECT)/views/shared_key.o $(OBJECT)/views/user.o $(OBJECT)/views/write_permission.o
OBJECTS = $(OBJECT)/util.o $(OBJECT)/entity.o $(OBJECT)/orm.o $(OBJECT)/mongoose.o $(OBJECT)/router.o $(OBJECT)/view.o $(VIEWS) $(OBJECT)/main.o
LIBRARIES = -ljansson -L./sommelier-drive-cryptos/target/release -lsommelier_drive_cryptos -L../sommelier-db-build/install/lib -lsommelier-db

TARGET = $(BUILD)/main
TEST_TARGETS = test/build/*
CC = gcc
# CFLAGS = -Wall -O2 -I./include
CFLAGS = -Wall -O0 -I./include -I./sommelier-drive-cryptos -I../sommelier-db-build/install/include

$(TARGET): $(OBJECTS)
	$(CC) $^ $(LIBRARIES) -o $(TARGET) 

$(OBJECT)/%.o: $(SOURCE)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

init:
	mkdir $(BUILD)
	mkdir $(OBJECT)
	mkdir $(OBJECT)/views
	mkdir $(LOG)
	mkdir $(DATA)

.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(TARGET) $(TEST_TARGETS)

.PHONY: clean-all
clean-all:
	rm -rf $(OBJECTS) $(TARGET) $(TEST_TARGETS) $(LOG)/* $(DATA)/*

.PHONY: exec
exec: $(TARGET)
	LD_LIBRARY_PATH=./sommelier-drive-cryptos/target/release/:../sommelier-db-build/install/lib $(TARGET)
