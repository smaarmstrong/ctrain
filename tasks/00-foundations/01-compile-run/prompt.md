# The compile-run cycle

Do one full turn of the cycle yourself: write a source file, compile it,
run the binary, and check its exit status.

Write a program that prints exactly this one line to standard output:

```
source -> binary -> output
```

(the text `source -> binary -> output` followed by a single newline), and
then exits with status 0. Print nothing else.

Before grading, walk the whole cycle by hand in the work directory:

```sh
cc -Wall -o cycle main.c   # compile: source text in, binary out
./cycle                    # run the binary
echo $?                    # its exit status — expect 0
```

Then edit the text, recompile, and run again, just to feel the loop:
change the message, run `./cycle` WITHOUT recompiling (old message — the
binary is a snapshot), recompile, run again (new message). Put the exact
required line back before you grade.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check compile-run`.
