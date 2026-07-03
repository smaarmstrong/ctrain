#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary

# diff_out is byte-exact, so trailing-newline mistakes are caught.
diff_out "three plain arguments" \
  "$TASK_DIR/expected-three.out" "$WORK_DIR/prog" hello brave world
diff_out "a single argument" \
  "$TASK_DIR/expected-one.out" "$WORK_DIR/prog" one
diff_out "option-looking and space-containing arguments pass through" \
  "$TASK_DIR/expected-tricky.out" "$WORK_DIR/prog" "two words" -n --
diff_out "no arguments: no output at all" \
  "$TASK_DIR/expected-none.out" "$WORK_DIR/prog"
run_ok "exit status is 0 with no arguments" "$WORK_DIR/prog"
run_ok "exit status is 0 with arguments" "$WORK_DIR/prog" a b

c_compile_san "$WORK_DIR/prog-san" "$SRC" || grade_summary
mem_check "no leaks, overflows, or undefined behaviour" \
  "$WORK_DIR/prog-san" alpha "b c" gamma

grade_summary
