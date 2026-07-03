# Stock report — printf widths and precision

Write a program that prints a stock valuation report for this fixed data
(hard-code it in your program — nothing is read from input):

| item       |  qty | unit price |
|------------|-----:|-----------:|
| bolt       | 1250 |       0.08 |
| camshaft   |   34 |     145.50 |
| gasket     |    0 |      12.99 |
| oil-filter |    7 |       8.50 |

For each item the *value* is `qty × unit price`, and the *share* is that
value as a percentage of the total value of all four items.

Print a header line, one line per item, and a `TOTAL` line, using these
fields on every line:

- item name, **left**-justified in a field 12 wide,
- quantity, right-aligned in a field 6 wide,
- unit price, right-aligned in a field 10 wide with exactly 2 decimals,
- value, right-aligned in a field 12 wide with exactly 2 decimals,
- share, right-aligned in a field 6 wide with exactly 1 decimal, followed
  immediately by a literal `%` character.

The header uses the same widths with the words `item`, `qty`, `price`,
`value`, `share` (the share heading is right-aligned in a field 7 wide,
since data rows occupy 6 + 1 characters there). The `TOTAL` line leaves
the qty and price columns blank (empty strings padded to the same widths)
and shows the grand total and `100.0%`.

The exact expected output is:

```
item           qty     price       value  share
bolt          1250      0.08      100.00   2.0%
camshaft        34    145.50     4947.00  96.9%
gasket           0     12.99        0.00   0.0%
oil-filter       7      8.50       59.50   1.2%
TOTAL                            5106.50 100.0%
```

Print nothing else — no trailing blank line beyond the final newline.
Do the arithmetic in `double`; let `printf` do all rounding for you.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check printf-gallery`.
