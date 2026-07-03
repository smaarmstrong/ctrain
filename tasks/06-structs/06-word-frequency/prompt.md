# word frequency with a binary search tree

Write a whole program in `main.c` (this one **does** define `main`).

Read every *word* from standard input until end of input, count how many
times each distinct word occurs, then print one line per distinct word:
the word, a single space, and its count. Lines must come out in
**ascending byte order** of the words (the order `strcmp` defines).

A word is a maximal run of non-whitespace characters — exactly what
`scanf("%63s", buf)` gives you. Comparison is case-sensitive and
byte-for-byte: `Cat` and `cat` are different words.

Sample: for the input

```
the cat sat on the cat mat
```

the output is

```
cat 2
mat 1
on 1
sat 1
the 2
```

Store the counts in a **binary search tree** — one heap-allocated node
per distinct word holding the word (its own heap copy), the count, and
`left`/`right` child pointers. Insert by walking comparisons down the
tree; print with an in-order traversal; free the whole tree (nodes *and*
word copies) before exiting.

Guarantees and rules:

- Words are at most **63** bytes; longer tokens never appear.
- At most a few hundred distinct words — recursion is fine.
- Empty input produces no output at all.
- Print nothing but the word-count lines; each ends with a newline.
  Exit status 0.
- Note: `strdup` may not be declared under `-std=c11`; copy words with
  `malloc` + `strcpy` instead.

The grader also reruns your program under AddressSanitizer: any leaked
node or word copy fails the task even when the output is correct.

Grade with `ctrain check word-frequency`.
