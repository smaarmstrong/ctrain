#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# Explicit rule from the prompt: build the digits yourself.
run_fails "does not call sprintf/snprintf (prompt rule)" \
  grep -qE '(^|[^A-Za-z0-9_])sn?printf([^A-Za-z0-9_]|$)' "$SRC"

# The learner's file provides the function; the harness provides main().
c_compile "$WORK_DIR/prog" "$TASK_DIR/test_main.c" "$SRC" || grade_summary
run_ok "all harness checks pass" "$WORK_DIR/prog"

c_compile_san "$WORK_DIR/prog-san" "$TASK_DIR/test_main.c" "$SRC" || grade_summary
mem_check "no buffer overruns, signed overflow, or leaks" "$WORK_DIR/prog-san"

grade_summary
