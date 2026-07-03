#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary

expect_out_stdin "squeezes an interior run of spaces" \
  $'a   b\n' $'a b' "$WORK_DIR/prog"
expect_out_stdin "leading spaces become one space" \
  $'   lead\n' $' lead' "$WORK_DIR/prog"
expect_out_stdin "single spaces are untouched" \
  $'one two three\n' $'one two three' "$WORK_DIR/prog"
expect_out_stdin "tabs pass through and are not squeezed" \
  $'a \t x\tb\t\tc\n' $'a \t x\tb\t\tc' "$WORK_DIR/prog"
expect_out_stdin "multiple lines, runs squeezed on each" \
  $'x  y\n\n  z   w\n' $'x y\n\n z w' "$WORK_DIR/prog"
expect_out_stdin "input without a trailing newline" \
  'ends  here' 'ends here' "$WORK_DIR/prog"
expect_out_stdin "empty input produces empty output" \
  '' '' "$WORK_DIR/prog"
run_ok "exits with status 0 on empty input" "$WORK_DIR/prog" </dev/null

grade_summary
