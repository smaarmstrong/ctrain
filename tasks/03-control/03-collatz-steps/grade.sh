#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary

diff_out "step counts, invalid inputs, and 64-bit intermediates" \
  "$TASK_DIR/expected.out" "$WORK_DIR/prog" < "$TASK_DIR/input.txt"

expect_out_stdin "sample from the prompt" "6 1 0 7" \
"6: 8
1: 0
0: invalid
7: 16" "$WORK_DIR/prog"

expect_out_stdin "empty input prints nothing" "" "" "$WORK_DIR/prog"

grade_summary
