SOURCE = src
OBJECT = object
BUILD = build

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

.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(TARGET)
