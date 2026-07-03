#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# default build: no flags
c_compile "$WORK_DIR/prog-default" "$SRC" || grade_summary
expect_out "default build prints community / 4 / off" \
  $'edition: community\nitems: 4\nlogging: off' \
  "$WORK_DIR/prog-default"

# all features on, custom item count
c_compile "$WORK_DIR/prog-pro" "$SRC" -DPRO -DENABLE_LOG -DMAX_ITEMS=9 || grade_summary
expect_out "-DPRO -DENABLE_LOG -DMAX_ITEMS=9 prints pro / 9 / on" \
  $'edition: pro\nitems: 9\nlogging: on' \
  "$WORK_DIR/prog-pro"

# PRO defined with an explicit value must still count as defined
c_compile "$WORK_DIR/prog-pro2" "$SRC" -DPRO=1 || grade_summary
expect_out "-DPRO=1 also selects the pro edition" \
  $'edition: pro\nitems: 4\nlogging: off' \
  "$WORK_DIR/prog-pro2"

# invalid configurations must fail to compile (#error)
run_fails "PRO + TRIAL together refuse to compile" \
  "$CC" -std=c11 -Wall -Wextra -Werror -o "$WORK_DIR/never1" "$SRC" -DPRO -DTRIAL
run_fails "MAX_ITEMS=0 refuses to compile" \
  "$CC" -std=c11 -Wall -Wextra -Werror -o "$WORK_DIR/never2" "$SRC" -DMAX_ITEMS=0

grade_summary
