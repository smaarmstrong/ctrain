THE IDEA

  A C program is just a text file of instructions that a tool called a
  compiler turns into a runnable program. Every C program starts running at
  one special place: a function named main. "Function" here just means a
  named block of steps; main is the one the system runs first.

  The very smallest useful program has three parts:

      #include <stdio.h>      bring in the standard input/output tools
      int main(void) {        where the program starts
          ...                 the steps to do
          return 0;           tell the system "all went well"
      }

  To print a line of text we use printf ("print, formatted"), which comes
  from that stdio.h toolbox. Give it the text you want in double quotes.

---

  Here is the whole program. In a moment you'll compile and run it, and see
  the words appear:

```run
#include <stdio.h>

int main(void)
{
    printf("hello, world\n");
    return 0;
}
```

  Two details to notice in that line of printing:

    - the text is wrapped in "double quotes"
    - it ends with \n, which is not a backslash and an n — it is the single
      "newline" character, i.e. "move to the next line". Without it the text
      would print with no line break after it.

---

WHY IT MATTERS

  This is the whole toolchain in miniature: write text, compile it, run it,
  and the program reports success or failure back to whoever ran it. Every C
  program you ever write is a bigger version of exactly this shape.

  That "report back" is the number main gives to `return`. `return 0;` means
  "I succeeded." Any other number means "something went wrong" — scripts and
  other programs rely on this to know whether your program worked.

---

HOW TO DO IT

  If you were doing this by hand at a terminal, you would compile the file
  into a runnable program and then run it:

      cc -o hello main.c    # compile main.c, name the result "hello"
      ./hello               # run it
      echo $?               # print the exit status of the last program

  The trainer does the compiling for you when you run `check`. Your job is
  just to write the program: print exactly `hello, world` and a newline,
  then return 0.

---

CHECK IT WORKED

  The grader runs your program and compares its output, byte for byte,
  against the expected `hello, world` line — and confirms it exits with
  status 0. So the output must be exactly right: no capital H, no extra
  spaces, no missing newline.

  Let's prove the exit status idea to yourself. This program prints a line
  and then returns the number 3 instead of 0:

```run
#include <stdio.h>

int main(void)
{
    printf("this program reports failure\n");
    return 3;
}
```

  It printed fine — but had a real shell run it, `echo $?` afterwards would
  show 3, meaning "failure". For this task you want return 0.

---

GOTCHAS

  - Don't forget the \n. `printf("hello, world")` prints the right words but
    no newline, and the grader wants the newline — it would fail.
  - Match the text exactly: lower-case `hello, world`, one space after the
    comma, nothing else. No greeting banner, no extra blank line.
  - End main with `return 0;`. Leaving it out happens to work in modern C
    for main specifically, but write it — it states your intent, and every
    other function will need a real return value.
  - Every statement ends with a semicolon `;`. Forgetting it is the most
    common first compile error you'll meet.
