# gdb stakeout

Your work directory contains `main.c` — a small "ledger" that replays 20
transaction records through a rolling digest. It prints only two lines:

```
processed 20 records
final digest: 519
```

Every intermediate state of the accumulator `acc` stays inside the process.
Your job is to observe one of them with a debugger, **without editing the
program**.

## The question

What is the exact value of `acc` immediately **after** the 13th record
(index 12, the value `6264`) has been mixed in?

Write that value — the full decimal number, one line, nothing else — into a
file called `answer.txt` in your work directory.

## How

Build with debug info and stake out the loop. One route (any gdb technique
is fine):

```sh
cc -std=c11 -Wall -Wextra -Werror -g -o ledger main.c
gdb ./ledger
```

Useful moves: `break` with a line number, conditions like `if i == 12`,
`next`/`step`, and `print acc`. Mind the timing — a breakpoint on the loop
line stops you *before* that iteration has run.

## Grading

- `main.c` still compiles cleanly and prints exactly the output shown above
  (leave it as you found it),
- `answer.txt` contains the correct value.

Grade with `ctrain check gdb-stakeout`.
