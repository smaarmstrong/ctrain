#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary

expect_out_stdin "empty input prints all zeros" \
  '' $'digits: 0 0 0 0 0 0 0 0 0 0\nwhite: 0\nother: 0' "$WORK_DIR/prog"
expect_out_stdin "mixed text is classified correctly" \
  $'room 101, floor 3\n' \
  $'digits: 1 2 0 1 0 0 0 0 0 0\nwhite: 4\nother: 10' "$WORK_DIR/prog"
expect_out_stdin "every digit counted in its own slot" \
  $'9876543210 0\t9\n' \
  $'digits: 2 1 1 1 1 1 1 1 1 2\nwhite: 3\nother: 0' "$WORK_DIR/prog"
expect_out_stdin "non-ASCII bytes count as other" \
  $'caf\xc3\xa9 9\n' \
  $'digits: 0 0 0 0 0 0 0 0 0 1\nwhite: 2\nother: 5' "$WORK_DIR/prog"
diff_out "larger sample file, exact output" \
  "$TASK_DIR/expected.out" "$WORK_DIR/prog" <"$TASK_DIR/input.txt"

c_compile_san "$WORK_DIR/prog-san" "$SRC" || grade_summary
mem_check "no overflows or undefined behaviour on the sample file" \
  "$WORK_DIR/prog-san" <"$TASK_DIR/input.txt"

grade_summary
