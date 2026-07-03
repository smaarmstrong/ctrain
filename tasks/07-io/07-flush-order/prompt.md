# Tick-tock — buffering and fflush

C streams buffer differently depending on where they point. `stderr` is
never fully buffered — its output appears immediately. `stdout` is
line-buffered at a terminal, but **fully buffered** when redirected to a
file or pipe: nothing actually reaches the file until the buffer fills or
the program exits. So a program that writes alternately to both streams
looks fine on screen, yet produces scrambled ordering in a log file —
unless it flushes.

Write a program that emits three tick-tock pairs, alternating between the
two streams:

- to **stdout**: `tick 1`, `tick 2`, `tick 3` (each on its own line)
- to **stderr**: `tock 1`, `tock 2`, `tock 3` (each on its own line)

in the program order: tick 1, tock 1, tick 2, tock 2, tick 3, tock 3.
The program then exits with status 0. Nothing else is printed.

The behavioural requirement — this is what the grader checks:

1. stdout alone carries exactly:

   ```
   tick 1
   tick 2
   tick 3
   ```

2. stderr alone carries exactly:

   ```
   tock 1
   tock 2
   tock 3
   ```

3. When **both streams are redirected to the same file**
   (`prog >log 2>&1`), the file must read, in order:

   ```
   tick 1
   tock 1
   tick 2
   tock 2
   tick 3
   tock 3
   ```

Requirement 3 is the one a naive program fails: with stdout fully
buffered, all the tocks land in the file first and the ticks arrive in
one lump at exit. Force each tick out before its tock — `fflush(stdout)`
after each tick, or turn off stdout buffering entirely with
`setvbuf(stdout, NULL, _IONBF, 0)` before the first write. Either is
acceptable; only the observable ordering is graded.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check flush-order`.
