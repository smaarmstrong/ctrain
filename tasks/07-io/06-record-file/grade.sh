#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary
PROG="$WORK_DIR/prog"
T="${CTRAIN_TIMEOUT:-10}"

zeros() { head -c "$1" /dev/zero; }

# Reference file, built byte-by-byte by the grader (little-endian):
#   bolt 7 / gasket 300 / cam 70000 / washer-large 4294967295
fixture="$WORK_DIR/fixture.bin"
{
  printf 'bolt';         zeros 12; printf '\x07';         zeros 3
  printf 'gasket';       zeros 10; printf '\x2c\x01';     zeros 2
  printf 'cam';          zeros 13; printf '\x70\x11\x01'; zeros 1
  printf 'washer-large'; zeros 4;  printf '\xff\xff\xff\xff'
} >"$fixture"

# --- write: exact binary layout ------------------------------------------
got="$WORK_DIR/got.bin"
run_ok "write: accepts 4 records from stdin" \
  bash -c 'printf "bolt 7\ngasket 300\ncam 70000\nwasher-large 4294967295\n" \
           | "$1" write "$2"' _ "$PROG" "$got"
run_ok "write: file is exactly 20 bytes per record (80 bytes)" \
  bash -c 'test "$(wc -c <"$1")" -eq 80' _ "$got"
run_ok "write: byte-for-byte little-endian layout" cmp -s "$fixture" "$got"

# --- get: random access on the grader-built file --------------------------
expect_out "get 0: first record"  "bolt 7"       "$PROG" get "$fixture" 0
expect_out "get 2: middle record" "cam 70000"    "$PROG" get "$fixture" 2
expect_out "get 3: 32-bit quantity survives the round trip" \
  "washer-large 4294967295" "$PROG" get "$fixture" 3
run_ok "get: exits 0 on success" \
  bash -c '"$1" get "$2" 1 >/dev/null 2>&1' _ "$PROG" "$fixture"

# --- round trip through the learner's own writer ---------------------------
mine="$WORK_DIR/mine.bin"
run_ok "round trip: write then get" \
  bash -c 'printf "nut 12\nbracket 9000\n" | "$1" write "$2" &&
           test "$("$1" get "$2" 1)" = "bracket 9000"' _ "$PROG" "$mine"

# --- errors ---------------------------------------------------------------
timeout --signal=KILL "$T" "$PROG" get "$fixture" 4 \
  >"$WORK_DIR/oob.out" 2>"$WORK_DIR/oob.err"
status=$?
run_ok "get past the end: exit status 1" test "$status" -eq 1
run_ok "get past the end: 'record-file: no record 4' on stderr" \
  grep -q '^record-file: no record 4' "$WORK_DIR/oob.err"
run_ok "get past the end: nothing on stdout" test ! -s "$WORK_DIR/oob.out"

missing="$WORK_DIR/absent.bin"
rm -f "$missing"
timeout --signal=KILL "$T" "$PROG" get "$missing" 0 \
  >"$WORK_DIR/miss.out" 2>"$WORK_DIR/miss.err"
status=$?
run_ok "unopenable file: exit status 1" test "$status" -eq 1
run_ok "unopenable file: 'record-file: cannot open ...' on stderr" \
  grep -q "^record-file: cannot open $missing" "$WORK_DIR/miss.err"

for args in "" "frob x" "get" "write" "get $fixture -1" "get $fixture 1x"; do
  # shellcheck disable=SC2086
  run_ok "usage error [args: '${args:-none}'] exits 2" \
    bash -c '"$1" $2 >/dev/null 2>&1; test $? -eq 2' _ "$PROG" "$args"
done

# --- memory-checked pass ---------------------------------------------------
c_compile_san "$WORK_DIR/prog-san" "$SRC" || grade_summary
mem_check "get is clean under sanitizers" "$WORK_DIR/prog-san" get "$fixture" 2
mem_check "write is clean under sanitizers" \
  bash -c 'printf "bolt 7\n" | "$1" write "$2"' _ "$WORK_DIR/prog-san" "$WORK_DIR/san.bin"

grade_summary
