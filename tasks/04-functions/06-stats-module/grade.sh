#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# 1. The whole program: driver + module, compiled separately, linked.
c_compile "$WORK_DIR/prog" "$WORK_DIR/main.c" "$WORK_DIR/stats.c" || grade_summary

expect_out_stdin "stats of 5 -3 12 0 6" "5 -3 12 0 6" \
  $'count 5\nmin -3\nmax 12\nsum 20\nmean 4.00' "$WORK_DIR/prog"

expect_out_stdin "stats of a single value" "-8" \
  $'count 1\nmin -8\nmax -8\nsum -8\nmean -8.00' "$WORK_DIR/prog"

expect_out_stdin "stats of 1..8" "1 2 3 4 5 6 7 8" \
  $'count 8\nmin 1\nmax 8\nsum 36\nmean 4.50' "$WORK_DIR/prog"

expect_out_stdin "empty input prints no data" "" "no data" "$WORK_DIR/prog"
run_fails "empty input exits with a non-zero status" \
  bash -c "\"$WORK_DIR/prog\" </dev/null"

# 2. Include-guard + direct API harness: stats.h is included twice there.
c_compile "$WORK_DIR/prog-guard" "$TASK_DIR/guard_main.c" "$WORK_DIR/stats.c" \
  -I"$WORK_DIR" || grade_summary
run_ok "stats.h survives double inclusion and the API checks pass" \
  "$WORK_DIR/prog-guard"

# 3. Same harness under sanitizers.
c_compile_san "$WORK_DIR/prog-san" "$TASK_DIR/guard_main.c" "$WORK_DIR/stats.c" \
  -I"$WORK_DIR" || grade_summary
mem_check "no leaks, overflows, or undefined behaviour" "$WORK_DIR/prog-san"

grade_summary
