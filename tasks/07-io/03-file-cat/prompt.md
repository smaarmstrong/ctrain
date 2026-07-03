# mycat — concatenate files

Write a simplified `cat`:

- **With no command-line arguments**: copy standard input to standard
  output, unchanged, until end of file.

- **With one or more arguments**: treat each argument as a file name.
  Open each file for reading, copy its entire contents to standard
  output, and close it, processing the files in argument order. The
  outputs are concatenated with nothing added between them.

- If a file cannot be opened, print exactly

  ```
  mycat: cannot open FILENAME
  ```

  (with `FILENAME` replaced by the argument) to **standard error**, and
  exit immediately with status **1** — files later in the argument list
  are not processed. Nothing about the error appears on standard output.

- On success exit with status **0**.

The copy must be faithful:

- a file whose last line has **no trailing newline** must be reproduced
  without one (do not add newlines that were not there);
- lines may be long — up to 4000 characters — so if you read with a
  fixed-size `fgets` buffer, remember that writing back each chunk with
  `fputs` as you go reproduces the bytes exactly, whether or not a chunk
  ends in `\n`. Test files are plain text (no NUL bytes).

Example: if `a.txt` contains `alpha\n` and `b.txt` contains `beta` (no
newline), then `mycat a.txt b.txt` prints `alpha\nbeta` and exits 0.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check file-cat`.
