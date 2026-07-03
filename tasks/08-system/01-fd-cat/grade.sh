#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary

# The prompt forbids stdio.h explicitly; everything below fd-level is out.
run_fails "does not mention stdio.h (the rule in the prompt)" \
  grep -q 'stdio\.h' "$SRC"

D="$WORK_DIR/data"
rm -rf "$D"
mkdir -p "$D"
printf 'first file\nline two\n' >"$D/a.txt"
printf 'second\n' >"$D/b.txt"
printf 'no trailing newline' >"$D/c.txt"
printf 'ab\0cd\0\xff\xfe' >"$D/bin.dat"

diff_out "copies a single file byte-for-byte" \
  "$D/a.txt" "$WORK_DIR/prog" "$D/a.txt"

cat "$D/a.txt" "$D/b.txt" "$D/c.txt" >"$D/want3"
diff_out "concatenates several files in argument order" \
  "$D/want3" "$WORK_DIR/prog" "$D/a.txt" "$D/b.txt" "$D/c.txt"

run_ok "binary data (NUL and high bytes) survives the copy" \
  bash -c '"$1" "$2" >"$3" 2>/dev/null && cmp -s "$2" "$3"' \
  _ "$WORK_DIR/prog" "$D/bin.dat" "$D/got.bin"

expect_out_stdin "with no arguments, copies stdin to stdout" \
  "hello fd world" "hello fd world" "$WORK_DIR/prog"

run_fails "an unopenable file means a non-zero exit status" \
  "$WORK_DIR/prog" "$D/no-such-file"

c_compile_san "$WORK_DIR/prog-san" "$SRC" || grade_summary
mem_check "no overflows or undefined behaviour while copying" \
  "$WORK_DIR/prog-san" "$D/a.txt" "$D/bin.dat"

grade_summary
