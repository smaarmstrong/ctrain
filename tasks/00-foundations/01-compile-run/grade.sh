#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/cycle" "$SRC" || grade_summary
diff_out "prints exactly 'source -> binary -> output' and a newline" \
  "$TASK_DIR/expected.out" "$WORK_DIR/cycle"
run_ok "exits with status 0" "$WORK_DIR/cycle"
grade_summary
