#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# The harness #includes the learner's main.c (found via -I).
c_compile "$WORK_DIR/prog" "$TASK_DIR/test_main.c" -I"$WORK_DIR" || grade_summary
diff_out "every macro prints exactly the expected text" \
  "$TASK_DIR/expected.out" "$WORK_DIR/prog"

c_compile_san "$WORK_DIR/prog-san" "$TASK_DIR/test_main.c" -I"$WORK_DIR" || grade_summary
mem_check "no undefined behaviour in any expansion" "$WORK_DIR/prog-san"

grade_summary
