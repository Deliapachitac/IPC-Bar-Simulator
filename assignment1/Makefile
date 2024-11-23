# Compiler
CC = gcc

# Source files
SRCS = miris.c Graph.c Hash.c List.c 

# Header files
HEADERS = Graph.h Hash.h List.h 

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = miris

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@

# Execute the program 
run:
	./$(TARGET) -i inputfile -o outputfile

# For the memory leaks
valgrind:
	valgrind --leak-check=full ./$(TARGET) -i inputfile -o outputfile


# Remove the object files
clean:
	rm -f $(OBJS) $(TARGET)