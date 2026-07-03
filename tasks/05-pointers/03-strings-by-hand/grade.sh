#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# Explicit prompt rule: no <string.h> helpers, not even in comments.
run_fails "no string.h identifiers in main.c (strlen, strcpy, ...)" \
  grep -Eq '\b(strlen|strcpy|strncpy|strcmp|strncmp|strcat|memcpy|memmove)\b' "$SRC"

c_compile "$WORK_DIR/prog" "$TASK_DIR/test_main.c" "$SRC" || grade_summary
run_ok "all harness checks pass" "$WORK_DIR/prog"

c_compile_san "$WORK_DIR/prog-san" "$TASK_DIR/test_main.c" "$SRC" || grade_summary
mem_check "no leaks, overflows, or undefined behaviour" "$WORK_DIR/prog-san"

grade_summary
