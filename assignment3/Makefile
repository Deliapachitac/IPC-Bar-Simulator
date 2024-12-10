# Compiler and flags
CC = gcc
CFLAGS = 

# Targets
RECEPTIONIST = receptionist
MONITOR = monitor
VISITOR = visitor

# Default target
all: $(RECEPTIONIST) $(MONITOR) $(VISITOR)

# Compile receptionist.c
$(RECEPTIONIST): receptionist.c
	$(CC) $(CFLAGS) -o $(RECEPTIONIST) receptionist.c

# Compile monitor.c
$(MONITOR): monitor.c
	$(CC) $(CFLAGS) -o $(MONITOR) monitor.c

# Compile visitor.c
$(VISITOR): visitor.c
	$(CC) $(CFLAGS) -o $(VISITOR) visitor.c

# Clean up build files
clean:
	rm -f $(RECEPTIONIST) $(MONITOR) $(VISITOR)

# Phony targets
.PHONY: all clean
