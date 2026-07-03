#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# The learner's file provides the stack; the harness provides main().
c_compile "$WORK_DIR/prog" "$TASK_DIR/test_main.c" "$SRC" || grade_summary
run_ok "all harness checks pass" "$WORK_DIR/prog"

# The full session again under ASan/UBSan: use-after-free, double free
# and leaked strings all fail here.
c_compile_san "$WORK_DIR/prog-san" "$TASK_DIR/test_main.c" "$SRC" || grade_summary
mem_check "no use-after-free, double free, or leaks" "$WORK_DIR/prog-san"

grade_summary
