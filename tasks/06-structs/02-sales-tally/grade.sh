#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary

expect_out_stdin "two products, first-seen order, sums aggregated" \
  'widget 5
gadget 3
widget 2
gadget -1
' 'widget 7
gadget 2
distinct 2' "$WORK_DIR/prog"

expect_out_stdin "single product repeated" \
  'bolt 1 bolt 1 bolt 1' 'bolt 3
distinct 1' "$WORK_DIR/prog"

expect_out_stdin "negative totals are still printed" \
  'nut 2
nut -5
' 'nut -3
distinct 1' "$WORK_DIR/prog"

expect_out_stdin "empty input prints distinct 0" \
  '' 'distinct 0' "$WORK_DIR/prog"

diff_out "larger mixed ledger, exact output" \
  "$TASK_DIR/expected.out" "$WORK_DIR/prog" < "$TASK_DIR/data.txt"

c_compile_san "$WORK_DIR/prog-san" "$SRC" || grade_summary
mem_check "no overflows or undefined behaviour on the ledger" \
  "$WORK_DIR/prog-san" < "$TASK_DIR/data.txt"

grade_summary
