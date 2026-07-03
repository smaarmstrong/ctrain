#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/revlines" "$SRC" || grade_summary

expect_out_stdin "reverses three lines and counts them" \
  'alpha
beta
gamma
' 'gamma
beta
alpha
-- 3 lines' "$WORK_DIR/revlines"

expect_out_stdin "a single line comes back unchanged" \
  'only
' 'only
-- 1 lines' "$WORK_DIR/revlines"

expect_out_stdin "empty input prints just the count" \
  '' '-- 0 lines' "$WORK_DIR/revlines"

# the fix must hold under a leak checker
c_compile_san "$WORK_DIR/revlines-san" "$SRC" || grade_summary
IN="$WORK_DIR/.leak-hunt-input.txt"
printf 'one\ntwo\nthree\nfour\n' > "$IN"
mem_check "no leaks or memory errors while reversing" \
  "$WORK_DIR/revlines-san" < "$IN"

grade_summary
