#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary

expect_out_stdin "mixed positive and negative values" \
  "3 4 -2 10" \
  $'add 3 -> total 3\nadd 4 -> total 7\nadd -2 -> total 5\nadd 10 -> total 15\nfinal total 15 over 4 values' \
  "$WORK_DIR/prog"

expect_out_stdin "a single value" \
  "42" \
  $'add 42 -> total 42\nfinal total 42 over 1 values' \
  "$WORK_DIR/prog"

expect_out_stdin "all negative values" \
  "-1 -2 -3" \
  $'add -1 -> total -1\nadd -2 -> total -3\nadd -3 -> total -6\nfinal total -6 over 3 values' \
  "$WORK_DIR/prog"

expect_out_stdin "empty input" \
  "" \
  "final total 0 over 0 values" \
  "$WORK_DIR/prog"

grade_summary
