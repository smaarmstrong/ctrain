# Coverage objectives

The backbone is K&R 2nd edition's chapter progression; the second half adds
what a modern C programmer needs that the book predates. Each objective is
ticked when at least one task exercises it. Canonical K&R exercises appear
in adapted, re-worded form.

97 tasks across 14 domains — every objective below is exercised by at least one task.

## 01-tutorial — K&R Ch1: A Tutorial Introduction (8 tasks)
- [x] hello world, compiling and running (`cc`, `-o`, exit status)
- [x] variables, arithmetic, `printf` formatting
- [x] temperature conversion table (the classic, re-worded)
- [x] reading characters: `getchar`/`putchar`, EOF
- [x] counting lines, words, characters (wc-shaped)
- [x] arrays: counting digit/whitespace frequencies
- [x] first functions: `power`-style helper
- [x] character arrays: longest-line

## 02-types — K&R Ch2: Types, Operators, Expressions (8 tasks)
- [x] integer and floating types, ranges, `<limits.h>`/`<float.h>`
- [x] constants, string literals, enums
- [x] implicit conversions and casts
- [x] char arithmetic (atoi/lower-style, re-worded)
- [x] bitwise operators: get/set/toggle bit fields in an int
- [x] shifts, masks (bitcount-style)
- [x] precedence and associativity pitfalls
- [x] modern: fixed-width types `<stdint.h>`, `size_t`, `bool` (C11 `<stdbool.h>`)

## 03-control — K&R Ch3: Control Flow (6 tasks)
- [x] if/else chains, ternary
- [x] switch (with fallthrough discipline)
- [x] while/for equivalence
- [x] do-while (itoa-style, re-worded)
- [x] break/continue
- [x] loops + validation: binary search over a sorted array

## 04-functions — K&R Ch4: Functions & Program Structure (7 tasks)
- [x] function definitions, prototypes, return values
- [x] separate compilation: multi-file build, headers, include guards
- [x] external and static variables, linkage
- [x] register/auto/scope rules
- [x] recursion (printd/qsort-style, re-worded)
- [x] the C preprocessor: #include, #define basics
- [x] modern: `inline`, `_Static_assert`, designated initializers

## 05-pointers — K&R Ch5: Pointers & Arrays (10 tasks)
- [x] pointer basics: & and *, swap via pointers
- [x] pointers vs arrays, pointer arithmetic
- [x] strings: strlen/strcpy/strcmp by hand (re-worded)
- [x] pointer arrays: sorting lines
- [x] multidimensional arrays: day-of-year style
- [x] command-line arguments: argc/argv (echo/grep-lite)
- [x] pointers to functions: generic sort with comparator
- [x] dynamic memory: malloc/free with ownership discipline (ASan-enforced)
- [x] realloc growth patterns: dynamic array
- [x] complicated declarations (dcl-style, re-worded)

## 06-structs — K&R Ch6: Structures (8 tasks)
- [x] struct basics, initialization, struct functions
- [x] arrays of structs (keyword counting-style)
- [x] pointers to structs, ->
- [x] typedef
- [x] self-referential structs: singly linked list (ASan-enforced)
- [x] binary search tree (word frequency-style, re-worded)
- [x] hash table with chaining (ASan-enforced)
- [x] unions and bit-fields

## 07-io — K&R Ch7: Input & Output (7 tasks)
- [x] formatted output: printf specifiers, width/precision
- [x] formatted input: scanf and its hazards; parsing with sscanf
- [x] file access: fopen/fclose/fgets/fputs (cat-style)
- [x] error handling: stderr, exit status, errno/perror
- [x] line input and ungetc
- [x] binary I/O: fread/fwrite records
- [x] buffering: setvbuf, fflush semantics

## 08-system — K&R Ch8: The UNIX System Interface (6 tasks)
- [x] file descriptors: read/write syscalls (cat via fd)
- [x] open/creat/close/unlink, flags and modes
- [x] lseek and random access
- [x] implementing a getchar-style buffered reader over read(2)
- [x] directory listing: opendir/readdir (fsize-style, re-worded)
- [x] a storage allocator's shape: sbrk-era ideas in modern terms (advanced)

## 09-stdlib — The Standard Library (7 tasks)
- [x] string.h: strtok/strstr/strchr/memcpy/memmove correctness
- [x] ctype.h: classification and case mapping
- [x] stdlib.h: strtol/strtod (with full error handling), qsort/bsearch
- [x] math.h: floating point, rounding, HUGE_VAL/errno
- [x] time.h: time/localtime/strftime
- [x] rand/srand and shuffling correctly
- [x] assert.h and defensive programming

## 10-preprocessor — Preprocessor & Macros (5 tasks)
- [x] object- and function-like macros, argument pitfalls (parenthesisation)
- [x] conditional compilation: #if/#ifdef/#error
- [x] include guards vs #pragma once
- [x] stringizing (#) and pasting (##), variadic macros (__VA_ARGS__)
- [x] macros vs inline functions vs const — when each

## 11-ub — Undefined Behaviour & Memory Safety (6 tasks)
- [x] signed overflow and how to check safely
- [x] out-of-bounds access: find-and-fix under ASan
- [x] use-after-free and double free: find-and-fix under ASan
- [x] uninitialised reads; strict aliasing basics
- [x] sequence points / unsequenced modification
- [x] integer conversion traps (size_t underflow, char signedness)

## 12-dsa — Data Structures & Algorithms in C (9 tasks)
- [x] stack (array-backed, hidden tests)
- [x] queue / ring buffer (hidden tests)
- [x] doubly linked list with full ownership (ASan-enforced)
- [x] binary search tree: insert/find/delete/traverse
- [x] hash table: open addressing or chaining, resize
- [x] insertion sort, then mergesort (stable, hidden tests)
- [x] quicksort with comparator (qsort-compatible signature)
- [x] binary search variants (first/last occurrence)
- [x] a generic container using void* and function pointers (advanced)

## 13-tooling — Build, Debug, Diagnose (6 tasks)
- [x] write a Makefile: targets, variables, pattern rules, .PHONY
- [x] multi-file project layout: headers, objects, linking
- [x] reading compiler warnings: fix a warning-riddled file (-Werror)
- [x] gdb: scripted debugging to extract a runtime fact
- [x] valgrind/ASan: diagnose a leaky program and fix it
- [x] compile-time hardening flags; -O2 vs -O0 observable differences

## 14-projects — Composite Projects (4 tasks, multi-file, end-to-end)
- [x] a text utility: wc/grep-lite with flags (argv parsing, files, exit codes)
- [x] a generic data-structure library (header + impl + tests, ASan-clean)
- [x] a tiny interactive shell: prompt, fork/exec, wait, builtins (advanced)
- [x] a K&R-style storage allocator or arena (bonus)

## Quality bars (apply to every task)
- graders test behaviour/output, not style — any correct implementation passes
- `-std=c11 -Wall -Wextra -Werror`: warnings are failures
- pointer/allocation tasks run under ASan/UBSan (valgrind fallback) and fail on leaks/UB
- deterministic graders; selftest enforces solution-passes / starter-fails
- everything runs in the gcc:14 container (see Dockerfile, CI)
