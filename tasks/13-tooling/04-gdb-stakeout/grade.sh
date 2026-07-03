#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# the ledger must be intact: compiles clean, prints the reference output
c_compile "$WORK_DIR/ledger" "$WORK_DIR/main.c" || grade_summary
expect_out "ledger still prints its original two lines" \
  "processed 20 records
final digest: 519" "$WORK_DIR/ledger"

# the stakeout result
run_ok "answer.txt exists" test -f "$WORK_DIR/answer.txt"
ANS="$(tr -d '[:space:]' < "$WORK_DIR/answer.txt" 2>/dev/null)"
run_ok "answer.txt holds the accumulator value after record 13" \
  test "$ANS" = "11638473647149304983"

grade_summary
