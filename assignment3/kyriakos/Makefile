# Makefile for the project
CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm

SOURCES = parent.c child.c
OBJECTS = $(SOURCES:.c=.o)
TARGETS = parent child

all: $(TARGETS)

parent: parent.o
	$(CC) parent.o -o parent $(LDFLAGS)

child: child.o
	$(CC) child.o -o child $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(TARGETS) $(OBJECTS)

.PHONY: all clean

run: parent child
	./parent

runv: parent child
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./parent
