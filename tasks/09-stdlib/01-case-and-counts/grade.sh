#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary

expect_out_stdin "swaps case and counts a simple line" \
  $'Hello, World 42!\n' \
  $'hELLO, wORLD 42!\nletters=10 digits=2 spaces=3 other=2' \
  "$WORK_DIR/prog"

expect_out_stdin "handles tabs, several lines, mixed case" \
  $'abcXYZ 123\n\tmixed UP and down!\n' \
  $'ABCxyz 123\n\tMIXED up AND DOWN!\nletters=20 digits=3 spaces=7 other=1' \
  "$WORK_DIR/prog"

expect_out_stdin "empty input prints only the zero summary" \
  '' \
  'letters=0 digits=0 spaces=0 other=0' \
  "$WORK_DIR/prog"

expect_out_stdin "a byte above 127 is 'other' and echoed unchanged" \
  $'Caf\xe9 5\n' \
  $'cAF\xe9 5\nletters=3 digits=1 spaces=2 other=1' \
  "$WORK_DIR/prog"

grade_summary
