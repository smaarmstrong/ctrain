#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# The learner's file provides bounded_copy; the harness provides main().
c_compile "$WORK_DIR/prog" "$TASK_DIR/test_main.c" "$SRC" || grade_summary
run_ok "all harness checks pass" "$WORK_DIR/prog"

# Repeat with exactly-sized heap buffers under ASan/UBSan.
c_compile_san "$WORK_DIR/prog-san" "$TASK_DIR/test_main.c" "$SRC" || grade_summary
mem_check "no out-of-bounds writes, reads, or leaks" "$WORK_DIR/prog-san"

grade_summary
