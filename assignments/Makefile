
# # Variables
# CC = gcc
# CFLAGS = -Wall -g
# OBJ = ADTList.o
# TARGET = ADTList

# # Rules
# all: $(TARGET)

# $(TARGET): $(OBJ)
# 	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# ADTList.o: ADTList.c ADTList.h 
# 	$(CC) $(CFLAGS) -c ADTList.c

# run:
# 	./$(TARGET)
# # Clean up build files
# clean:
# 	rm -f *.o $(TARGET)

# # Phony target
# .PHONY: clean all


# Compiler and flags
CC = gcc
CFLAGS = #-Wall -Wextra -g

# Source files
SRCS = ADTGraph.c ADTList.c

# Header files
HEADERS = ADTGraph.h ADTList.h

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = program

# Default rule to build the program
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .c files into .o files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
