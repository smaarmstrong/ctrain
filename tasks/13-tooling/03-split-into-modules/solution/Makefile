# Reference Makefile: objects, header dependencies, incremental rebuilds.
CC     = cc
CFLAGS = -std=c11 -Wall -Wextra -Werror -g

OBJS = main.o stats.o

statcalc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

main.o: main.c stats.h
stats.o: stats.c stats.h

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f statcalc $(OBJS)
