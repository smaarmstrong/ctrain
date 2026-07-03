#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# 1. The program itself still builds and behaves identically.
c_compile "$WORK_DIR/prog" "$SRC" -I"$WORK_DIR" || grade_summary
diff_out "program output is unchanged" "$TASK_DIR/expected.out" "$WORK_DIR/prog"

# 2. The headers survive repeated and reordered inclusion.
c_compile "$WORK_DIR/tprog" "$TASK_DIR/test_main.c" -I"$WORK_DIR" || grade_summary
run_ok "headers work when included twice, in both orders" "$WORK_DIR/tprog"

c_compile_san "$WORK_DIR/tprog-san" "$TASK_DIR/test_main.c" -I"$WORK_DIR" || grade_summary
mem_check "no undefined behaviour" "$WORK_DIR/tprog-san"

grade_summary
