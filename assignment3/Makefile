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
all:  $(VISITOR) $(INITIALLIZER) #$(RECEPTIONIST) $(MONITOR)

# # Compile receptionist.c
# $(RECEPTIONIST): receptionist.c
# 	$(CC) $(CFLAGS) -o $(RECEPTIONIST) receptionist.c $(CLIBS) segment.c

# # Compile monitor.c
# $(MONITOR): monitor.c
# 	$(CC) $(CFLAGS) -o $(MONITOR) monitor.c $(CLIBS) segment.c

# Compile visitor.c
$(VISITOR): visitor.c
	$(CC) $(CFLAGS) -o $(VISITOR) visitor.c segment.c $(CLIBS) 

# Compile initiallizer.c
$(INITIALLIZER): initiallizer.c
	$(CC) $(CFLAGS) -o $(INITIALLIZER) initiallizer.c segment.c $(CLIBS) 

run:
	./initiallizer -v 30 -r 20 -s /path_to_nemea

# Clean up build files
clean:
	rm -f $(RECEPTIONIST) $(MONITOR) $(VISITOR) $(INITIALLIZER)

# Phony targets
.PHONY: all clean
