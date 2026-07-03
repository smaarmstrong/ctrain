#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

run_ok "'make' builds both binaries" make -C "$WORK_DIR"
run_ok "app-debug exists" test -x "$WORK_DIR/app-debug"
run_ok "app-release exists" test -x "$WORK_DIR/app-release"

# happy path: same stdout from both builds
expect_out "app-debug 49 -> 7" "7" "$WORK_DIR/app-debug" 49
expect_out "app-release 49 -> 7" "7" "$WORK_DIR/app-release" 49

# the trace exists only in the debug build (stderr)
run_ok "app-debug prints its stderr trace" \
  bash -c '"$1" 49 2>&1 >/dev/null | grep -q "debug: parsing" ' _ "$WORK_DIR/app-debug"
run_ok "app-release is silent on stderr" \
  bash -c '[ -z "$("$1" 49 2>&1 >/dev/null)" ]' _ "$WORK_DIR/app-release"

# negative input: assert aborts in debug, fallback answers in release
run_fails "app-debug -9 aborts (assert active)" "$WORK_DIR/app-debug" -9
run_ok "app-debug -9 prints no result line" \
  bash -c '! "$1" -9 2>/dev/null | grep -q "no result"' _ "$WORK_DIR/app-debug"
expect_out "app-release -9 -> 'no result' (assert compiled out)" \
  "no result" "$WORK_DIR/app-release" -9
run_ok "app-release -9 exits with status 1" \
  bash -c '"$1" -9 >/dev/null 2>&1; [ $? -eq 1 ]' _ "$WORK_DIR/app-release"

# clean is phony and thorough
touch "$WORK_DIR/clean"
run_ok "'make clean' runs despite a file named clean" make -C "$WORK_DIR" clean
run_ok "clean removed both binaries" \
  bash -c '! test -e "$1/app-debug" && ! test -e "$1/app-release"' _ "$WORK_DIR"
rm -f "$WORK_DIR/clean"

grade_summary
