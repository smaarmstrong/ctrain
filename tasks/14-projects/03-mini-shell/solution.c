#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_TOKENS 64

int main(void)
{
    char line[4096];

    for (;;) {
        fputs("$ ", stdout);
        fflush(stdout);
        if (!fgets(line, sizeof line, stdin))
            return 0;

        char *args[MAX_TOKENS];
        int n = 0;
        char *tok = strtok(line, " \t\n");
        while (tok && n < MAX_TOKENS - 1) {
            args[n++] = tok;
            tok = strtok(NULL, " \t\n");
        }
        args[n] = NULL;
        if (n == 0)
            continue;

        if (strcmp(args[0], "exit") == 0)
            return n > 1 ? atoi(args[1]) : 0;

        if (strcmp(args[0], "cd") == 0) {
            if (n != 2)
                fprintf(stderr, "msh: cd: expected exactly one argument\n");
            else if (chdir(args[1]) != 0)
                fprintf(stderr, "msh: cd: %s: cannot change directory\n",
                        args[1]);
            continue;
        }

        fflush(stdout);
        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "msh: fork failed\n");
            continue;
        }
        if (pid == 0) {
            execvp(args[0], args);
            fprintf(stderr, "msh: %s: command not found\n", args[0]);
            _exit(127);
        }
        int status;
        waitpid(pid, &status, 0);
    }
}
