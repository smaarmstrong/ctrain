# ctrain — build the trainer, then learn C with it.
#
#   make            build ./ctrain
#   make selftest   verify every grader (solution passes, starter fails)
#   make clean

CC      ?= cc
CFLAGS  ?= -std=c11 -Wall -Wextra -Werror -O2

ctrain: src/ctrain.c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: selftest clean
selftest: ctrain
	./selftest.sh

clean:
	rm -f ctrain
