# Compiler
CC = gcc

# Source files
SRCS = lexan.c

# Header files
HEADERS = 

# Object files
OBJS = lexan.o

# Executable name
TARGET = lexan

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	gcc -o splitter splitter.c
	$(CC) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@

# Execute the program  GreatExpectations.txt
run:
	./$(TARGET) -i GreatExpectations.txt -l 5 -m 5 -t 10 -e ExclusionList.txt -o output.txt

# For the memory leaks
valgrind:
	valgrind --leak-check=full ./$(TARGET)


# Remove the object files
clean:
	rm -f $(OBJS) $(TARGET)