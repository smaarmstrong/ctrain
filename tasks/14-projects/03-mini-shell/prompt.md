# msh — a minimal shell

Write `msh`, a command interpreter small enough to grade by piping a
scripted session into it. It must behave identically whether stdin is a
terminal or a pipe.

## The loop

1. Print the prompt — exactly the two characters `$ ` (dollar, space) —
   to **stdout**, and flush stdout before reading.
2. Read one line from stdin (lines are at most 4096 characters).
   On end-of-file, exit immediately with status `0` (no extra output).
3. Split the line into tokens on spaces and tabs (no quoting rules; a
   line has at most 63 tokens). A blank line just loops back to step 1.
4. Run the command (below), wait for it to finish completely, then loop.

Because the prompt goes to stdout too, a session like `echo hello`
followed by end-of-file produces exactly this on stdout:

```
$ hello
$ 
```

(prompt, the command's output, then the prompt printed once more before
the EOF is seen — note there is no newline after the final `$ `).

## Builtins (run in the shell process itself — no fork)

- `exit` — terminate the shell with status `0`;
  `exit N` — terminate with status `N` (N is a small decimal number).
- `cd DIR` — change the shell's own working directory, so it affects
  every later command. If the directory cannot be entered, or `cd` is
  given no argument or more than one, print a one-line message to
  **stderr** and keep the shell running.

## Everything else

Fork a child; in the child, `execvp` the first token with the full
token list as its argv. The parent waits for the child before printing
the next prompt (so output never interleaves). If `execvp` fails, the
child prints one line to **stderr** and exits with status `127` — the
shell itself carries on and must still run later commands normally.

## Rules the grader relies on

- stdout carries **only** the prompts and whatever the executed
  commands print; every diagnostic goes to stderr.
- Flush stdout before every fork and before every read, or your output
  will appear in the wrong order when stdout is a pipe.
- The shell's final exit status: `N` from `exit N`, `0` from a bare
  `exit` or from end-of-file.

Your code goes in `main.c` (you may split it into several `.c` files;
all of them are compiled together). You will need
`#define _POSIX_C_SOURCE 200809L` before the includes for `fork`,
`execvp`, `waitpid` and `chdir` under `-std=c11`. Grade with
`ctrain check mini-shell`.
