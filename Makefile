CC = gcc
CFLAGS = -Wall -Wextra -I lib/

DEBUG_TRACE_EXECUTION ?= 0 
DEBUG_PRINT_CODE ?= 0

ifeq ($(DEBUG_TRACE_EXECUTION), 1)
	CFLAGS += -DDEBUG_TRACE_EXECUTION
endif

ifeq ($(DEBUG_PRINT_CODE), 1)
	CFLAGS += -DDEBUG_PRINT_CODE
endif

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

debug: CFLAGS += -DDEBUG_TRACE_EXECUTION -DDEBUG_PRINT_CODE
debug-trace: CFLAGS += -DDEBUG_TRACE_EXECUTION
debug-print: CFLAGS += -DDEBUG_PRINT_CODE
debug: $(TARGET)
debug-trace: $(TARGET)
debug-print: $(TARGET)
.PHONY: debug
