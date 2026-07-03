# Classify exam scores into bands

Write a program that reads integer exam scores from standard input until
end of file (whitespace-separated — `scanf("%d", ...)` in a loop works)
and prints one classification line per score, in input order.

For each score `s`:

- If `s` is below 0 or above 100, the score is out of range. Print:

  ```
  s: invalid
  ```

- Otherwise print the letter band followed by a pass/fail marker:

  | range   | letter |
  |---------|--------|
  | 90–100  | A      |
  | 80–89   | B      |
  | 70–79   | C      |
  | 60–69   | D      |
  | 0–59    | F      |

  A score of 60 or more is a pass. The line format is exactly:

  ```
  s: L (pass)
  ```

  or

  ```
  s: L (fail)
  ```

An `if / else if / ... / else` chain is the natural shape for the banding;
the pass/fail marker is a one-line job for the conditional operator
(`cond ? a : b`). The grader only checks output, so use whichever you
like — but try both.

Sample: input `85 100 59 101 -3` produces

```
85: B (pass)
100: A (pass)
59: F (fail)
101: invalid
-3: invalid
```

Print nothing else — no prompts, no summary, no output at all for empty
input. Every printed line ends with a newline.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check score-bands`.
