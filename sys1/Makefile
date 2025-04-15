# Compiler
CC = gcc

# Source files
SRCS = fss_manager.c  List.c

# Object files
OBJS = $(SRCS:.c=.o) 

# Executable name
TARGET = fss_manager

# Rule to link object files and create the executable
# gcc -o splitter splitter.c Hash.c
# gcc -o builder builder.c Hash.c

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@

# Execute the program  GreatExpectations.txt
run:
	./$(TARGET) -l logfile -c configfile -n 10 

# For the memory leaks
valgrind:
	valgrind --leak-check=full ./$(TARGET)


# Remove the object files
clean:
	rm -f $(OBJS) $(TARGET)