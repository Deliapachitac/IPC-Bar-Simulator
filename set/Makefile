# Makefile for the mycopy program

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2

# Target executable name
TARGET = mycopy

# Source files
SRC = mycopy.c

# Object files
OBJ = $(SRC:.c=.o)

# Default target to build the program
all: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)
	./$(TARGET) del del1

run:
	./$(TARGET) del del1

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJ) $(TARGET)

# Phony targets
.PHONY: all clean
