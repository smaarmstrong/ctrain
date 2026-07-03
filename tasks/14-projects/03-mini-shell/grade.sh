#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$WORK_DIR"/*.c || grade_summary
PROG="$WORK_DIR/prog"

F="$WORK_DIR/.fixtures"
rm -rf "$F"
mkdir -p "$F/inner"
printf 'from-inner\n' >"$F/inner/note.txt"

# --- scripted sessions: stdout must be exactly prompts + command output ---
expect_out_stdin "echo hello: prompt, output, prompt" \
  $'echo hello\n' $'$ hello\n$ ' "$PROG"

expect_out_stdin "arguments reach the command intact" \
  $'echo one two three\n' $'$ one two three\n$ ' "$PROG"

expect_out_stdin "two commands run in order, one wait each" \
  $'echo first\necho second\n' $'$ first\n$ second\n$ ' "$PROG"

expect_out_stdin "blank lines just reprompt" \
  $'\n\necho x\n' $'$ $ $ x\n$ ' "$PROG"

expect_out_stdin "tabs separate tokens too" \
  $'echo\ta\tb\n' $'$ a b\n$ ' "$PROG"

expect_out_stdin "immediate EOF prints one prompt and exits" \
  '' '$ ' "$PROG"

# --- cd is a builtin: it must change the SHELL's directory ----------------
expect_out_stdin "cd persists for later commands (cat a relative file)" \
  $'cd '"$F/inner"$'\ncat note.txt\n' $'$ $ from-inner\n$ ' "$PROG"

expect_out_stdin "a failing cd writes stderr only and the shell carries on" \
  $'cd '"$F/no-such-dir"$'\necho alive\n' $'$ $ alive\n$ ' "$PROG"

# --- a command that cannot exec must not kill the shell -------------------
expect_out_stdin "unknown command: stderr only, shell keeps running" \
  $'definitely-not-a-command-xyz\necho alive\n' $'$ $ alive\n$ ' "$PROG"

# --- exit statuses ----------------------------------------------------------
run_ok "EOF exits with status 0" \
  bash -c 'printf "" | "$1" >/dev/null 2>&1' _ "$PROG"
run_ok "bare exit exits with status 0" \
  bash -c 'printf "exit\n" | "$1" >/dev/null 2>&1' _ "$PROG"
run_ok "exit 7 exits with status 7" \
  bash -c 'printf "exit 7\n" | "$1" >/dev/null 2>&1; test $? -eq 7' _ "$PROG"
run_ok "exit stops the shell: later commands never run" \
  bash -c 'out=$(printf "exit 3\necho leaked\n" | "$1" 2>/dev/null); \
           test $? -eq 3 && test "$out" = "\$ "' _ "$PROG"

# --- diagnostics go to stderr, never stdout --------------------------------
T="${CTRAIN_TIMEOUT:-10}"
printf 'definitely-not-a-command-xyz\n' | timeout --signal=KILL "$T" \
  "$PROG" >"$F/bad.out" 2>"$F/bad.err"
run_ok "exec failure produces a message on stderr" test -s "$F/bad.err"

# --- memory safety -----------------------------------------------------------
c_compile_san "$WORK_DIR/prog-san" "$WORK_DIR"/*.c || grade_summary
printf 'echo mem\ncd %s\ncat note.txt\nexit\n' "$F/inner" >"$F/session.txt"
mem_check "no leaks, overflows, or undefined behaviour across a session" \
  "$WORK_DIR/prog-san" <"$F/session.txt"

grade_summary
