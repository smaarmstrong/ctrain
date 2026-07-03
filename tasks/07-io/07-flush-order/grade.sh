#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary
PROG="$WORK_DIR/prog"
T="${CTRAIN_TIMEOUT:-10}"

expect_out "stdout alone: the three ticks" \
  $'tick 1\ntick 2\ntick 3' "$PROG"

timeout --signal=KILL "$T" "$PROG" >/dev/null 2>"$WORK_DIR/tocks.txt"
printf 'tock 1\ntock 2\ntock 3\n' >"$WORK_DIR/tocks.want"
diff_out "stderr alone: the three tocks" \
  "$WORK_DIR/tocks.want" cat "$WORK_DIR/tocks.txt"

# The real test: both streams into ONE file — order must survive.
timeout --signal=KILL "$T" bash -c '"$1" >"$2" 2>&1' _ "$PROG" "$WORK_DIR/log.txt"
status=$?
printf 'tick 1\ntock 1\ntick 2\ntock 2\ntick 3\ntock 3\n' >"$WORK_DIR/log.want"
diff_out "redirected to one file: ticks and tocks interleave in program order" \
  "$WORK_DIR/log.want" cat "$WORK_DIR/log.txt"
run_ok "exits 0" test "$status" -eq 0

grade_summary
