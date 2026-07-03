# sort lines

Write a whole program (with `main`) that reads every line of standard
input, sorts the lines, and prints them.

Behaviour:

- A line is a run of characters ended by `'\n'`. If the input ends
  without a final `'\n'`, the trailing characters still count as one
  last line.
- Sort ascending by plain byte comparison — exactly the order
  `strcmp` gives. Do not fold case, do not use locale collation.
- Duplicate lines are all kept (their relative order does not matter —
  they are identical anyway).
- Empty lines are real lines and sort first.
- Print each sorted line followed by one `'\n'`, and nothing else.
  Empty input produces no output at all.
- Exit status 0.

The point of the exercise is the data structure: store each line in
its own allocated buffer and sort an **array of pointers** to those
buffers — the sort swaps pointers, never copies text. Don't guess a
fixed line length; grow buffers as needed (test lines stay under a few
hundred characters and inputs under 100 lines, but hard-coding those
numbers is the wrong habit — allocate and grow).

Free everything you allocate: the grader re-runs the program under
AddressSanitizer, and a leak fails the task even when the output is
right.

Example — input:

```
pear
apple

fig
```

output:

```

apple
fig
pear
```

You may use `strcmp` and `qsort` if you wish; any correct
implementation passes.

Grade with `ctrain check sort-lines`.
