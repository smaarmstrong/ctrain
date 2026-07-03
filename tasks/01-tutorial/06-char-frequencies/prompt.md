# digit and whitespace frequencies

Read standard input to `EOF` and classify every character into exactly
one of three buckets:

- a **digit** `'0'`–`'9'` — keep a separate count for each of the ten
  digits, in an array of ten counters (hint: `c - '0'` is the index),
- **whitespace** — space, tab or newline — one combined count,
- **other** — everything else — one combined count.

Then print exactly three lines and exit with status 0:

```
digits: D0 D1 D2 D3 D4 D5 D6 D7 D8 D9
white: W
other: N
```

where `D0`…`D9` are the counts of `'0'`…`'9'` in order, separated by
single spaces, and `W` and `N` are the whitespace and other totals. Note
the single space after each colon. Empty input prints all-zero counts:

```
digits: 0 0 0 0 0 0 0 0 0 0
white: 0
other: 0
```

Example — the input `room 101, floor 3` followed by a newline gives:

```
digits: 1 2 0 1 0 0 0 0 0 0
white: 4
other: 10
```

Your program must not assume the input is text: any byte may appear, and
anything that is not a digit or one of the three whitespace characters
counts as other.

Your code goes in `main.c` in the work directory. Do the tallying with
an array and a loop — the grader also rebuilds your program with
sanitizers on, so an out-of-range index will fail the task. Grade with
`ctrain check char-frequencies`.
