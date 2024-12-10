# ./barber Q genc genb N F
# Q - number of chairs in waiting room
# genc - interval of generate customers [ms]
# genb - interval of generate of time service [ms]
# N - number of customers
# F - output filename

NAME = barber

# compiler of C language:
CC=gcc
# params
CFLAGS= -std=gnu99 -Wall -Wextra -pedantic -Werror
CLIBS= -lpthread -lrt

all: $(NAME)

.PHONY: clean
.PHONY: pack

$(NAME): $(NAME).c
	$(CC) $(CFLAGS) -o $@ $< $(CLIBS)

run:
	./barber 10 20 25 100 result

clean:
	rm -f *.o $(NAME) $(NAME).zip

pack:
	zip $(NAME).zip $(NAME).c Makefile README LICENSE.md output