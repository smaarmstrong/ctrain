#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary
diff_out "prints the dispenser log in the specified order" \
  "$TASK_DIR/expected.out" "$WORK_DIR/prog"
run_ok "exits with status 0" "$WORK_DIR/prog"

# Under UBSan an unsequenced-modification rewrite gone wrong shows up here.
c_compile_san "$WORK_DIR/prog-san" "$SRC" || grade_summary
mem_check "no undefined behaviour while dispensing" "$WORK_DIR/prog-san"

grade_summary
