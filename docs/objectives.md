# Coverage objectives

The backbone is K&R 2nd edition's chapter progression; the second half adds
what a modern C programmer needs that the book predates. Each objective is
ticked when at least one task exercises it. Canonical K&R exercises appear
in adapted, re-worded form.

Task counts are targets for the full build (~100 tasks overall).

## 01-tutorial — K&R Ch1: A Tutorial Introduction (~8 tasks)
- [ ] hello world, compiling and running (`cc`, `-o`, exit status)
- [ ] variables, arithmetic, `printf` formatting
- [ ] temperature conversion table (the classic, re-worded)
- [ ] reading characters: `getchar`/`putchar`, EOF
- [ ] counting lines, words, characters (wc-shaped)
- [ ] arrays: counting digit/whitespace frequencies
- [ ] first functions: `power`-style helper
- [ ] character arrays: longest-line

## 02-types — K&R Ch2: Types, Operators, Expressions (~7 tasks)
- [ ] integer and floating types, ranges, `<limits.h>`/`<float.h>`
- [ ] constants, string literals, enums
- [ ] implicit conversions and casts
- [ ] char arithmetic (atoi/lower-style, re-worded)
- [ ] bitwise operators: get/set/toggle bit fields in an int
- [ ] shifts, masks (bitcount-style)
- [ ] precedence and associativity pitfalls
- [ ] modern: fixed-width types `<stdint.h>`, `size_t`, `bool` (C11 `<stdbool.h>`)

## 03-control — K&R Ch3: Control Flow (~6 tasks)
- [ ] if/else chains, ternary
- [ ] switch (with fallthrough discipline)
- [ ] while/for equivalence
- [ ] do-while (itoa-style, re-worded)
- [ ] break/continue
- [ ] loops + validation: binary search over a sorted array

## 04-functions — K&R Ch4: Functions & Program Structure (~7 tasks)
- [ ] function definitions, prototypes, return values
- [ ] separate compilation: multi-file build, headers, include guards
- [ ] external and static variables, linkage
- [ ] register/auto/scope rules
- [ ] recursion (printd/qsort-style, re-worded)
- [ ] the C preprocessor: #include, #define basics
- [ ] modern: `inline`, `_Static_assert`, designated initializers

## 05-pointers — K&R Ch5: Pointers & Arrays (~10 tasks)
- [ ] pointer basics: & and *, swap via pointers
- [ ] pointers vs arrays, pointer arithmetic
- [ ] strings: strlen/strcpy/strcmp by hand (re-worded)
- [ ] pointer arrays: sorting lines
- [ ] multidimensional arrays: day-of-year style
- [ ] command-line arguments: argc/argv (echo/grep-lite)
- [ ] pointers to functions: generic sort with comparator
- [ ] dynamic memory: malloc/free with ownership discipline (ASan-enforced)
- [ ] realloc growth patterns: dynamic array
- [ ] complicated declarations (dcl-style, re-worded)

## 06-structs — K&R Ch6: Structures (~8 tasks)
- [ ] struct basics, initialization, struct functions
- [ ] arrays of structs (keyword counting-style)
- [ ] pointers to structs, ->
- [ ] typedef
- [ ] self-referential structs: singly linked list (ASan-enforced)
- [ ] binary search tree (word frequency-style, re-worded)
- [ ] hash table with chaining (ASan-enforced)
- [ ] unions and bit-fields

## 07-io — K&R Ch7: Input & Output (~7 tasks)
- [ ] formatted output: printf specifiers, width/precision
- [ ] formatted input: scanf and its hazards; parsing with sscanf
- [ ] file access: fopen/fclose/fgets/fputs (cat-style)
- [ ] error handling: stderr, exit status, errno/perror
- [ ] line input and ungetc
- [ ] binary I/O: fread/fwrite records
- [ ] buffering: setvbuf, fflush semantics

## 08-system — K&R Ch8: The UNIX System Interface (~6 tasks)
- [ ] file descriptors: read/write syscalls (cat via fd)
- [ ] open/creat/close/unlink, flags and modes
- [ ] lseek and random access
- [ ] implementing a getchar-style buffered reader over read(2)
- [ ] directory listing: opendir/readdir (fsize-style, re-worded)
- [ ] a storage allocator's shape: sbrk-era ideas in modern terms (advanced)

## 09-stdlib — The Standard Library (~7 tasks)
- [ ] string.h: strtok/strstr/strchr/memcpy/memmove correctness
- [ ] ctype.h: classification and case mapping
- [ ] stdlib.h: strtol/strtod (with full error handling), qsort/bsearch
- [ ] math.h: floating point, rounding, HUGE_VAL/errno
- [ ] time.h: time/localtime/strftime
- [ ] rand/srand and shuffling correctly
- [ ] assert.h and defensive programming

## 10-preprocessor — Preprocessor & Macros (~5 tasks)
- [ ] object- and function-like macros, argument pitfalls (parenthesisation)
- [ ] conditional compilation: #if/#ifdef/#error
- [ ] include guards vs #pragma once
- [ ] stringizing (#) and pasting (##), variadic macros (__VA_ARGS__)
- [ ] macros vs inline functions vs const — when each

## 11-ub — Undefined Behaviour & Memory Safety (~6 tasks)
- [ ] signed overflow and how to check safely
- [ ] out-of-bounds access: find-and-fix under ASan
- [ ] use-after-free and double free: find-and-fix under ASan
- [ ] uninitialised reads; strict aliasing basics
- [ ] sequence points / unsequenced modification
- [ ] integer conversion traps (size_t underflow, char signedness)

## 12-dsa — Data Structures & Algorithms in C (~9 tasks)
- [ ] stack (array-backed, hidden tests)
- [ ] queue / ring buffer (hidden tests)
- [ ] doubly linked list with full ownership (ASan-enforced)
- [ ] binary search tree: insert/find/delete/traverse
- [ ] hash table: open addressing or chaining, resize
- [ ] insertion sort, then mergesort (stable, hidden tests)
- [ ] quicksort with comparator (qsort-compatible signature)
- [ ] binary search variants (first/last occurrence)
- [ ] a generic container using void* and function pointers (advanced)

## 13-tooling — Build, Debug, Diagnose (~6 tasks)
- [ ] write a Makefile: targets, variables, pattern rules, .PHONY
- [ ] multi-file project layout: headers, objects, linking
- [ ] reading compiler warnings: fix a warning-riddled file (-Werror)
- [ ] gdb: scripted debugging to extract a runtime fact
- [ ] valgrind/ASan: diagnose a leaky program and fix it
- [ ] compile-time hardening flags; -O2 vs -O0 observable differences

## 14-projects — Composite Projects (~4 tasks, multi-file, end-to-end)
- [ ] a text utility: wc/grep-lite with flags (argv parsing, files, exit codes)
- [ ] a generic data-structure library (header + impl + tests, ASan-clean)
- [ ] a tiny interactive shell: prompt, fork/exec, wait, builtins (advanced)
- [ ] a K&R-style storage allocator or arena (bonus)

## Quality bars (apply to every task)
- graders test behaviour/output, not style — any correct implementation passes
- `-std=c11 -Wall -Wextra -Werror`: warnings are failures
- pointer/allocation tasks run under ASan/UBSan (valgrind fallback) and fail on leaks/UB
- deterministic graders; selftest enforces solution-passes / starter-fails
- everything runs in the gcc:14 container (see Dockerfile, CI)
