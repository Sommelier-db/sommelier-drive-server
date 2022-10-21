SOURCE = src
OBJECT = object
BUILD = build
LOG = log

OBJECTS = $(OBJECT)/mongoose.o $(OBJECT)/main.o

TARGET = $(BUILD)/main
CC = gcc
# CFLAGS = -Wall -O2 -I./include
CFLAGS = -Wall -I./include

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $^

$(OBJECT)/%.o: $(SOURCE)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

init:
	mkdir $(BUILD)
	mkdir $(OBJECT)
	mkdir $(LOG)

.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(TARGET)

.PHONY: clean-all
clean-all:
	rm -rf $(OBJECTS) $(TARGET) $(LOG)/*
