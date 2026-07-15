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
make ctrain   # produces ./ctrain  (bare `make` just lists the targets)
./ctrain list
```

## Workflow

New to C? Let the trainer teach you, then decide what to practise for you:

```sh
make learn                   # teach the next task, then set it up to try
$EDITOR work/01-tutorial/01-hello-world/main.c
make check                   # grade the task you're on (defaults to the current one)
make train                   # once you know the ropes: pick the next task for you
```

`make learn` walks you through a paced lesson — prose, pauses, and runnable
steps that **compile and run real C** so you see the output — then hands off
to a solo attempt. `make train` is the autopilot: it gives you the next new
task in a fundamentals-first order, or brings back an older one for a
spaced-repetition review when it's due (never more than two reviews in a row
while new material waits). Tasks without a written lesson fall back to showing
the spec.

The full CLI, for driving a specific task by id:

```sh
./ctrain list                   # tasks grouped by K&R chapter, with your status
./ctrain learn                  # teach the next new task (or: learn <id>)
./ctrain train                  # auto-pick the next task (review or new)
./ctrain start  hello-world     # print the spec, create work/<task>/main.c
./ctrain check  [id]            # compile + behavioural tests + sanitizers
./ctrain solution [id]          # one reference implementation
./ctrain reset  [id]            # restore starter (your code -> main.c.bak)
./ctrain status                 # XP, daily streak, completion
```

`<id>` is `domain/nn-name` or the unique trailing name; `check`, `solution`
and `reset` default to the task `learn`/`train`/`start` last gave you.

Progress is stored in `~/.local/state/ctrain/progress` (reps and next-review
dates included). XP is awarded on each pass — a full award the first time, a
smaller one for a review; a daily streak keeps you honest.

## What it covers

**97 tasks across 14 domains.** K&R chapters 1–8 as the backbone — tutorial, types & operators, control
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
├── solution.c     one reference implementation (or solution/ for multi-file)
└── learn.md       optional tutor lesson shown by `learn` (see docs/authoring.md)
```

Graders test behaviour, not style: any correct implementation passes.

## Selftest

Every grader is verified against its own artifacts — it must FAIL on the
starter stub and PASS on the reference solution:

```sh
./selftest.sh               # natively
./selftest.sh --container   # in a gcc:14 container (as CI does)
make test                   # unit-test the train/learn selection + SR logic
```

CI runs the full selftest on every pull request.
