# Builds the tool twice — but both binaries come out identical.
# Fix me: see prompt.md for what debug and release must each get.
CC = cc

all: app-debug app-release

app-debug: main.c
	$(CC) -std=c11 -Wall -Wextra -Werror -o app-debug main.c

app-release: main.c
	$(CC) -std=c11 -Wall -Wextra -Werror -o app-release main.c

clean:
	rm -f app-debug app-release
