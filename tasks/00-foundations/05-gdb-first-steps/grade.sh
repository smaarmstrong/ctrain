#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/fact" "$SRC" || grade_summary
diff_out "prints 5! and 7! correctly" \
  "$TASK_DIR/expected.out" "$WORK_DIR/fact"
run_ok "exits with status 0 (no crash)" "$WORK_DIR/fact"
grade_summary
