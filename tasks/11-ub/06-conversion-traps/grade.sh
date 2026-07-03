#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# The learner's file provides the two functions; the harness provides main().
c_compile "$WORK_DIR/prog" "$TASK_DIR/test_main.c" "$SRC" || grade_summary
run_ok "all harness checks pass" "$WORK_DIR/prog"

# Repeat under ASan/UBSan: a wrapped loop bound reads one element past an
# exactly-sized heap block and aborts immediately.
c_compile_san "$WORK_DIR/prog-san" "$TASK_DIR/test_main.c" "$SRC" || grade_summary
mem_check "no out-of-bounds reads or undefined behaviour" "$WORK_DIR/prog-san"

grade_summary
