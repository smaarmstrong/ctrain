#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary

diff_out "sorts a small mixed file" \
  "$TASK_DIR/expected1.out" "$WORK_DIR/prog" <"$TASK_DIR/input1.txt"
diff_out "duplicates, empty lines, missing final newline" \
  "$TASK_DIR/expected2.out" "$WORK_DIR/prog" <"$TASK_DIR/input2.txt"
diff_out "empty input produces no output" \
  "$TASK_DIR/expected-empty.out" "$WORK_DIR/prog" </dev/null

c_compile_san "$WORK_DIR/prog-san" "$SRC" || grade_summary
mem_check "no leaks, overflows, or undefined behaviour" \
  "$WORK_DIR/prog-san" <"$TASK_DIR/input2.txt"

grade_summary
