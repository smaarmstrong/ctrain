# sales tally

Write a whole program in `main.c` (this one **does** define `main`).

Standard input is a sequence of sale records, each consisting of a product
name and an integer quantity, separated by whitespace:

```
widget 5
gadget 3
widget 2
gadget -1
```

Read records until end of input, then print one line per **distinct**
product, in the order each product was **first seen**, giving the product
name and the sum of all its quantities separated by a single space.
After those lines print a final line with the word `distinct`, a space,
and the number of distinct products. For the input above:

```
widget 7
gadget 2
distinct 2
```

Rules and guarantees:

- Keep the records in an **array of structs** (name plus running total)
  and find existing products with a linear search — no need for anything
  cleverer.
- Product names contain no whitespace and are at most **31** characters
  long. There are at most **100** distinct products. Input is always
  well-formed pairs; you never need to reject anything.
- Quantities are `int`s and may be negative (returns). A product whose
  total ends up zero or negative is still printed.
- Empty input prints just `distinct 0`.
- Print nothing else; every line ends with a newline. Exit status 0.

Reading with `scanf("%31s %d", ...)` until it stops returning 2 is a
perfectly good input loop.

Grade with `ctrain check sales-tally`.
