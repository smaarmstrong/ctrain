#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary
diff_out "output is byte-identical to the original report" \
  "$TASK_DIR/expected.out" "$WORK_DIR/prog"
run_ok "exits with status 0" "$WORK_DIR/prog"
grade_summary
