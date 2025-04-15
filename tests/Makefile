# Makefile for the mycopy program

# Compiler
CC = gcc

# Compiler flags
CFLAGS = #-Wall -Wextra -O2

# Target executable name
TARGET = sept2024

# Source files
SRC = sept2024.c

# Object files
OBJ = $(SRC:.c=.o)

# Default target to build the program
all: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

run:
	./$(TARGET)
	
# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJ) $(TARGET)

