# Reference Makefile: variables, pattern rule, .PHONY clean.
CC     = cc
CFLAGS = -std=c11 -Wall -Wextra -Werror -g

OBJS = main.o ops.o

calc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c ops.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f calc $(OBJS)
