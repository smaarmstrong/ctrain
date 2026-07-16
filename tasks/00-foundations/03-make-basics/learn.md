THE IDEA

  By now the cycle is: edit main.c, run `cc -Wall -o prog main.c`, run
  `./prog`. That compile command is already a mouthful, and real programs
  are built from many files with many flags. Nobody retypes that all day.

  `make` is a tool that remembers build commands for you. You write them
  once, in a plain text file named `Makefile`, as RULES. A rule has three
  parts:

      target: dependencies
      <TAB>   recipe

      target        the file this rule knows how to produce
      dependencies  the files it is built FROM
      recipe        the command(s) that produce it — each recipe line
                    MUST start with a real Tab character, not spaces

  A concrete rule, the one you will write yourself in a minute:

      greet: main.c
              cc -Wall -Werror -o greet main.c

  Read it as a sentence: "the file `greet` is built from `main.c`; to
  (re)build it, run this cc command."

---

WHY IT MATTERS

  Running `make` does something smarter than blindly running the recipe:
  it compares TIMESTAMPS. If `greet` already exists and is newer than
  `main.c`, there is nothing to do and make says so. If you have edited
  `main.c` since the last build, make sees the source is newer and runs
  the recipe again.

  That kills the classic beginner bug from lesson one — running a stale
  binary after an edit. With make there is one habit: `make`, then run.
  It rebuilds exactly when needed and never when not.

  It is also why THIS repo drives everything through make. You have been
  typing `make learn`, `make check`... — each of those is just a rule in
  the Makefile at the repo root. The trainer itself is a C program, and
  `make ctrain` is a rule exactly like `greet: main.c`. One tool, the
  correct flags encoded once, nothing stale. (Have a look — `less
  Makefile` at the repo root after this lesson. You will recognise the
  shape now.)

---

  Here is the little program your Makefile will build in this task. Run
  it once so you know what "working" looks like:

```run
#include <stdio.h>

int main(void)
{
    printf("built by make, so never stale\n");
    return 0;
}
```

---

HOW TO DO IT

  In a directory containing `main.c`, create a file named `Makefile`
  containing the rule from above, then just say:

      make

  With no arguments, make builds the FIRST target in the file (the
  "default target"). You should see it echo the cc command. Then:

      ./greet       run the result
      make          again — "'greet' is up to date": nothing recompiled

  Now touch the source and watch the timestamp logic wake up:

      touch main.c  pretend we edited it (bumps its timestamp)
      make          rebuilds, because main.c is now newer than greet

  Two commands worth knowing while you are at it:

      make -q       "quiet question": build nothing, just answer via the
                    exit status — 0 if up to date, 1 if a rebuild is due
      make greet    name a target explicitly instead of taking the first

---

GOTCHAS

  - THE TAB. Recipe lines must begin with a real Tab character. Spaces
    give the single most famous error in Unix:
        Makefile:2: *** missing separator.  Stop.
    If you see that, your editor put spaces where a Tab belongs.
  - The file must be called `Makefile` (or `makefile`) for plain `make`
    to find it.
  - The first rule in the file is what bare `make` builds — order your
    Makefile so the real program comes first.
  - Dependencies are declarations, not decoration: if you list the wrong
    ones, make will happily skip rebuilds it should have done. Everything
    the recipe reads belongs on the dependency list.
