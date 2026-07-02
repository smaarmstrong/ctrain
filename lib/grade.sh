#!/usr/bin/env bash
# lib/grade.sh — shared helpers for ctrain task graders.
#
# Every task's grade.sh sources this, then uses:
#
#   c_compile   BIN SRC...          compile with -std=c11 -Wall -Wextra -Werror
#   c_compile_san BIN SRC...        same + ASan/UBSan (falls back gracefully)
#   run_ok      "desc" cmd...       pass if cmd exits 0
#   run_fails   "desc" cmd...       pass if cmd exits non-zero
#   expect_out  "desc" "expected" cmd...            exact stdout match
#   expect_out_stdin "desc" "input" "expected" cmd  ...fed via stdin
#   diff_out    "desc" expected_file cmd...         stdout diffed against file
#   mem_check   "desc" cmd...       run under ASan (or valgrind) — fail on any
#                                   leak, overflow, or UB report
#   grade_summary                   print PASS/FAIL, set exit status
#
# Environment (set by the ctrain runner and by selftest.sh):
#   CTRAIN_ROOT  repo root
#   TASK_DIR     the task's directory (grade.sh, test_main.c, expected files)
#   WORK_DIR     the learner's directory (their .c files live here)
#   SRC          convenience path to $WORK_DIR/main.c
#
# Graders test BEHAVIOUR, not style: any correct implementation must pass.

set -u

CC="${CC:-cc}"
CFLAGS_BASE=(-std=c11 -Wall -Wextra -Werror -g)

# Learner code may loop forever; every run helper is capped (seconds).
RUN_TIMEOUT="${CTRAIN_TIMEOUT:-10}"
_run() { timeout --signal=KILL "$RUN_TIMEOUT" "$@"; }

# ----- colours ---------------------------------------------------------------
if [ -t 1 ]; then
  C_G=$'\033[32m'; C_R=$'\033[31m'; C_Y=$'\033[33m'
  C_BOLD=$'\033[1m'; C_DIM=$'\033[2m'; C_0=$'\033[0m'
else
  C_G=; C_R=; C_Y=; C_BOLD=; C_DIM=; C_0=
fi

_PASS=0; _FAIL=0
_TMP="$(mktemp -d "${TMPDIR:-/tmp}/ctrain-grade.XXXXXX")"
trap 'rm -rf "$_TMP"' EXIT

_pass() { printf "  ${C_G}✓${C_0} %s\n" "$1"; _PASS=$((_PASS + 1)); }
_fail() { printf "  ${C_R}✗${C_0} %s\n" "$1"; _FAIL=$((_FAIL + 1)); }

# show at most N lines of a captured log, dimmed and indented
_show_log() {
  [ -s "$1" ] || return 0
  sed -e 's/^/      /' -e 20q "$1" | while IFS= read -r line; do
    printf "%s%s%s\n" "$C_DIM" "$line" "$C_0"
  done
}

# ----- compilation -----------------------------------------------------------

# c_compile BIN SRC...  — warnings are errors; a warning means FAIL.
c_compile() {
  local bin="$1"; shift
  local log="$_TMP/cc.log"
  if "$CC" "${CFLAGS_BASE[@]}" -o "$bin" "$@" -lm 2>"$log"; then
    _pass "compiles cleanly (${CFLAGS_BASE[*]})"
    return 0
  fi
  _fail "compiles cleanly (${CFLAGS_BASE[*]})"
  _show_log "$log"
  return 1
}

# Does this toolchain support ASan/UBSan? Cached per grader run.
_SAN_OK=""
san_available() {
  if [ -z "$_SAN_OK" ]; then
    local probe="$_TMP/san-probe"
    if printf 'int main(void){return 0;}\n' >"$probe.c" &&
       "$CC" -std=c11 -fsanitize=address,undefined -o "$probe" "$probe.c" 2>/dev/null &&
       "$probe" 2>/dev/null; then
      _SAN_OK=yes
    else
      _SAN_OK=no
    fi
  fi
  [ "$_SAN_OK" = yes ]
}

# c_compile_san BIN SRC... — sanitized build; plain build if unsupported
# (mem_check then uses valgrind instead when it is present).
c_compile_san() {
  local bin="$1"; shift
  local log="$_TMP/cc-san.log"
  local flags=("${CFLAGS_BASE[@]}")
  if san_available; then
    flags+=(-fsanitize=address,undefined -fno-omit-frame-pointer)
  fi
  if "$CC" "${flags[@]}" -o "$bin" "$@" -lm 2>"$log"; then
    _pass "compiles cleanly for memory checking"
    return 0
  fi
  _fail "compiles cleanly for memory checking"
  _show_log "$log"
  return 1
}

# ----- behavioural checks ----------------------------------------------------

run_ok() {
  local desc="$1"; shift
  local log="$_TMP/run.log"
  if _run "$@" >"$log" 2>&1; then _pass "$desc"; else _fail "$desc"; _show_log "$log"; fi
}

run_fails() {
  local desc="$1"; shift
  if _run "$@" >/dev/null 2>&1; then _fail "$desc"; else _pass "$desc"; fi
}

expect_out() {
  local desc="$1" want="$2"; shift 2
  local got
  got="$(_run "$@" 2>/dev/null)"
  if [ "$got" = "$want" ]; then
    _pass "$desc"
  else
    _fail "$desc"
    printf "%s      expected: %s%s\n" "$C_DIM" "$(printf %s "$want" | head -c 200)" "$C_0"
    printf "%s      got:      %s%s\n" "$C_DIM" "$(printf %s "$got"  | head -c 200)" "$C_0"
  fi
}

expect_out_stdin() {
  local desc="$1" input="$2" want="$3"; shift 3
  local got
  got="$(printf '%s' "$input" | _run "$@" 2>/dev/null)"
  if [ "$got" = "$want" ]; then
    _pass "$desc"
  else
    _fail "$desc"
    printf "%s      expected: %s%s\n" "$C_DIM" "$(printf %s "$want" | head -c 200)" "$C_0"
    printf "%s      got:      %s%s\n" "$C_DIM" "$(printf %s "$got"  | head -c 200)" "$C_0"
  fi
}

# diff_out "desc" expected_file cmd...    (stdin may be redirected by caller)
diff_out() {
  local desc="$1" want="$2"; shift 2
  local got="$_TMP/got.out" d="$_TMP/diff.out"
  _run "$@" >"$got" 2>/dev/null
  if diff -u "$want" "$got" >"$d" 2>&1; then
    _pass "$desc"
  else
    _fail "$desc"
    _show_log "$d"
  fi
}

# ----- memory safety ---------------------------------------------------------
# mem_check "desc" cmd...
# The binary in cmd should be built with c_compile_san. Under ASan/UBSan the
# process aborts on any report (leaks included). If sanitizers are unavailable
# but valgrind exists, run under valgrind instead; failing both, degrade to a
# plain run and say so.
mem_check() {
  local desc="$1"; shift
  local log="$_TMP/mem.log"
  if san_available; then
    if ASAN_OPTIONS="detect_leaks=1:abort_on_error=0:exitcode=99" \
       UBSAN_OPTIONS="halt_on_error=1:print_stacktrace=1" \
       _run "$@" >"$log" 2>&1; then
      _pass "$desc (ASan/UBSan clean)"
    else
      _fail "$desc (ASan/UBSan reported errors)"
      grep -E "ERROR|SUMMARY|runtime error" "$log" | head -10 >"$_TMP/mem-brief.log" || true
      _show_log "$_TMP/mem-brief.log"
    fi
  elif command -v valgrind >/dev/null 2>&1; then
    if timeout --signal=KILL $((RUN_TIMEOUT * 4)) \
         valgrind --quiet --error-exitcode=99 --leak-check=full \
         --errors-for-leak-kinds=definite,possible "$@" >"$log" 2>&1; then
      _pass "$desc (valgrind clean)"
    else
      _fail "$desc (valgrind reported errors)"
      _show_log "$log"
    fi
  else
    printf "  %s! no sanitizers or valgrind found — memory checks skipped%s\n" "$C_Y" "$C_0"
    if _run "$@" >"$log" 2>&1; then _pass "$desc (plain run)"; else _fail "$desc (plain run)"; _show_log "$log"; fi
  fi
}

# ----- summary ---------------------------------------------------------------
grade_summary() {
  local total=$((_PASS + _FAIL))
  echo
  if [ "$_FAIL" -eq 0 ] && [ "$total" -gt 0 ]; then
    printf "${C_G}${C_BOLD}PASS${C_0} — %d/%d checks passed\n" "$_PASS" "$total"
    exit 0
  fi
  printf "${C_R}${C_BOLD}FAIL${C_0} — %d/%d checks passed\n" "$_PASS" "$total"
  exit 1
}
