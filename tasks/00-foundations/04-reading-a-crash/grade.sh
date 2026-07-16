#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/settings" "$SRC" || grade_summary
diff_out "prints the three values (with '(missing)' for the bare key)" \
  "$TASK_DIR/expected.out" "$WORK_DIR/settings"
run_ok "exits with status 0 (no crash)" "$WORK_DIR/settings"

# the point of the lesson: the fix must satisfy the sanitizer too
c_compile_san "$WORK_DIR/settings-san" "$SRC" || grade_summary
mem_check "no invalid reads or undefined behaviour" "$WORK_DIR/settings-san"
grade_summary
