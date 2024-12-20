# Compiler and flags
CC = gcc
CFLAGS= 
CLIBS= -lpthread -lrt

# Targets
RECEPTIONIST = receptionist
MONITOR = monitor
VISITOR = visitor
INITIALLIZER = initiallizer

# Default target
all:  $(VISITOR) $(INITIALLIZER) $(RECEPTIONIST) #$(MONITOR)

# Compile receptionist.c
$(RECEPTIONIST): receptionist.c
	$(CC) $(CFLAGS) -o $(RECEPTIONIST) receptionist.c segment.c $(CLIBS) 

# # Compile monitor.c
# $(MONITOR): monitor.c
# 	$(CC) $(CFLAGS) -o $(MONITOR) monitor.c segment.c $(CLIBS) 

# Compile visitor.c
$(VISITOR): visitor.c
	$(CC) $(CFLAGS) -g -o $(VISITOR) visitor.c segment.c $(CLIBS) 

# Compile initiallizer.c
$(INITIALLIZER): initiallizer.c
	$(CC) $(CFLAGS) -g -o $(INITIALLIZER) initiallizer.c segment.c $(CLIBS) 

run:
	./initiallizer -v 5 -r 6 -s /path_to_nemea

# Clean up build files
clean:
	rm -f $(RECEPTIONIST) $(MONITOR) $(VISITOR) $(INITIALLIZER)

# Phony targets
.PHONY: all clean
