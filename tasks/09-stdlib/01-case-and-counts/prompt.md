# character classes and case swapping

Write a program that reads its entire standard input byte by byte and:

1. **Echoes the input with the case of every letter swapped** — uppercase
   letters become lowercase, lowercase become uppercase, and every other
   byte (digits, punctuation, whitespace, bytes above 127) is echoed
   unchanged.
2. **After the echoed text**, prints one summary line:

   ```
   letters=L digits=D spaces=S other=O
   ```

   where the four counters classify every input byte with exactly one
   category, tested in this order:

   - `letters` — `isalpha`
   - `digits`  — `isdigit`
   - `spaces`  — `isspace` (this includes newlines and tabs)
   - `other`   — everything else (punctuation, bytes ≥ 128, …)

The summary line ends with a newline. Test inputs always either end with a
newline or are completely empty, so the summary always starts at the
beginning of a line. On empty input, print only the summary line with all
four counters at zero.

Example — input:

```
Hello, World 42!
```

output:

```
hELLO, wORLD 42!
letters=10 digits=2 spaces=3 other=2
```

(the three spaces are the two blanks plus the final newline; the two
"other" bytes are `,` and `!`).

Use the `<ctype.h>` functions. Careful: their argument must be an
`unsigned char` value (or `EOF`) — pass a raw `char` that happens to be
negative and the behaviour is undefined. The grader feeds you a byte above
127, which must be classified as `other` and echoed unchanged.

Print nothing else. Exit with status 0.

Your code goes in `main.c`. Grade with `ctrain check case-and-counts`.
