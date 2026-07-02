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
└── expected.out   for diff-graded tasks (generate from solution.c)
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

## Verifying

```sh
./selftest.sh 05-pointers/03-your-task      # native: fast iteration
./selftest.sh --container 05-pointers/03-your-task   # as CI runs it
```

Native runs on machines without libasan degrade `mem_check` to a plain run
(you'll see a yellow note) — the container run is the real thing.
