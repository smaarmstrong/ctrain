#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# 1. build via the learner's Makefile
run_ok "plain 'make' (default target) succeeds" make -C "$WORK_DIR"
run_ok "executable ./calc exists" test -x "$WORK_DIR/calc"
run_ok "main.o was produced" test -f "$WORK_DIR/main.o"
run_ok "ops.o was produced" test -f "$WORK_DIR/ops.o"

# 2. behaviour of the built program
expect_out "./calc 6 7" "6 + 7 = 13
6 * 7 = 42" "$WORK_DIR/calc" 6 7
expect_out "./calc -3 10" "-3 + 10 = 7
-3 * 10 = -30" "$WORK_DIR/calc" -3 10
run_fails "no arguments -> non-zero exit" "$WORK_DIR/calc"

# 3. a second make has nothing to do
run_ok "immediately after a build, 'make -q' reports up to date" \
  make -C "$WORK_DIR" -q

# 4. clean must be .PHONY: plant a decoy file named 'clean'
touch "$WORK_DIR/clean"
run_ok "'make clean' runs despite a file named clean" make -C "$WORK_DIR" clean
run_ok "clean removed the calc binary" bash -c "! test -e '$WORK_DIR/calc'"
run_ok "clean removed every .o file" \
  bash -c "! ls '$WORK_DIR'/*.o >/dev/null 2>&1"

# 5. rebuild from scratch still works
run_ok "'make' rebuilds after clean" make -C "$WORK_DIR"
run_ok "calc is back" test -x "$WORK_DIR/calc"

grade_summary
