# count lines, words and characters

Write a miniature `wc`: read standard input to `EOF` and print three
counts on a single line —

```
LINES WORDS CHARS
```

— three decimal numbers separated by single spaces, followed by one
newline, then exit with status 0. Print nothing else.

Definitions (state them in code exactly like this):

- **CHARS** — every character read, including spaces, tabs and newlines.
- **LINES** — the number of newline characters (`'\n'`). Input that does
  not end in a newline therefore has an "unfinished" last line that is
  **not** counted.
- **WORDS** — maximal runs of characters that are not space, tab or
  newline. `"  a\t bc \n"` contains two words, `a` and `bc`. A word
  broken only by punctuation is still one word: `don't` counts once.

Edge cases the grader probes:

- empty input prints `0 0 0`,
- runs of mixed whitespace between words,
- input whose final line has no trailing newline (the word on it still
  counts; the line does not),
- a blank line (it adds one line, no words, one character).

Example — for the input

```
one
two three
```

(ending in a newline) the output is:

```
2 3 14
```

Your code goes in `main.c` in the work directory. Grade with
`ctrain check word-count`.
