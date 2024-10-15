# Object files
OBJ = $(SRC:.c=.o)

# Default target to build the program
all: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)
	./$(TARGET)

run:
	./$(TARGET)

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJ) $(TARGET)

