#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# Explicit prompt rule: no qsort anywhere in the file.
run_fails "qsort does not appear in main.c" grep -q 'qsort' "$SRC"

c_compile "$WORK_DIR/prog" "$TASK_DIR/test_main.c" "$SRC" || grade_summary
run_ok "all harness checks pass" "$WORK_DIR/prog"

c_compile_san "$WORK_DIR/prog-san" "$TASK_DIR/test_main.c" "$SRC" || grade_summary
mem_check "no leaks, overflows, or undefined behaviour" "$WORK_DIR/prog-san"

grade_summary
