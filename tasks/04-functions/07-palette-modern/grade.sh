#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# The harness includes the learner's palette.h (-I) and provides main().
c_compile "$WORK_DIR/prog" "$TASK_DIR/test_main.c" "$WORK_DIR/palette.c" \
  -I"$WORK_DIR" || grade_summary
run_ok "all harness checks pass" "$WORK_DIR/prog"

c_compile_san "$WORK_DIR/prog-san" "$TASK_DIR/test_main.c" "$WORK_DIR/palette.c" \
  -I"$WORK_DIR" || grade_summary
mem_check "no leaks, overflows, or undefined behaviour" "$WORK_DIR/prog-san"

grade_summary
