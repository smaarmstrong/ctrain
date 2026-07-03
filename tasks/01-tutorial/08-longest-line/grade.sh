#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary

expect_out_stdin "picks the longest of several lines" \
  $'short\nthe longest line\nmid\n' $'16\nthe longest line' "$WORK_DIR/prog"
expect_out_stdin "single line" \
  $'only\n' $'4\nonly' "$WORK_DIR/prog"
expect_out_stdin "last line without a newline still counts" \
  'unterminated but longest' $'24\nunterminated but longest' "$WORK_DIR/prog"
expect_out_stdin "tie goes to the first line" \
  $'aa\nbb\n' $'2\naa' "$WORK_DIR/prog"
expect_out_stdin "a lone empty line reports length 0" \
  $'\n' '0' "$WORK_DIR/prog"
expect_out_stdin "empty input prints nothing" \
  '' '' "$WORK_DIR/prog"
run_ok "exits 0 on empty input" "$WORK_DIR/prog" </dev/null
diff_out "long-line file, byte-exact output" \
  "$TASK_DIR/expected.out" "$WORK_DIR/prog" <"$TASK_DIR/input.txt"

c_compile_san "$WORK_DIR/prog-san" "$SRC" || grade_summary
mem_check "no buffer overflows or undefined behaviour" \
  "$WORK_DIR/prog-san" <"$TASK_DIR/input.txt"

grade_summary
