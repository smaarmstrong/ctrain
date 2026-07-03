#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary
PROG="$WORK_DIR/prog"
T="${CTRAIN_TIMEOUT:-10}"

five="$WORK_DIR/five.txt"
printf 'l1\nl2\nl3\nl4\nl5\n' >"$five"
three="$WORK_DIR/three.txt"
printf 'aa\nbb\ncc\n' >"$three"
notrail="$WORK_DIR/notrail.txt"
printf 'first\nsecond' >"$notrail"

# --- happy path ---------------------------------------------------------
expect_out "first 2 lines of a 5-line file" $'l1\nl2' "$PROG" 2 "$five"
expect_out "COUNT larger than the file prints it whole" \
  $'aa\nbb\ncc' "$PROG" 10 "$three"
expect_out "several files are concatenated in order" \
  $'l1\nl2\naa\nbb' "$PROG" 2 "$five" "$three"
expect_out "final line without newline counts and is kept as-is" \
  $'first\nsecond' "$PROG" 5 "$notrail"
run_ok "success exits 0" \
  bash -c '"$1" 1 "$2" >/dev/null 2>&1' _ "$PROG" "$five"

# --- usage errors: stderr + exit 2, stdout silent ------------------------
for bad in "" "5" "abc $five" "0 $five" "-3 $five" "7x $five"; do
  # shellcheck disable=SC2086
  run_ok "usage error [args: '${bad:-none}'] exits 2" \
    bash -c '"$1" $2 >/dev/null 2>&1; test $? -eq 2' _ "$PROG" "$bad"
done
timeout --signal=KILL "$T" "$PROG" abc "$five" \
  >"$WORK_DIR/usage.out" 2>"$WORK_DIR/usage.err"
run_ok "usage message goes to stderr and starts with 'usage:'" \
  grep -q '^usage:' "$WORK_DIR/usage.err"
run_ok "usage error prints nothing to stdout" \
  test ! -s "$WORK_DIR/usage.out"

# --- file errors: perror-style diagnostic, keep going, exit 1 ------------
missing="$WORK_DIR/no-such.txt"
rm -f "$missing"
timeout --signal=KILL "$T" "$PROG" 2 "$five" "$missing" "$three" \
  >"$WORK_DIR/mix.out" 2>"$WORK_DIR/mix.err"
status=$?
run_ok "an unopenable file makes the final exit status 1" \
  test "$status" -eq 1
printf 'l1\nl2\naa\nbb\n' >"$WORK_DIR/mix.want"
diff_out "remaining files are still printed, stdout free of diagnostics" \
  "$WORK_DIR/mix.want" cat "$WORK_DIR/mix.out"
run_ok "diagnostic is 'head: FILE: <reason>' on stderr" \
  grep -q "^head: $missing: ." "$WORK_DIR/mix.err"

grade_summary
