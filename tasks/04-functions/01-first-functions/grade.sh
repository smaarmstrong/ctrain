#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# The learner's file provides the functions; the harness provides main().
c_compile "$WORK_DIR/prog" "$TASK_DIR/test_main.c" "$SRC" || grade_summary
run_ok "all harness checks pass" "$WORK_DIR/prog"
grade_summary
