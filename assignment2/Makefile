# Compiler
CC = gcc

# Source files
SRCS = lexan.c 

# Header files
HEADERS = 

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = lexan

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@

# Execute the program 
run:
	./$(TARGET) -i GreatExpectations.txt -l 15 -m 15 -t 10 -e ExclusionList -o output.txt

# For the memory leaks
valgrind:
	valgrind --leak-check=full ./$(TARGET)


# Remove the object files
clean:
	rm -f $(OBJS) $(TARGET)