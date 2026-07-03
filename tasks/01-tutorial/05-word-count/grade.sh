#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary

expect_out_stdin "empty input counts 0 0 0" \
  '' '0 0 0' "$WORK_DIR/prog"
expect_out_stdin "one line, two words" \
  $'hello world\n' '1 2 12' "$WORK_DIR/prog"
expect_out_stdin "two lines, three words" \
  $'one\ntwo three\n' '2 3 14' "$WORK_DIR/prog"
expect_out_stdin "mixed whitespace between words" \
  $' a\t bc \n' '1 2 8' "$WORK_DIR/prog"
expect_out_stdin "no trailing newline: word counts, line does not" \
  'abc' '0 1 3' "$WORK_DIR/prog"
expect_out_stdin "blank line adds a line but no words" \
  $'x\n\ny\n' '3 2 5' "$WORK_DIR/prog"
expect_out_stdin "punctuation stays inside a word" \
  $'don\'t stop\n' '1 2 11' "$WORK_DIR/prog"

grade_summary
