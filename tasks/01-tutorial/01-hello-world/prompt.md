# hello, world

Every C career opens the same way. Write a program that prints exactly one
line to standard output:

```
hello, world
```

— the twelve characters `hello, world` followed by a single newline — and
then exits with status 0.

Print nothing else: no prompt text, no extra blank lines, no trailing
spaces.

Before grading, build and run it yourself to see the whole toolchain once:

```sh
cc -o hello main.c    # compile main.c into an executable named hello
./hello               # run it
echo $?               # show the exit status of the last command — expect 0
```

The value your `main` returns *is* that exit status; `return 0;` means
"succeeded" to the shell and to every script that ever calls your program.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check hello-world`.
