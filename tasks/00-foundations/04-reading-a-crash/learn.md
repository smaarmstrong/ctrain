THE IDEA

  Sometimes a C program does not print a polite error — it just dies.
  The message at the shell is famously terse:

      Segmentation fault (core dumped)

  Here is what actually happened. Every running program is given regions
  of memory it is allowed to use. Each variable, each string, each piece
  of your program lives at a numbered ADDRESS inside those regions. C lets
  you work with addresses directly — that is the language's power — which
  also means a buggy program can compute a bad address and try to use it:
  address zero, an address one past the end of an array, an address that
  was freed. The moment the program touches memory it does not own, the
  operating system stops it dead. That is a segmentation fault ("segment"
  is the old word for those memory regions). The program does not get to
  finish, clean up, or print anything more.

  A segfault is therefore never the real bug — it is the SYMPTOM. The bug
  is whatever computed the bad address, and the whole skill of this lesson
  is working back from the symptom to that cause.

---

  Watch one die. This program prints its first line, then asks strchr to
  find a '=' in a string that does not contain one. strchr says "not
  found" the C way — by returning a NULL pointer, the deliberately-invalid
  address zero — and the code uses it anyway:

```run
#include <stdio.h>
#include <string.h>

int main(void)
{
    printf("about to look for '=' where there is none...\n");
    fflush(stdout); /* push that line out now — the crash below must not swallow it */

    const char *entry = "colour";              /* no '=' inside */
    const char *sep = strchr(entry, '=');      /* not found -> sep is NULL */
    printf("the value is: %s\n", sep + 1);     /* uses address 0+1 -> crash */
    return 0;
}
```

  One line of output, then nothing — the second printf never completed.
  Notice there was no warning from the compiler: this program is legal C.
  The mistake only exists at run time, on this input.

---

WHY IT MATTERS

  How do you even KNOW it crashed, if the shell's message is the only
  clue? Exit codes, from the compile-run lesson, come back here. A program
  killed by the operating system cannot return from main, so the shell
  reports 128 plus the number of the signal that killed it. The segfault
  signal is number 11:

      ./prog
      echo $?      # 139  = 128 + 11: killed by a segfault

  You will meet 139 in build logs and test output for the rest of your
  career; read it as "segfaulted" on sight. (Its cousin 134 is SIGABRT —
  the program aborted itself, e.g. a failed assert.)

  And crashing is the GOOD outcome. The same wild address can happen to
  land inside memory the program does own — then nothing crashes and the
  program silently corrupts its own data instead. You cannot rely on the
  OS to catch your pointer bugs; you need a tool that checks properly.

---

ADDRESS SANITIZER

  That tool is AddressSanitizer (ASan). Ask the compiler to build extra
  checking into the binary, then just run it:

      cc -g -fsanitize=address -o prog main.c
      ./prog

  (`-g` embeds file and line-number info so reports can name your code;
  you will meet it again in the gdb lesson.) Now an out-of-bounds read
  that would have silently worked prints a report like this — shown here,
  not run, so we can pick it apart:

      ==4242==ERROR: AddressSanitizer: heap-buffer-overflow on address
      0x602000000018 at pc ... bp ... sp ...
      READ of size 1 at 0x602000000018 thread T0
          #0 0x55c1a3f012ab in scan_word /work/main.c:14
          #1 0x55c1a3f013f0 in main /work/main.c:23
      ...

  Read it top down, and stop early — the first lines carry the story:

      line 1        WHAT went wrong: heap-buffer-overflow — reading past
                    the end of something allocated on the heap
      READ of size  the program was reading (not writing) 1 byte
      #0 ...        WHERE: the exact function, file and line that did it
                    — scan_word, main.c line 14
      #1 ...        who called it (main, line 23), and so on down the
                    call chain

  Frame #0 of that stack is the payoff: your file, your line. Go there.
  The rest of the report (shadow bytes, hex dumps) can wait until you are
  much further along; almost every bug is findable from the first frame
  in your own code.

---

  One honest caveat before you rely on it: ASan support lives in a
  library that ships with the compiler toolchain, and not every machine
  has it installed natively (this one may not). This repo's graders probe
  for it and fall back gracefully, and CI runs every task inside a
  container where ASan is always present — so the container run is the
  gate that counts. If `-fsanitize=address` fails to link on your
  machine, nothing is wrong with your code; practise reading the report
  above and move on.

---

HOW TO DO IT

  The crash-reading drill, start to finish:

      1. it "just died" / exit code 139        -> it segfaulted
      2. rebuild with ASan if available:
             cc -g -fsanitize=address -o prog main.c
         and run again — read WHAT + frame #0 (file:line)
      3. no ASan? no problem: the crash is still deterministic. Find the
         last output that DID appear and you have bracketed the crash
         site (the gdb lesson next makes this step precise)
      4. at the guilty line, ask: which pointer here could be bad, and
         which input makes it bad? NULL returns are suspect number one —
         C library functions that search (strchr, fopen, malloc, getenv)
         all report failure by returning NULL, and every one of those
         returns must be checked before use

---

GOTCHAS

  - The segfault is the symptom, not the bug. The bug computed the bad
    address; the crash just cashed it in.
  - Exit code 139 = segfault, 134 = abort. Learn them on sight.
  - No crash does not mean no bug — wild pointers can hit owned memory
    and corrupt it silently. ASan exists precisely for this.
  - Check every pointer a library hands back. "It can't fail here" is
    how the third config line crashes in production.
