CC = gcc
CFLAGS = -Wall -Wextra -I lib/
SRCS = main.c chunk.c debug.c memory.c value.c vm.c scanner.c compiler.c
OBJS = $(SRCS:%.c=build/%.o)
TARGET = build/clox

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: %.c | build
	$(CC) $(CFLAGS) -c -o $@ $<

build:
	mkdir -p build

clean:
	rm -rf build
