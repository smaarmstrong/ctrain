# Macro, function, or constant — pick the right tool

C gives you three ways to name a small computation or value: a
`#define`, a function, and a `const`/`enum` constant. Each requirement
below can only be met by one of them — read the requirements carefully
and let them force your hand. The grader checks behaviour only; it never
looks at which tool you picked, but the wrong tool cannot pass.

Write all three in `main.c`. Do **not** define `main` — the grader
textually `#include`s your file into a test harness.

1. **`BUF_SIZE`** — the buffer size, `32`. Requirements:
   - `#if BUF_SIZE == 32` in code that includes your file must take the
     true branch (the value must be visible **to the preprocessor** —
     note that `const int` and `enum` are invisible to `#if`, where
     unknown identifiers silently evaluate as `0`);
   - `static unsigned char scratch[BUF_SIZE];` at file scope must
     compile and make `sizeof scratch == 32`.

2. **`ARRAY_LEN(a)`** — the number of elements of an array `a`,
   whatever its element type (`double`, `char`, structs, ...).
   Requirements:
   - `ARRAY_LEN(samples)` where `samples` has 5 `double`s is `5`; for
     `char msg[] = "hello"` it is `6` (the terminator counts);
   - it must be an integer constant expression when the array's size is
     known at compile time: the harness uses it as a **file-scope**
     array dimension (`static short mirror[ARRAY_LEN(samples)];`), so a
     function cannot work — think `sizeof`.

3. **`int clampi(int x, int lo, int hi)`** — returns `lo` if `x < lo`,
   `hi` if `x > hi`, otherwise `x` (assume `lo <= hi`). Requirement:
   - every argument is evaluated **exactly once**, even when it has side
     effects. The harness calls it with arguments like `bump()` (a
     function that increments a counter) and then verifies the counter —
     a classic conditional macro evaluates its arguments two or three
     times and fails this. Write a real function.

The starter picks a defensible-looking but wrong tool for each one:
everything compiles, three checks fail. Fix each by switching tools.

Grade with `ctrain check right-tool`.
