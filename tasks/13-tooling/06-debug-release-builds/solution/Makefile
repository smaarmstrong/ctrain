CC = cc
CFLAGS = -std=c11 -Wall -Wextra -Werror
DEBUG_FLAGS = -O0 -g -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG

all: app-debug app-release

app-debug: main.c
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $@ main.c

app-release: main.c
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o $@ main.c

.PHONY: all clean
clean:
	rm -f app-debug app-release
