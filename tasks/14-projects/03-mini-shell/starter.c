#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/* msh — a minimal shell.
 *
 * TODO:
 *   1. loop: print "$ ", fflush(stdout), fgets a line; EOF -> exit 0
 *   2. tokenize on spaces/tabs (strtok is fine); blank line -> loop
 *   3. builtin exit [N]  -> return N (default 0)
 *   4. builtin cd DIR    -> chdir in THIS process; errors to stderr
 *   5. anything else     -> fork; child: execvp(argv[0], argv), and on
 *      failure print to stderr and _exit(127); parent: waitpid
 */

int main(void)
{
    char line[4096];

    for (;;) {
        fputs("$ ", stdout);
        fflush(stdout);
        if (!fgets(line, sizeof line, stdin))
            return 0;
        /* TODO: tokenize and run the command */
    }
}
