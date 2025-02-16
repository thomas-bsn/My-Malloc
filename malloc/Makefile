CC = gcc
CPPFLAGS = -D_DEFAULT_SOURCE
CFLAGS = -Wall -Wextra -Werror -std=c99 -fPIC -fno-builtin -Wvla
LDFLAGS = -shared
VPATH = src

TARGET_LIB = libmalloc.so
OBJS = malloc.o bucket.o

all: library

library: $(TARGET_LIB)
$(TARGET_LIB): CFLAGS += -pedantic -fvisibility=hidden
$(TARGET_LIB): LDFLAGS += -Wl,--no-undefined
$(TARGET_LIB): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^
	cp libmalloc.so tests/ -r

debug: CFLAGS += -g
debug: clean $(TARGET_LIB)

clean:
	$(RM) $(TARGET_LIB) tests/$(TARGET_LIB) $(OBJS) tests/testsuites1 && clear

check: debug
	$(CC) tests/test1.c $(CFLAGS) $(OBJS) -o tests/testsuites1 $(TARGET_LIB) -g
	./tests/test.sh

.PHONY: all $(TARGET_LIB) clean
