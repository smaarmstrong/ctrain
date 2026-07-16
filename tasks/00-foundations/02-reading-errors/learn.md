THE IDEA

  When the compiler cannot make sense of your source, it does not guess —
  it stops and tells you why. Beginners see a wall of red text and panic.
  Don't: every message has the same fixed anatomy, and once you can read
  one you can read them all.

      main.c:5:15: error: expected ';' before 'printf'

  Read it left to right:

      main.c      which FILE
      5           which LINE in that file
      15          which COLUMN on that line (roughly where it noticed)
      error:      how bad — see below
      the rest    what it was expecting or objecting to

  There are three severities:

      error:      the compiler cannot continue; NO binary is produced
      warning:    legal C, but suspicious — compiles anyway (but this
                  trainer builds with -Werror, which promotes every
                  warning to an error, so treat them identically)
      note:       not a problem — extra context attached to the message
                  above it ("declared here", "expected int but got...")

---

WHY IT MATTERS

  You will spend more time reading diagnostics than writing code, in every
  language, forever. C compilers in particular can produce a CASCADE: one
  real mistake early in the file confuses the parser, and it reports ten
  phantom errors in perfectly fine code below. Which gives the single most
  useful rule in this lesson:

      FIX THE FIRST ERROR FIRST. Then recompile.

  Never start at the bottom. The first message is about real code; the
  rest may be echoes. Fixing one error and recompiling often makes five
  others vanish.

---

  Time to read a real one. This next program is broken on purpose — a
  missing semicolon at the end of line 5. The tutor will try to compile it
  anyway; it won't run, and that is the point. Read the compiler's message
  against the anatomy above before continuing:

```run
#include <stdio.h>

int main(void)
{
    int answer = 42
    printf("the answer is %d\n", answer);
    return 0;
}
```

  Notice two things. First, the location: the complaint points at line 6,
  where `printf` begins — the compiler only became certain something was
  missing when it hit the NEXT token, one line after the real mistake. When
  a message says "expected ';' before X", look at the line ABOVE X. Second:
  no output appeared, because no binary was ever produced.

---

THE CLASSICS

  Three errors account for most of a beginner's compiler time. Learn their
  shapes now and you will recognise them instantly later:

  1. Missing semicolon — you just met it:
         error: expected ';' before '...'
     Look at the end of the PREVIOUS line.

  2. Undeclared identifier — a name the compiler has never heard of,
     usually a typo, a missing declaration, or a missing #include:
         error: 'anwser' undeclared (first use in this function)
     Check the spelling against where the variable was declared. gcc will
     often add a note: "did you mean 'answer'?" — read the notes.

  3. Format mismatch — telling printf to expect one type and handing it
     another:
         warning: format '%s' expects argument of type 'char *',
         but argument 2 has type 'int' [-Wformat=]
     A warning, not an error — plain `cc` would compile it and the
     program would print garbage or crash. That is exactly why the
     graders here use -Werror: this "warning" is a real bug.

---

HOW TO DO IT

  The debugging loop when a compile fails:

      1. compile:  cc -Wall -o prog main.c
      2. read ONLY the first error (and any notes attached to it)
      3. open the named file at the named line — and look one line up
         if the message says "expected ... before ..."
      4. fix that one thing
      5. recompile. Repeat until the compiler is silent.

  Silence, then a fresh binary, then `./prog`. Resist the urge to fix
  three reported errors in one pass while you are learning — errors two
  and three may not be real.

---

GOTCHAS

  - The line number is where the compiler NOTICED, not always where you
    ERRED. "expected ';'" is the classic off-by-one-line message.
  - A cascade of 15 errors is usually 1 mistake. First error first.
  - Warnings matter. Under this trainer's -Werror they fail the build;
    in the wild they fail at 2am in production instead.
  - Read notes ("did you mean...?", "declared here") — the compiler is
    trying to hand you the fix.
