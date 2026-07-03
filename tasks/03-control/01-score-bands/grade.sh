#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary

expect_out_stdin "bands and pass/fail markers" "85 100 90 89 60 59 42 0" \
"85: B (pass)
100: A (pass)
90: A (pass)
89: B (pass)
60: D (pass)
59: F (fail)
42: F (fail)
0: F (fail)" "$WORK_DIR/prog"

expect_out_stdin "each band boundary" "79 70 69 80" \
"79: C (pass)
70: C (pass)
69: D (pass)
80: B (pass)" "$WORK_DIR/prog"

expect_out_stdin "out-of-range scores are invalid" "101 -3 50 1000" \
"101: invalid
-3: invalid
50: F (fail)
1000: invalid" "$WORK_DIR/prog"

expect_out_stdin "empty input prints nothing" "" "" "$WORK_DIR/prog"

grade_summary
