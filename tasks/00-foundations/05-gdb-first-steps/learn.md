THE IDEA

  The crash lesson left one step vague: "find the guilty line". When ASan
  is not around — or the program misbehaves without crashing — the general
  tool is a DEBUGGER. A debugger runs your program on your behalf, and
  because it is in charge, it can freeze time at any moment and show you
  exactly where execution is and what every variable holds.

  The standard C debugger on Linux is gdb (the "GNU debugger"). It is a
  huge tool, but the beginner's session is three commands long:

      run     start the program (it runs normally until something happens)
      bt      "backtrace": show where execution is, as a stack of calls
      quit    leave gdb

  That tiny loop — run, crash, bt, read, quit — solves a remarkable share
  of all debugging problems.

---

  A debugger can only name your functions and line numbers if the binary
  carries a map back to the source. That map is debug info, and you ask
  the compiler for it with `-g` (the same flag ASan wanted, for the same
  reason):

      cc -g -Wall -o prog main.c
      gdb ./prog

  Forget `-g` and gdb still works, but the backtrace shows raw addresses
  instead of `main.c:12` — far less useful. Habit: while learning, always
  compile with -g. It does not change what the program does.

---

WHY IT MATTERS

  Here is a program with a bug worth meeting in a controlled setting —
  RUNAWAY RECURSION. First, healthy recursion, so you know what right
  looks like: countdown(n) prints n and calls countdown(n - 1), stepping
  DOWN toward the stopping condition:

```run
#include <stdio.h>

static void countdown(int n)
{
    if (n == 0) {           /* the base case: where recursion stops */
        printf("liftoff\n");
        return;
    }
    printf("%d...\n", n);
    countdown(n - 1);       /* one step CLOSER to the base case */
}

int main(void)
{
    countdown(5);
    return 0;
}
```

  Every call sits in a region of memory called the STACK — one frame per
  call, holding its variables. countdown(5) stacks 6 frames, hits the
  base case, and unwinds. But if a typo makes each call step AWAY from
  the base case (say `countdown(n + 1)`), the calls never stop, frames
  pile up until the stack's fixed space runs out, and the program
  segfaults. Exit code 139 again — but this time the cause is depth, not
  a bad pointer, and the backtrace makes that unmistakable.

---

HOW TO DO IT

  A real session with that `n + 1` typo, shown here so you can read it
  before you drive one yourself. Lines starting with `(gdb)` are what
  you type:

      $ cc -g -Wall -o count main.c
      $ gdb ./count
      Reading symbols from ./count...
      (gdb) run
      Starting program: /work/count
      5...
      6...
      7...
      ...
      Program received signal SIGSEGV, Segmentation fault.
      0x000055555555517e in countdown (n=104856) at main.c:10
      10          countdown(n + 1);
      (gdb) bt
      #0  0x000055555555517e in countdown (n=104856) at main.c:10
      #1  0x0000555555555191 in countdown (n=104855) at main.c:10
      #2  0x0000555555555191 in countdown (n=104854) at main.c:10
      ...thousands more identical frames...
      (gdb) quit

  How to read it:

  - The moment it crashed, gdb froze the program and printed WHERE:
    function, argument values, file and line (`at main.c:10`) — and even
    the source line itself. That alone is most of the diagnosis.
  - `bt` lists the call stack, newest first. Frame #0 is where it died;
    frame #1 called frame #0; and so on down to main. Read `bt` output
    top-down and stop at the first frame in YOUR code — same rule as an
    ASan report.
  - A backtrace that is thousands of copies of the SAME function, with
    an argument marching in one direction (n=104856, 104855, ...), is
    the signature of runaway recursion. A normal bug's backtrace is
    short; when you see a wall of one name, look at how that function
    calls itself.
  - `quit` while a program is still loaded will ask
    "Quit anyway? (y or n)" — answer y.

---

GOTCHAS

  - No -g, no line numbers. Recompile with -g before reaching for gdb.
  - Frame #0 is the scene of the death, not always the scene of the
    crime — but it is always where to start reading.
  - If `bt` scrolls forever, that IS the finding: runaway recursion.
    (Press q to stop gdb's pager.)
  - gdb is a conversation, not a batch tool: the program is genuinely
    running inside it, and you can restart it with `run` as often as
    you like without leaving.
