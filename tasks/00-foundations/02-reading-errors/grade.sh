#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/receipt" "$SRC" || grade_summary
diff_out "prints the two receipt lines exactly" \
  "$TASK_DIR/expected.out" "$WORK_DIR/receipt"
run_ok "exits with status 0" "$WORK_DIR/receipt"
grade_summary
