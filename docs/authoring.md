# Authoring tasks

Read this before adding or changing a task. The selftest and CI hold every
task to these rules.

## Anatomy

```
tasks/<domain>/<nn-name>/
├── meta.json      required
├── prompt.md      required — the complete spec
├── grade.sh       required — sources lib/grade.sh
├── solution.c     required (or solution/ for multi-file tasks)
├── starter.c      recommended (or starter/ for multi-file tasks)
├── test_main.c    for function tasks: harness that provides main()
├── expected.out   for diff-graded tasks (generate from solution.c)
└── learn.md       optional — the interactive lesson shown by `ctrain learn`
```

`meta.json` is a flat object:

```json
{
  "title": "short imperative title",
  "domain": "Ch5 · Pointers & Arrays",
  "objective": "one line: the skill this proves",
  "difficulty": "intro | core | advanced | bonus",
  "est_min": 15
}
```

Optionally add `"prereq": ["00-foundations/04-reading-a-crash"]` — ids of
foundations lessons the task quietly leans on. If the learner hasn't passed
one yet, `learn`/`train` print a one-line "learn it first" nudge. Advisory
only: it never gates or reorders anything, and it goes silent once the
lesson is passed. Point at the toolchain skill the task assumes, not at the
previous task in the sequence (the curriculum order already handles that).

## The grading contract

`grade.sh` runs with these environment variables (set by the runner and by
selftest.sh):

- `CTRAIN_ROOT` — repo root; source helpers with `. "$CTRAIN_ROOT/lib/grade.sh"`
- `TASK_DIR` — the task directory (read-only: never write into it)
- `WORK_DIR` — the learner's directory; build artifacts go here or in mktemp
- `SRC` — `$WORK_DIR/main.c`, the learner's primary file

Two grading modes, composable:

1. **Whole-program**: `c_compile "$WORK_DIR/prog" "$SRC"`, then run it —
   `diff_out` against an expected file, `expect_out`/`expect_out_stdin` for
   exact matches, args via the command tail.
2. **Function harness**: the task ships `test_main.c` with `main()` and
   assertions; the prompt says "do not define main". Compile both:
   `c_compile "$WORK_DIR/prog" "$TASK_DIR/test_main.c" "$SRC"`.

For any task touching pointers, arrays, or allocation, add the sanitized
pass:

```sh
c_compile_san "$WORK_DIR/prog-san" "$TASK_DIR/test_main.c" "$SRC" || grade_summary
mem_check "no leaks, overflows, or undefined behaviour" "$WORK_DIR/prog-san"
```

End every grader with `grade_summary`. Bail out early after a failed
compile with `c_compile ... || grade_summary`.

## Quality bars (selftest-enforced or reviewed)

- **Behaviour, not style.** Any correct implementation must pass. Never
  grep the learner's source for tokens, except to enforce an explicit rule
  stated in the prompt (e.g. "without using `strlen`") — and prefer a
  harness design that makes cheating impossible over source inspection.
- **The prompt is the whole spec.** Exact output formats (show a sample),
  exit statuses, edge cases the grader will probe. If the grader checks it,
  the prompt must have said it.
- **Starter fails, solution passes.** The starter should compile cleanly
  (use `(void)param;` for unused parameters) but fail at least one
  *behavioural* check — never rely on sanitizer output alone to fail the
  starter, since native runs may lack ASan.
- **Deterministic.** No randomness without a fixed seed, no wall-clock or
  locale dependence, no network. Sort anything readdir-ordered.
- **Timeouts are built in.** Helpers cap learner code at `$CTRAIN_TIMEOUT`
  (default 10s); keep test inputs small enough to finish in ~1s.
- **Original wording.** K&R exercises appear structurally (the trainer
  mirrors the book's progression) but every prompt is written from scratch
  in this repo's own words. Never reproduce the book's prose or its code.

## `learn.md` (optional lesson)

The interactive lesson shown by `ctrain learn` before the learner attempts the
task — for people meeting the material for the first time, not just being
tested on it. If it's absent, `learn` falls back to showing the prompt and
offering to set the task up.

`learn` reads it as a **tutor script** and walks the learner through it, so
write plain readable prose plus two markers:

- A line that is exactly `---` is a **pause**: the tutor prints everything
  above it, then waits ("Enter to continue") before going on. Use it to break
  the lecture into digestible beats.
- A fenced ```` ```run ```` block is a **step to try**. Unlike the other
  trainers, ctrain's run block is a **complete, standalone C program**: the
  tutor writes it to a temp file, compiles it (`cc -std=c11 -Wall`) and runs
  it, so the learner sees real output. Each block must therefore include its
  own `#include`s and a `main`. Keep a block to one idea.

Everything else prints as-is, so use light structure (`$ command` for
illustrative snippets), not heavy Markdown. The house style is five sections
interleaved with pauses and run steps: **THE IDEA / WHY IT MATTERS / HOW TO DO
IT / CHECK IT WORKED / GOTCHAS**.

**Teach for a near-total beginner.** Never make the learner run something they
couldn't reconstruct from understanding: introduce each concept inline before
using it, and prefer the simplest form over clever idioms (no unexplained
pointer arithmetic, macro tricks or terse one-liners dropped cold). A good
lesson teaches the *skill* so the solution becomes obvious; it is never just a
reworded `solution.c`. See `tasks/01-tutorial/` for worked examples.

## Verifying

```sh
./selftest.sh 05-pointers/03-your-task      # native: fast iteration
./selftest.sh --container 05-pointers/03-your-task   # as CI runs it
make test                                   # the runner's own unit tests
```

Native runs on machines without libasan degrade `mem_check` to a plain run
(you'll see a yellow note) — the container run is the real thing.

Syntax-check a lesson's run blocks the way the tutor will: they must each
compile and run as a standalone program.
