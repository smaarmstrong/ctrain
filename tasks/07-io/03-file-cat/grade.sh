#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary

# Fixtures: a normal file, a file with no trailing newline + a long line.
a="$WORK_DIR/a.txt"; b="$WORK_DIR/b.txt"
printf 'alpha\nbravo\ncharlie\n' >"$a"
{ printf '%0.slong' {1..1000}; printf '\nno newline at end'; } >"$b"

# stdin mode
expect_out_stdin "no args: copies stdin to stdout" \
  $'one\ntwo\n' $'one\ntwo' "$WORK_DIR/prog"

# single file, then concatenation in argument order
diff_out "one file: reproduced exactly" "$a" "$WORK_DIR/prog" "$a"
cat "$a" "$b" >"$WORK_DIR/want-ab.txt"
diff_out "two files: concatenated in order, bytes faithful" \
  "$WORK_DIR/want-ab.txt" "$WORK_DIR/prog" "$a" "$b"
cat "$b" "$a" >"$WORK_DIR/want-ba.txt"
diff_out "argument order respected" \
  "$WORK_DIR/want-ba.txt" "$WORK_DIR/prog" "$b" "$a"

# unopenable file: message on stderr, clean stdout, exit 1, stop there
missing="$WORK_DIR/no-such-file.txt"
rm -f "$missing"
timeout --signal=KILL "${CTRAIN_TIMEOUT:-10}" \
  "$WORK_DIR/prog" "$a" "$missing" "$b" \
  >"$WORK_DIR/err-case.out" 2>"$WORK_DIR/err-case.err"
status=$?
run_ok "unopenable file: exit status 1" test "$status" -eq 1
printf 'mycat: cannot open %s\n' "$missing" >"$WORK_DIR/want-err.txt"
diff_out "unopenable file: exact message on stderr" \
  "$WORK_DIR/want-err.txt" cat "$WORK_DIR/err-case.err"
diff_out "files before the failure were copied; later ones were not" \
  "$a" cat "$WORK_DIR/err-case.out"

grade_summary
