#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary

IN="$TASK_DIR/input.txt"

diff_out "plain substring match" \
  "$TASK_DIR/expected-plain.out"  "$WORK_DIR/prog" the <"$IN"
diff_out "-n numbers the matches" \
  "$TASK_DIR/expected-n.out"      "$WORK_DIR/prog" -n the <"$IN"
diff_out "-v inverts the match" \
  "$TASK_DIR/expected-v.out"      "$WORK_DIR/prog" -v the <"$IN"
diff_out "-v -n combined (either order)" \
  "$TASK_DIR/expected-vn.out"     "$WORK_DIR/prog" -n -v the <"$IN"
diff_out "empty pattern matches every line" \
  "$TASK_DIR/expected-empty.out"  "$WORK_DIR/prog" "" <"$IN"
diff_out "last line without a final newline still matches" \
  "$TASK_DIR/expected-nonl.out"   "$WORK_DIR/prog" -n tail <"$TASK_DIR/input-nonl.txt"

run_ok  "exits 0 when something matched"       bash -c 'printf "abc\n" | "$1" b'       _ "$WORK_DIR/prog"
run_ok  "exits 1 when nothing matched"         bash -c 'printf "abc\n" | "$1" zebra; test $? -eq 1' _ "$WORK_DIR/prog"
run_ok  "exits 2 with no pattern"              bash -c '"$1" </dev/null 2>/dev/null; test $? -eq 2' _ "$WORK_DIR/prog"
run_ok  "exits 2 on an unknown flag"           bash -c '"$1" -x pat </dev/null 2>/dev/null; test $? -eq 2' _ "$WORK_DIR/prog"
run_ok  "exits 2 with two patterns"            bash -c '"$1" a b </dev/null 2>/dev/null; test $? -eq 2' _ "$WORK_DIR/prog"
run_ok  "usage goes to stderr, nothing to stdout" \
  bash -c 'out=$("$1" 2>/dev/null </dev/null); test -z "$out"' _ "$WORK_DIR/prog"
run_ok  "usage line text on stderr" \
  bash -c 'err=$("$1" 2>&1 >/dev/null </dev/null); test "$err" = "usage: findtext [-v] [-n] pattern"' _ "$WORK_DIR/prog"
run_ok  "a lone - is a pattern, not a flag" \
  bash -c 'printf -- "a-b\nplain\n" | "$1" -; test $? -eq 0' _ "$WORK_DIR/prog"

c_compile_san "$WORK_DIR/prog-san" "$SRC" || grade_summary
mem_check "no leaks, overflows, or undefined behaviour" \
  "$WORK_DIR/prog-san" -n -v the <"$IN"

grade_summary
