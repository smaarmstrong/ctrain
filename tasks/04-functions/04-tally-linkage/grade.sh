#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# The harness defines globals named count/sum/total/min/max on purpose:
# learner state with external linkage and one of those names fails to link.
c_compile "$WORK_DIR/prog" "$TASK_DIR/test_main.c" "$SRC" || grade_summary
run_ok "all harness checks pass" "$WORK_DIR/prog"
grade_summary
