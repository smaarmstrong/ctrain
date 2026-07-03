#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary

expect_out_stdin "counts and sorts a one-line input" \
  'the cat sat on the cat mat
' 'cat 2
mat 1
on 1
sat 1
the 2' "$WORK_DIR/prog"

expect_out_stdin "case-sensitive: Cat and cat differ" \
  'Cat cat Cat' 'Cat 2
cat 1' "$WORK_DIR/prog"

expect_out_stdin "empty input prints nothing" '' '' "$WORK_DIR/prog"

diff_out "larger text, exact sorted output" \
  "$TASK_DIR/expected.out" "$WORK_DIR/prog" < "$TASK_DIR/words.txt"

c_compile_san "$WORK_DIR/prog-san" "$SRC" || grade_summary
mem_check "tree fully freed — no leaks or undefined behaviour" \
  "$WORK_DIR/prog-san" < "$TASK_DIR/words.txt"

grade_summary
