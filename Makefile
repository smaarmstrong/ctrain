# ctrain — build the trainer, then learn C with it.
#
# Bare `make` just lists the targets (it changes nothing). Building the runner
# is lesson zero — any practice verb builds it on demand, or do it explicitly:
#
#     make ctrain     build ./ctrain (the trainer, written in C11)
#
# New to the material?  Let it teach you first:
#
#     make learn      teach the next task, then set it up so you can try
#     ...edit work/<task>/main.c...
#     make check      grade it   (then `make learn` again for the next one)
#
# Already know the ropes?  Just practise:
#
#     make train      pick what you should do next (new material, or a review)
#     make check      grade the task you are currently on
#
# `make train` decides on its own whether to give you new material (in a
# fundamentals-first order) or bring back something older for a spaced-
# repetition review. These verbs forward to ./ctrain — run `./ctrain help`
# for the full CLI (start/reset/solution and per-task ids).

CC      ?= cc
CFLAGS  ?= -std=c11 -Wall -Wextra -Werror -O2

CTRAIN  := ./ctrain
.DEFAULT_GOAL := help

ctrain: src/ctrain.c src/sr.h src/lesson.h
	@command -v $(CC) >/dev/null 2>&1 || { \
	  printf 'ctrain needs a C compiler ($(CC)) and it is not installed.\n' >&2; \
	  printf '  Rocky/RHEL:  sudo dnf group install "Development Tools"\n' >&2; \
	  printf '  (or set CC=clang if you have clang instead)\n' >&2; \
	  exit 1; }
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: help learn train next check solution list status cli selftest test clean

help:
	@printf 'ctrain — just run one of:\n\n'
	@printf '  make learn      teach the next task, then set it up to try\n'
	@printf '  make train      pick the next task for you (new material, or a review)\n'
	@printf '  make check      grade the task you are currently on\n'
	@printf '  make solution   reveal the reference solution for it\n'
	@printf '  make list       every task, grouped by chapter, with your status\n'
	@printf '  make status     your XP, streak and completion\n'
	@printf '  make ctrain     build the runner;  make test / make selftest to verify\n\n'
	@printf 'Full CLI (start/reset a specific task by id):  %s help\n' '$(CTRAIN)'

learn:    ctrain ; @$(CTRAIN) learn
train:    ctrain ; @$(CTRAIN) train
next:     ctrain ; @$(CTRAIN) train
check:    ctrain ; @$(CTRAIN) check
solution: ctrain ; @$(CTRAIN) solution
list:     ctrain ; @$(CTRAIN) list
status:   ctrain ; @$(CTRAIN) status
cli:      ctrain ; @$(CTRAIN) help

# Verify every task's grader (solution passes, starter fails).
selftest: ctrain
	./selftest.sh

# Stdlib-only unit tests for the selection / spaced-repetition / lesson logic.
test:
	$(CC) $(CFLAGS) -o tests/test_selection tests/test_selection.c
	./tests/test_selection

clean:
	rm -f ctrain tests/test_selection
