# ticket dispenser — who drew first?

C does not promise the order in which the parts of one expression are
evaluated. Function arguments are a classic trap: in
`f(g(), h())` the compiler may call `g` and `h` in either order. When
both calls have side effects on the same state, the program's meaning
silently depends on the compiler's whim. Worse, expressions that
*modify* the same object twice without an intervening sequence point
(the infamous `i = i++`) are outright undefined behaviour. Compilers
warn about the textbook cases but stay silent about side effects hidden
behind function calls — like the ones in this task, which compile
cleanly under `-Wall -Wextra -Werror`.

`main.c` simulates a ticket dispenser: a counter starts at 1 and
`next_ticket()` hands out the current number and increments. The
program is supposed to print **exactly** this:

```
alice: 1
bob: 2
carol: 3
pair: left=4 right=5
pair: left=6 right=7
slot 8 stores 9
total issued: 9
```

The intended meaning, line by line:

- alice, bob, carol each take one ticket, in that order;
- then two "pair draws" happen: within each pair the **left** ticket is
  drawn first, then the **right** one;
- then one final pair is drawn where the **first** ticket picked chooses
  a slot (`slot 8`) and the **second** provides the value stored there
  (`stores 9`);
- finally the total number of tickets issued is printed.

The starter gets some lines wrong because it draws several tickets
inside a single expression, leaving the draw order to the compiler.
Rewrite those expressions so every draw is **sequenced**: one call per
statement, results captured in named variables, then printed. Any
program whose behaviour is fully defined and prints the exact text
above passes; the grader diffs your output byte-for-byte (final
newline included, nothing after it).

The program takes no input and no arguments, and must exit with
status 0.

Grade with `ctrain check ticket-order`.
