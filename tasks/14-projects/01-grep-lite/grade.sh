#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$WORK_DIR"/*.c || grade_summary
PROG="$WORK_DIR/prog"
T="${CTRAIN_TIMEOUT:-10}"

F="$WORK_DIR/.fixtures"
rm -rf "$F"
mkdir -p "$F"
poem="$F/poem.txt"
printf 'the quick brown fox\njumps over\nthe lazy dog\nend\n' >"$poem"
a="$F/a.txt"
printf 'cat\ndog\n' >"$a"
b="$F/b.txt"
printf 'dog\nbird\n' >"$b"
nt="$F/notrail.txt"
printf 'first\nsecond has dog' >"$nt"

# --- basic matching, one file --------------------------------------------
expect_out "prints the lines containing PATTERN" \
  $'the quick brown fox\nthe lazy dog' "$PROG" the "$poem"
expect_out "-n prefixes 1-based line numbers" \
  $'1:the quick brown fox\n3:the lazy dog' "$PROG" -n the "$poem"
expect_out "-c prints only the count of selected lines" \
  "2" "$PROG" -c the "$poem"
expect_out "-c prints 0 when nothing matches" \
  "0" "$PROG" -c zebra "$poem"
expect_out "-v selects the non-matching lines" \
  $'jumps over\nend' "$PROG" -v the "$poem"
expect_out "-v -c counts the non-matching lines" \
  "2" "$PROG" -v -c the "$poem"
expect_out "-c -n: -n has no effect when counting" \
  "2" "$PROG" -c -n the "$poem"

# --- stdin and the empty pattern -----------------------------------------
expect_out_stdin "no FILE arguments means filter stdin" \
  $'one\ntwo\nthree\n' $'one\ntwo' "$PROG" o
expect_out_stdin "an empty pattern matches every line" \
  $'x\ny\n' $'x\ny' "$PROG" ""

# --- two or more files: name prefixes ------------------------------------
expect_out "two files: every line gets a FILE: prefix" \
  "$a:dog
$b:dog" "$PROG" dog "$a" "$b"
expect_out "two files with -n: FILE:LINENO:line" \
  "$a:2:dog
$b:1:dog" "$PROG" -n dog "$a" "$b"
expect_out "two files with -c: one FILE:COUNT line per file, in order" \
  "$a:1
$b:1" "$PROG" -c dog "$a" "$b"

# --- final line without a trailing newline still ends in \n --------------
printf 'second has dog\n' >"$F/nt.want"
diff_out "a last line with no trailing newline is printed with one" \
  "$F/nt.want" "$PROG" dog "$nt"

# --- exit statuses ---------------------------------------------------------
run_ok "at least one selected line exits 0" \
  bash -c '"$1" the "$2" >/dev/null 2>&1' _ "$PROG" "$poem"
run_ok "no selected lines (no error) exits 1" \
  bash -c '"$1" zebra "$2" >/dev/null 2>&1; test $? -eq 1' _ "$PROG" "$poem"

timeout --signal=KILL "$T" "$PROG" >"$F/usage.out" 2>"$F/usage.err"
status=$?
run_ok "missing PATTERN exits 2" test "$status" -eq 2
run_ok "usage message goes to stderr" test -s "$F/usage.err"
run_ok "usage error prints nothing to stdout" test ! -s "$F/usage.out"
run_ok "only options and no PATTERN is still a usage error (exit 2)" \
  bash -c '"$1" -n -v >/dev/null 2>&1; test $? -eq 2' _ "$PROG"

# --- unopenable file: diagnose, keep going, exit 2 ------------------------
missing="$F/no-such-file.txt"
rm -f "$missing"
timeout --signal=KILL "$T" "$PROG" dog "$missing" "$a" \
  >"$F/mix.out" 2>"$F/mix.err"
status=$?
run_ok "an unopenable file makes the exit status 2 despite matches" \
  test "$status" -eq 2
printf '%s:dog\n' "$a" >"$F/mix.want"
diff_out "remaining files still searched; two file args keep the prefix" \
  "$F/mix.want" cat "$F/mix.out"
run_ok "the unopenable file gets a message on stderr" test -s "$F/mix.err"

# --- memory safety ---------------------------------------------------------
c_compile_san "$WORK_DIR/prog-san" "$WORK_DIR"/*.c || grade_summary
mem_check "no leaks, overflows, or undefined behaviour" \
  "$WORK_DIR/prog-san" -n the "$poem" "$a"

grade_summary
