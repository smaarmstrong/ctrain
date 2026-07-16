#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# 1. plain make builds the default target
run_ok "plain 'make' (default target) succeeds" make -C "$WORK_DIR"
run_ok "an executable ./greet exists" test -x "$WORK_DIR/greet"

# 2. the built program behaves
expect_out "./greet prints its one line" \
  "built by make, so never stale" "$WORK_DIR/greet"
run_ok "./greet exits with status 0" "$WORK_DIR/greet"

# 3. timestamp logic: nothing to do right after a build...
run_ok "immediately after a build, 'make -q' reports up to date" \
  make -C "$WORK_DIR" -q

# 4. ...and a rebuild is due once the source is newer than the binary
sleep 1  # ensure the touched timestamp is strictly newer
touch "$WORK_DIR/main.c"
run_fails "after 'touch main.c', 'make -q' reports a rebuild is needed" \
  make -C "$WORK_DIR" -q
run_ok "'make' rebuilds after the touch" make -C "$WORK_DIR"

grade_summary
