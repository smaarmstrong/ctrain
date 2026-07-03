#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# --wrap=read lets the harness count the learner's read() syscalls.
c_compile "$WORK_DIR/prog" "$TASK_DIR/test_main.c" "$SRC" -Wl,--wrap=read \
  || grade_summary
run_ok "all harness checks pass" "$WORK_DIR/prog"

c_compile_san "$WORK_DIR/prog-san" "$TASK_DIR/test_main.c" "$SRC" -Wl,--wrap=read \
  || grade_summary
mem_check "no leaks, overflows, or undefined behaviour" "$WORK_DIR/prog-san"

grade_summary
