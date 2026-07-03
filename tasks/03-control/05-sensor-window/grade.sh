#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

c_compile "$WORK_DIR/prog" "$SRC" || grade_summary

expect_out_stdin "sample from the prompt" "12 -60 99 200 5 9999 42 -999" \
"valid: 3
skipped: 2
min: 5
max: 99" "$WORK_DIR/prog"

expect_out_stdin "boundaries -50 and 150 are valid; -51 and 151 are glitches" \
"-50 150 -51 151" \
"valid: 2
skipped: 2
min: -50
max: 150" "$WORK_DIR/prog"

expect_out_stdin "sentinel stops everything, even later glitches and sentinels" \
"10 9999 5000 9999 -70 20" \
"valid: 1
skipped: 0
min: 10
max: 10" "$WORK_DIR/prog"

expect_out_stdin "sentinel first: nothing counted" "9999 1 2 3" \
"valid: 0
skipped: 0" "$WORK_DIR/prog"

expect_out_stdin "all glitches: no min/max lines" "-100 9000 151" \
"valid: 0
skipped: 3" "$WORK_DIR/prog"

expect_out_stdin "empty input" "" \
"valid: 0
skipped: 0" "$WORK_DIR/prog"

expect_out_stdin "single valid reading is both min and max" "-3" \
"valid: 1
skipped: 0
min: -3
max: -3" "$WORK_DIR/prog"

grade_summary
