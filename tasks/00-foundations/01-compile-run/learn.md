THE IDEA

  A C program starts life as a plain text file — characters you can read,
  usually named something.c. Your computer's processor cannot run text. It
  runs "machine code": raw numeric instructions baked into a file called a
  binary (or an executable).

  The tool that translates one into the other is the COMPILER. On this
  system it answers to the name `cc` ("C compiler"). The whole cycle you
  will repeat thousands of times is:

      1. write or edit   main.c        (text — the SOURCE)
      2. compile         cc ... main.c (translate it)
      3. run             ./prog        (execute the BINARY it produced)

  Three separate steps, three separate things: the source file, the
  compiler, and the binary the compiler writes out.

---

  Here is a complete C source file. Don't worry about what every line means
  yet — the next chapter teaches the language. Right now the point is the
  cycle: this text will be compiled, then run, and you will see its output:

```run
#include <stdio.h>

int main(void)
{
    printf("I was text a moment ago.\n");
    printf("Now I am a running program.\n");
    return 0;
}
```

  When you pressed Enter just now, the tutor did exactly the two steps you
  will soon do by hand: it saved that text to a file, ran the compiler on
  it, and executed the result.

---

WHY IT MATTERS

  Everything else in this trainer — every task, every grader — is a loop of
  edit, compile, run. If the cycle is fuzzy, everything built on it is
  fuzzy. Two consequences of the cycle trip up almost every beginner:

  - The binary is a SNAPSHOT. Editing main.c changes the text file only.
    The binary you compiled five minutes ago does not notice. If you edit
    and re-run without recompiling, you are running the OLD program —
    "my change did nothing!" nearly always means "you forgot to recompile".

  - Compiling can fail. If the compiler cannot understand your text it
    prints error messages and writes NO binary (or leaves the stale old
    one). The next lesson is entirely about reading those messages.

---

HOW TO DO IT

  At a terminal, the plain form is:

      cc main.c

  That works, but it names the binary `a.out` — a 1970s default that tells
  you nothing. Two flags make `cc` far more useful, and you should form the
  habit of always passing both:

      cc -Wall -o prog main.c
      ./prog

  - `-o prog` means "put the Output in a file named prog" (any name you
    like). Without it: a.out.
  - `-Wall` means "Warn me about ALL the common suspicious things". A
    warning is the compiler saying "this is legal C, but it is probably
    not what you meant" — for example printing a number with the wrong
    format, or using a variable you never gave a value. The program still
    compiles, but experienced C programmers treat every warning as a bug
    report, because in C "probably wrong" code often crashes or corrupts
    data later, far from the real mistake. This trainer's graders go one
    step further and use -Werror, which turns warnings INTO errors: your
    code must compile with no complaints at all.

  The `./` in `./prog` matters too: it says "run the prog that is right
  here in this directory" (the shell does not look in the current
  directory for commands unless you point at it).

---

CHECK IT WORKED

  How do you know a program succeeded? Besides reading its output, every
  program hands a number back to whoever ran it — its EXIT STATUS. By
  convention 0 means "all fine" and anything else means "something went
  wrong". That number is what `return 0;` at the end of main is for, and
  at a shell you can inspect it with:

      ./prog
      echo $?        # prints the exit status of the last command — 0

  One more turn of the cycle, with a program that misbehaves on purpose —
  it reports failure by returning 2 instead of 0:

```run
#include <stdio.h>

int main(void)
{
    printf("printing worked, but I am about to report failure\n");
    return 2;
}
```

  The output looks normal — the failure is invisible until something asks
  for the exit status. Scripts, `make`, and this trainer's graders all ask.

---

GOTCHAS

  - Edit, compile, RUN — in that order, every time. An edit does nothing
    until you recompile.
  - `cc -o prog main.c`: the name after -o is where the binary goes. Do
    not write `cc -o main.c prog` — that tells the compiler to OVERWRITE
    your source file with the binary. (Modern compilers refuse, but do not
    rely on it.)
  - No output file appears if compilation failed. Check the messages
    before hunting for the binary.
  - Always -Wall. Silence from the compiler should mean something.
