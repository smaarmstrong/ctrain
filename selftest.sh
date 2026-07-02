#!/usr/bin/env bash
#
# selftest.sh — verify every task's grader against its own artifacts.
#
# For each task, in a throwaway work dir:
#   starter (or empty stub)  -> grade.sh  must FAIL   (grader not pre-satisfied)
#   solution.c / solution/   -> grade.sh  must PASS   (grader not too strict)
#
# Usage:
#   ./selftest.sh                      # all tasks, natively
#   ./selftest.sh 05-pointers/03-swap  # just these tasks
#   ./selftest.sh --container [ids]    # same, inside a gcc container
#
# The container run adds SYS_PTRACE so LeakSanitizer works.
set -uo pipefail
ROOT="$(cd "$(dirname "$(readlink -f "$0")")" && pwd)"
IMAGE="${IMAGE:-docker.io/library/gcc:14}"

if [ "${1:-}" = "--container" ]; then
  shift
  ENGINE="$(command -v podman || command -v docker)" || {
    echo "need podman or docker for --container"; exit 1; }
  exec "$ENGINE" run --rm --cap-add SYS_PTRACE \
    -v "$ROOT":/ctrain:Z -w /ctrain "$IMAGE" ./selftest.sh "$@"
fi

if [ "$#" -gt 0 ]; then
  TASKS=("$@")
else
  TASKS=()
  while IFS= read -r meta; do
    d="$(dirname "$meta")"
    TASKS+=("$(basename "$(dirname "$d")")/$(basename "$d")")
  done < <(find "$ROOT/tasks" -mindepth 3 -maxdepth 3 -name meta.json | sort)
fi

if [ -t 1 ]; then G=$'\033[32m'; R=$'\033[31m'; D=$'\033[2m'; N=$'\033[0m'
else G=; R=; D=; N=; fi

fail_total=0
run_case() { # run_case <task_dir> <work_dir> -> grade.sh exit code
  local tdir="$1" wdir="$2"
  CTRAIN_ROOT="$ROOT" TASK_DIR="$tdir" WORK_DIR="$wdir" SRC="$wdir/main.c" \
    bash "$tdir/grade.sh" >"$wdir/.grade.log" 2>&1
}

for id in "${TASKS[@]}"; do
  tdir="$ROOT/tasks/$id"
  if [ ! -f "$tdir/grade.sh" ]; then
    printf "%s✗ %-40s no grade.sh%s\n" "$R" "$id" "$N"; fail_total=$((fail_total+1)); continue
  fi
  tmp="$(mktemp -d "${TMPDIR:-/tmp}/ctrain-selftest.XXXXXX")"

  # --- case 1: starter/stub must FAIL -------------------------------------
  wdir="$tmp/starter"; mkdir -p "$wdir"
  if [ -d "$tdir/starter" ]; then
    cp "$tdir/starter/"* "$wdir/"
  elif [ -f "$tdir/starter.c" ]; then
    cp "$tdir/starter.c" "$wdir/main.c"
  else
    printf '/* stub */\n' >"$wdir/main.c"
  fi
  if run_case "$tdir" "$wdir"; then
    printf "%s✗ %-40s grader PASSES on the starter stub%s\n" "$R" "$id" "$N"
    fail_total=$((fail_total+1)); rm -rf "$tmp"; continue
  fi

  # --- case 2: solution must PASS ------------------------------------------
  wdir="$tmp/solution"; mkdir -p "$wdir"
  if [ -d "$tdir/solution" ]; then
    cp "$tdir/solution/"* "$wdir/"
  elif [ -f "$tdir/solution.c" ]; then
    cp "$tdir/solution.c" "$wdir/main.c"
  else
    printf "%s✗ %-40s no solution.c or solution/%s\n" "$R" "$id" "$N"
    fail_total=$((fail_total+1)); rm -rf "$tmp"; continue
  fi
  if run_case "$tdir" "$wdir"; then
    printf "%s✓ %-40s%s\n" "$G" "$id" "$N"
  else
    printf "%s✗ %-40s grader FAILS on the reference solution%s\n" "$R" "$id" "$N"
    sed -e 's/^/    /' -e 40q "$wdir/.grade.log"
    fail_total=$((fail_total+1))
  fi
  rm -rf "$tmp"
done

echo
if [ "$fail_total" -eq 0 ]; then
  printf "%sselftest: all %d task(s) OK%s\n" "$G" "${#TASKS[@]}" "$N"
else
  printf "%sselftest: %d of %d task(s) FAILED%s\n" "$R" "$fail_total" "${#TASKS[@]}" "$N"
  exit 1
fi
