#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary

# Mixed valid/invalid input: check stdout, stderr and exit status separately.
in="$WORK_DIR/mixed.in"
cat >"$in" <<'EOF'
3 4
10 -2
1 2 3
7
foo 9
   5    6
12abc 3
EOF
timeout --signal=KILL "${CTRAIN_TIMEOUT:-10}" \
  "$WORK_DIR/prog" <"$in" >"$WORK_DIR/mixed.out" 2>"$WORK_DIR/mixed.err"
status=$?
diff_out "stdout: sums for the valid lines only" \
  "$TASK_DIR/expected.out" cat "$WORK_DIR/mixed.out"
diff_out "stderr: 'line N: bad input' for each bad line" \
  "$TASK_DIR/expected.err" cat "$WORK_DIR/mixed.err"
run_ok "exit status is 1 when any line is bad" test "$status" -eq 1

# All-valid input: clean stdout, exit 0.
expect_out_stdin "sums every valid line" $'1 2\n-3 -4\n0 0\n' \
  $'sum = 3\nsum = -7\nsum = 0' "$WORK_DIR/prog"
run_ok "exit status is 0 when every line parses" \
  bash -c 'printf "1 2\n3 4\n" | "$1" >/dev/null 2>&1' _ "$WORK_DIR/prog"

# The scanf trap: numbers must not be paired up across lines.
expect_out_stdin "does not pair numbers across lines" $'1\n2\n' \
  "" "$WORK_DIR/prog"
run_fails "single-number lines are rejected (non-zero exit)" \
  bash -c 'printf "1\n2\n" | "$1" >/dev/null 2>&1' _ "$WORK_DIR/prog"

# Empty input: nothing printed, success.
expect_out_stdin "empty input prints nothing" "" "" "$WORK_DIR/prog"
run_ok "empty input exits 0" \
  bash -c '"$1" </dev/null >/dev/null 2>&1' _ "$WORK_DIR/prog"

grade_summary
