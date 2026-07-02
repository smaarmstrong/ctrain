#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary
diff_out "prints the full 0–300 °F table, exactly formatted" \
  "$TASK_DIR/expected.out" "$WORK_DIR/prog"
grade_summary
