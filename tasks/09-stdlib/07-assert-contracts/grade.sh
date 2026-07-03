#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# ---- build 1: asserts enabled ------------------------------------------------
c_compile "$WORK_DIR/prog" "$TASK_DIR/test_main.c" "$SRC" || grade_summary
run_ok "clamped: normal behaviour (valid preconditions)" "$WORK_DIR/prog" clamp
run_ok "parse_port: accepts and rejects correctly" "$WORK_DIR/prog" port
run_fails "clamped(1, 5, 2): violated precondition aborts (asserts on)" \
  "$WORK_DIR/prog" clamp-bad

# ---- build 2: -DNDEBUG — the contract check must compile away ---------------
c_compile "$WORK_DIR/prog-nd" -DNDEBUG "$TASK_DIR/test_main.c" "$SRC" || grade_summary
run_ok "NDEBUG: the same violation no longer aborts" "$WORK_DIR/prog-nd" clamp-bad
run_ok "NDEBUG: parse_port validation still works (not an assert!)" \
  "$WORK_DIR/prog-nd" port

# ---- sanitizers (normal build, valid inputs only) ----------------------------
c_compile_san "$WORK_DIR/prog-san" "$TASK_DIR/test_main.c" "$SRC" || grade_summary
mem_check "clamp path is memory-clean" "$WORK_DIR/prog-san" clamp
mem_check "port path is memory-clean" "$WORK_DIR/prog-san" port

grade_summary
