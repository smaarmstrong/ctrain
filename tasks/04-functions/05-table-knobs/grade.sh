#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# Default build: TABLE_LO/TABLE_HI must fall back to 1 and 10.
c_compile "$WORK_DIR/prog-default" "$SRC" || grade_summary
diff_out "default build prints the 1..10 table" \
  "$TASK_DIR/expected-default.out" "$WORK_DIR/prog-default"

# Override build: -D flags must win over the in-file defaults.
c_compile "$WORK_DIR/prog-3-6" "$SRC" -DTABLE_LO=3 -DTABLE_HI=6 || grade_summary
diff_out "build with -DTABLE_LO=3 -DTABLE_HI=6 prints the 3..6 table" \
  "$TASK_DIR/expected-3-6.out" "$WORK_DIR/prog-3-6"

# Negative bounds work too.
c_compile "$WORK_DIR/prog-neg" "$SRC" "-DTABLE_LO=-2" -DTABLE_HI=2 || grade_summary
diff_out "build with -DTABLE_LO=-2 -DTABLE_HI=2 prints the -2..2 table" \
  "$TASK_DIR/expected-neg.out" "$WORK_DIR/prog-neg"

grade_summary
