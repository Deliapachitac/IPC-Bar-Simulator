
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


# # Compiler and flags
# CC = gcc
# CFLAGS = #-Wall -Wextra -g

# # Source files
# SRCS = ADTGraph.c ADTList.c

# # Header files
# HEADERS = ADTGraph.h ADTList.h

# # Object files
# OBJS = $(SRCS:.c=.o)

# # Executable name
# TARGET = program

# # Default rule to build the program
# all: $(TARGET)

# # Rule to link object files and create the executable
# $(TARGET): $(OBJS)
# 	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# # Rule to compile .c files into .o files
# %.o: %.c $(HEADERS)
# 	$(CC) $(CFLAGS) -c $< -o $@

# # Rule to clean up object files and the executable
# clean:
# 	rm -f $(OBJS) $(TARGET)

# # Phony targets
# .PHONY: all clean

# //?///////////////////
# Compiler and flags
CC = gcc
CFLAGS = #-Wall -Wextra -g

# Source files
SRCS = myprogram.c ADTGraph.c ADTHash.c ADTList.c # 

# Header files
HEADERS = ADTGraph.h ADTHash.h ADTList.h # 

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = miris

# Default rule to build the program
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .c files into .o files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

#
run:
	./$(TARGET) -i inputfile -o outputfile
# Remove the object files
clean:
	rm -f $(OBJS) $(TARGET)

# ?///////////////////////////////////////////////////////////
# # Compiler and flags
# CC = gcc
# CFLAGS = #-Wall -Wextra -std=c11

# # Executable name
# EXEC = myprogram

# # Source files
# SRCS = myprogram.c ADTGraph.c ADTHash.c ADTList.c

# # Object files (compiled .o files from .c files)
# OBJS = $(SRCS:.c=.o)

# # Header files (for dependencies)
# HEADERS = ADTGraph.h ADTHash.h ADTList.h

# # Default target to build the executable
# all: $(EXEC)

# # Rule to link object files into the final executable
# $(EXEC): $(OBJS)
# 	$(CC) $(CFLAGS) -o $@ $(OBJS)

# run:
# 	./$(EXEC) -i inputfile -o outputfile

# # Rule to compile .c files into .o files
# %.o: %.c $(HEADERS)
# 	$(CC) $(CFLAGS) -c $<

# # Clean up object files and executable
# clean:
# 	rm -f $(OBJS) $(EXEC)


