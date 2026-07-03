#!/usr/bin/env bash
. "$CTRAIN_ROOT/lib/grade.sh"

# 1. full build + program behaviour
run_ok "'make' succeeds" make -C "$WORK_DIR"
run_ok "executable ./statcalc exists" test -x "$WORK_DIR/statcalc"
run_ok "main.o and stats.o were produced" \
  bash -c "test -f '$WORK_DIR/main.o' && test -f '$WORK_DIR/stats.o'"
expect_out "./statcalc prints the exact report line" \
  "n=8 sum=2470 max=997 mean=308.75" "$WORK_DIR/statcalc"

# 2. incremental rebuilds: back-date everything, touch one file, remake.
REF_OLD="$WORK_DIR/.ref-old"; REF_MID="$WORK_DIR/.ref-mid"
touch -d '2020-01-01 00:00:00' "$REF_OLD"
touch -d '2020-02-01 00:00:00' "$REF_MID"

backdate() {
  touch -d '2020-01-01 00:00:00' \
    "$WORK_DIR"/*.c "$WORK_DIR"/*.h "$WORK_DIR"/*.o "$WORK_DIR/statcalc" \
    2>/dev/null
}

backdate
touch -d '2020-03-01 00:00:00' "$WORK_DIR/stats.c" 2>/dev/null
run_ok "'make' after touching stats.c succeeds" make -C "$WORK_DIR"
run_ok "stats.o was recompiled" \
  bash -c "[ '$WORK_DIR/stats.o' -nt '$REF_MID' ]"
run_ok "main.o was NOT recompiled (only what changed rebuilds)" \
  bash -c "[ ! '$WORK_DIR/main.o' -nt '$REF_OLD' ]"

backdate
touch -d '2020-03-01 00:00:00' "$WORK_DIR/stats.h" 2>/dev/null
run_ok "'make' after touching stats.h succeeds" make -C "$WORK_DIR"
run_ok "a header change recompiles BOTH objects" \
  bash -c "[ '$WORK_DIR/main.o' -nt '$REF_MID' ] && [ '$WORK_DIR/stats.o' -nt '$REF_MID' ]"

# 3. the module stands on its own against the harness
c_compile "$WORK_DIR/unit" "$TASK_DIR/test_main.c" "$WORK_DIR/stats.c" \
  -I"$WORK_DIR" || grade_summary
run_ok "harness checks pass against stats.c/stats.h" "$WORK_DIR/unit"

c_compile_san "$WORK_DIR/unit-san" "$TASK_DIR/test_main.c" "$WORK_DIR/stats.c" \
  -I"$WORK_DIR" || grade_summary
mem_check "no out-of-bounds reads or UB in the module" "$WORK_DIR/unit-san"

grade_summary
