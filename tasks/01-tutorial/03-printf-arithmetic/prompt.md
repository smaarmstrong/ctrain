# arithmetic report

Declare two `int` variables in `main`:

```c
int a = 1234;
int b = 56;
```

and print exactly the following six lines, every value **computed from
`a` and `b`** with arithmetic operators and `printf` — do not paste the
numbers in as string literals:

```
a=1234 b=56
sum=1290 diff=1178
quot=22 rem=2
ratio=22.036
pct=4.54%
boxed=[  1234]
```

Line by line:

1. the two values themselves,
2. `a + b` and `a - b`,
3. the **integer** quotient `a / b` and remainder `a % b`,
4. `a / b` computed in **floating point**, printed with exactly three
   digits after the decimal point (`22.036`, not `22`),
5. what percentage `b` is of `a`, floating point, two digits after the
   decimal point, followed by a literal `%` sign (hint: `%%`),
6. the value of `a` right-aligned in a field six characters wide,
   between square brackets — so two spaces of padding before `1234`.

Print nothing else: no headers, no trailing blank line. Exit with
status 0.

The grader diffs your output byte-for-byte against the expected text, so
spacing matters everywhere.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check printf-arithmetic`.
