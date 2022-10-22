SOURCE = src
OBJECT = object
BUILD = build
LOG = log
DATA = data

OBJECTS = $(OBJECT)/entity.o $(OBJECT)/orm.o $(OBJECT)/mongoose.o $(OBJECT)/main.o
LIBRARIES = -lsqlite3 -ljansson

TARGET = $(BUILD)/main
CC = gcc
# CFLAGS = -Wall -O2 -I./include
CFLAGS = -Wall -I./include

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
	rm -rf $(OBJECTS) $(TARGET)

.PHONY: clean-all
clean-all:
	rm -rf $(OBJECTS) $(TARGET) $(LOG)/* $(DATA)/*
