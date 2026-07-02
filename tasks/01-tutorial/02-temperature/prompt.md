# Fahrenheit to Celsius table

Write a program that prints a Fahrenheit-to-Celsius conversion table to
standard output, covering 0 to 300 degrees Fahrenheit inclusive, in steps
of 20.

Each line must contain:

- the Fahrenheit value, right-aligned in a field 3 characters wide,
- a single space,
- the Celsius equivalent with exactly one digit after the decimal point,
  right-aligned in a field 6 characters wide.

Use the exact conversion `C = (5/9)(F - 32)` computed in floating point —
watch out: `5/9` in integer arithmetic is zero.

The first lines of the expected output look like this:

```
  0  -17.8
 20   -6.7
 40    4.4
```

Print nothing else — no headers, no trailing blank line beyond the final
newline.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check temperature`.
