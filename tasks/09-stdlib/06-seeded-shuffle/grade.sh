#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary

# Build the reference from the task's own solution with the same libc,
# so rand()'s sequence is identical for both binaries.
c_compile "$WORK_DIR/ref" "$TASK_DIR/solution.c" || grade_summary

for pair in "1 8" "2 8" "42 20" "12345 100" "7 1"; do
  set -- $pair
  seed="$1" n="$2"
  want="$("$WORK_DIR/ref" "$seed" "$n")"
  expect_out "matches the reference shuffle for SEED=$seed N=$n" \
    "$want" "$WORK_DIR/prog" "$seed" "$n"
done

first_run="$(timeout --signal=KILL "${CTRAIN_TIMEOUT:-10}" "$WORK_DIR/prog" 42 20)"
expect_out "same seed twice gives the same order" \
  "$first_run" "$WORK_DIR/prog" 42 20

run_fails "no arguments -> exit status 1" "$WORK_DIR/prog"
run_fails "one argument -> exit status 1" "$WORK_DIR/prog" 5
run_fails "N = 0 -> exit status 1" "$WORK_DIR/prog" 1 0
run_fails "N = 1001 -> exit status 1" "$WORK_DIR/prog" 1 1001
run_fails "non-numeric N -> exit status 1" "$WORK_DIR/prog" 1 12x
run_fails "non-numeric SEED -> exit status 1" "$WORK_DIR/prog" seed 5

c_compile_san "$WORK_DIR/prog-san" "$SRC" || grade_summary
mem_check "shuffle run is memory-clean" "$WORK_DIR/prog-san" 42 20

grade_summary
