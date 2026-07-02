# ctrain — learn C in the terminal, the hard-nosed way

A console trainer for learning **all of C**, structured after Kernighan &
Ritchie's *The C Programming Language* and extended with modern C (C11/C17),
memory safety, and tooling. Same DNA as
[smaarmstrong/redhat](https://github.com/smaarmstrong/redhat): real tasks,
graded by outcome.

The twist: **your code is graded by compiling and running it.** Every grader
builds your file with `-std=c11 -Wall -Wextra -Werror` (a warning is a
failure), runs it against behavioural tests, and — on pointer and allocation
tasks — runs it again under AddressSanitizer/UBSan (or valgrind). A program
that leaks, overflows, or invokes undefined behaviour does not pass, full
stop.

The trainer itself is written in C11. Building it is lesson zero:

```sh
make          # produces ./ctrain
./ctrain list
```

## Workflow

```sh
./ctrain list                # tasks grouped by K&R chapter, with your status
./ctrain start counting     # print the spec, create work/<task>/main.c
$EDITOR work/01-tutorial/03-counting/main.c
./ctrain check counting     # compile + behavioural tests + sanitizers
./ctrain solution counting  # one reference implementation
./ctrain reset counting     # restore starter (your code -> main.c.bak)
./ctrain status              # XP, daily streak, completion
```

Progress is stored in `~/.local/state/ctrain/progress`. XP is awarded on
first pass; a daily streak keeps you honest.

## What it covers

K&R chapters 1–8 as the backbone — tutorial, types & operators, control
flow, functions & program structure, pointers & arrays, structures, stdio,
and the UNIX system interface — then the standard library, the preprocessor,
undefined behaviour, data structures & algorithms (linked lists, stacks,
queues, BSTs, hash tables, sorting/searching with hidden tests), tooling
(Makefiles, gdb, valgrind/ASan, reading compiler warnings), and three
composite multi-file projects. See [docs/objectives.md](docs/objectives.md)
for the full checklist.

Prompts are original; the K&R lineage is structural (the classic exercises —
temperature tables, word counting, and friends — appear in adapted form).

## Requirements

A C compiler (`gcc` or `clang`), `make`, and `bash`. Nothing else — no GUI,
no package managers, no frameworks. For full memory-safety grading you want
a toolchain with ASan/UBSan (any mainstream gcc/clang) or valgrind installed;
graders detect what is available and degrade gracefully.

Or use the container, which has everything:

```sh
podman build -t ctrain . && podman run -it --cap-add SYS_PTRACE -v "$PWD":/ctrain:Z ctrain
```

## Task anatomy

```
tasks/05-pointers/03-swap/
├── meta.json      title, domain, objective, difficulty, est_min
├── prompt.md      the spec: what to implement, exact behaviour
├── starter.c      optional scaffold (copied to work/<task>/main.c)
├── test_main.c    optional C test harness linked against your code
├── grade.sh       compile → behavioural checks → sanitizer runs
└── solution.c     one reference implementation (or solution/ for multi-file)
```

Graders test behaviour, not style: any correct implementation passes.

## Selftest

Every grader is verified against its own artifacts — it must FAIL on the
starter stub and PASS on the reference solution:

```sh
./selftest.sh               # natively
./selftest.sh --container   # in a gcc:14 container (as CI does)
```

CI runs the full selftest on every pull request.
