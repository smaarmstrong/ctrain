# gdb first steps

The starter `main.c` computes two factorials. It compiles cleanly — and
segfaults. Diagnose it with the three-command gdb session from the
lesson before you touch the code:

```sh
cc -g -Wall -Werror -o fact main.c
gdb ./fact
(gdb) run
(gdb) bt        # read frame #0 and the shape of the stack, then q the pager
(gdb) quit
```

The backtrace's shape tells you what kind of bug this is; frame #0 tells
you the line. Fix it (the fix is one character), so that the program
prints exactly:

```
5! = 120
7! = 5040
```

and exits with status 0.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check gdb-first-steps`.
