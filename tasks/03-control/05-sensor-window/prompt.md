# Summarise sensor readings with break and continue

A temperature sensor produces a stream of integer readings. Write a
program that reads them from standard input (whitespace-separated,
`scanf("%d", ...)` in a loop works) and prints a summary.

Process readings in order, applying these rules to each value — in this
priority:

1. **Sentinel.** The value `9999` marks the end of the stream. Stop
   processing immediately: anything after it must be ignored, even
   further `9999`s or garbage values. (`break` is the tool.)
2. **Glitch.** A reading below `-50` or above `150` is a sensor glitch.
   Count it as skipped and move on to the next value — it must not
   touch the min/max. (`continue` is the tool.) The boundaries are
   valid: `-50` and `150` themselves are good readings.
3. **Good reading.** Anything else counts as valid and updates the
   running minimum and maximum.

Input also ends at end of file or at the first token that is not an
integer.

Then print the summary and exit 0. Always print these two lines:

```
valid: N
skipped: M
```

and, **only when there was at least one valid reading**, two more:

```
min: X
max: Y
```

Sample: input `12 -60 99 200 5 9999 42 -999` produces

```
valid: 3
skipped: 2
min: 5
max: 99
```

(`-60` and `200` are glitches; `9999` stops the stream so `42` and
`-999` are never looked at.)

Empty input prints exactly:

```
valid: 0
skipped: 0
```

Print nothing else — no prompts, no blank lines. Every line ends with a
newline.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check sensor-window`.
