# Fixed-size records

Build a tiny binary record store. Each record is **exactly 20 bytes**:

| bytes  | content                                                    |
|--------|------------------------------------------------------------|
| 0–15   | item name: ASCII, no spaces, at most 15 chars, padded to 16 bytes with `'\0'` |
| 16–19  | quantity: unsigned 32-bit integer, **little-endian**        |

The program has two commands:

## `prog write FILE`

Read lines of the form `NAME QTY` (one space between them) from standard
input until end of file, and write one 20-byte record per line to `FILE`
(create or truncate it; open it in **binary** mode, `"wb"`). Input lines
are well-formed: `NAME` fits the field, `QTY` fits in an unsigned 32-bit
integer (it may be as large as 4294967295). The file must contain the
records back-to-back — its size is exactly `20 × number-of-lines`.

## `prog get FILE INDEX`

Open `FILE` (binary mode, `"rb"`), position directly to record number
`INDEX` (0-based) with `fseek` — do not read the records before it —
`fread` that one record, and print

```
NAME QTY
```

(name, one space, quantity in decimal) followed by a newline to standard
output, then exit **0**.

## Errors

- `FILE` cannot be opened (either command): print
  `record-file: cannot open FILE` to **standard error**, exit **1**.
- `INDEX` is at or past the end of the file (the `fread` comes up
  short): print `record-file: no record INDEX` to standard error,
  exit **1**.
- Anything else wrong with the arguments (unknown command, wrong number
  of arguments, `INDEX` not a non-negative decimal integer): print a
  line starting `usage:` to standard error, exit **2**.

Diagnostics never appear on standard output.

## Notes

- Assemble and decode the quantity **byte by byte** (e.g.
  `b[16] = qty & 0xff; b[17] = (qty >> 8) & 0xff; …`) so the layout is
  little-endian by construction, not by luck of the host CPU.
- The grader checks the file byte-for-byte, and also reads a file it
  built itself — a text-format impostor will not pass.

Example session:

```
$ printf 'bolt 7\ngasket 300\n' | prog write stock.bin
$ prog get stock.bin 1
gasket 300
```

`stock.bin` is 40 bytes: `bolt` + 12 zero bytes + `07 00 00 00`, then
`gasket` + 10 zero bytes + `2c 01 00 00`.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check record-file`.
