SOURCE = src
OBJECT = object
BUILD = build
LOG = log
DATA = data

OBJECTS = $(OBJECT)/util.o $(OBJECT)/entity.o $(OBJECT)/orm.o $(OBJECT)/mongoose.o $(OBJECT)/router.o $(OBJECT)/views.o $(OBJECT)/main.o
LIBRARIES = -lsqlite3 -ljansson

TARGET = $(BUILD)/main
TEST_TARGETS = test/build/*
CC = gcc
# CFLAGS = -Wall -O2 -I./include
CFLAGS = -Wall -O0 -I./include

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $^ $(LIBRARIES)

$(OBJECT)/%.o: $(SOURCE)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

init:
	mkdir $(BUILD)
	mkdir $(OBJECT)
	mkdir $(LOG)
	mkdir $(DATA)

.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(TARGET) $(TEST_TARGETS)

.PHONY: clean-all
clean-all:
	rm -rf $(OBJECTS) $(TARGET) $(TEST_TARGETS) $(LOG)/* $(DATA)/*
