# # compiler
# CC = gcc


# # Object files
# OBJS = mygraph.o ADTList.o 

# # Το εκτελέσιμο πρόγραμμα
# EXEC = mygraph

# # Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
# CFLAGS = -Wall -Werror -g -I 
# LDFLAGS = -lm

# $(EXEC): $(OBJS)
# 	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

# clean:
# 	rm -f $(OBJS) $(EXEC)

# run: $(EXEC)
# 	./$(EXEC) 

# valgrind: $(EXEC)
# 	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all ./$(EXEC) $(ARGS)


# Variables
CC = gcc
CFLAGS = -Wall -g
OBJ = ADTList.o
TARGET = ADTList

# Rules
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

ADTList.o: ADTList.c ADTList.h 
	$(CC) $(CFLAGS) -c ADTList.c

run:
	./$(TARGET)
# Clean up build files
clean:
	rm -f *.o $(TARGET)

# Phony target
.PHONY: clean all
