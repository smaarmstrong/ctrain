#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary

D="$WORK_DIR/tree"
rm -rf "$D" "$WORK_DIR/emptydir"
mkdir -p "$D/sub" "$WORK_DIR/emptydir"
printf 'x' >"$D/.hidden"
printf 'abc' >"$D/aa"
printf '12345' >"$D/e.txt"
: >"$D/empty.dat"
printf 'hello world\n' >"$D/readme"

want='1 .hidden
3 aa
5 e.txt
0 empty.dat
12 readme
dir sub'
expect_out "lists entries sorted by byte order: sizes, dir markers, dot-files kept" \
  "$want" "$WORK_DIR/prog" "$D"

expect_out "an empty directory prints nothing" \
  "" "$WORK_DIR/prog" "$WORK_DIR/emptydir"

run_fails "a nonexistent directory means a non-zero exit" \
  "$WORK_DIR/prog" "$WORK_DIR/no-such-dir"
run_fails "wrong argument count means a non-zero exit" \
  "$WORK_DIR/prog"

c_compile_san "$WORK_DIR/prog-san" "$SRC" || grade_summary
mem_check "no leaks, overflows, or undefined behaviour while listing" \
  "$WORK_DIR/prog-san" "$D"

grade_summary
