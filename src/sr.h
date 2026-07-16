/*
 * sr.h — pure spaced-repetition and task-selection logic for ctrain.
 *
 * This header is deliberately free of I/O, globals and the task/progress
 * machinery in ctrain.c: it operates on plain scalars and small arrays so it
 * can be unit-tested on its own (see tests/test_selection.c) without touching
 * the filesystem or real learner state. ctrain.c includes it and feeds it a
 * curriculum-ordered view of the task list; the test feeds it synthetic data.
 *
 * It mirrors the selection design of the redhat and pytrain trainers:
 *   - a fixed review ladder (1, 3, 7, 16, 35, 75 days, then doubling),
 *   - reviews take priority once due, most-overdue first,
 *   - but never more than two reviews in a row while new material still waits,
 *   - new material is offered in a fundamentals-first teaching order.
 */
#ifndef CTRAIN_SR_H
#define CTRAIN_SR_H

#include <string.h>

/* ---- spaced-repetition ladder -------------------------------------------- */

/* Days until the next review, indexed by how many times the task has been
 * passed (reps >= 1). After the ladder runs out the gap keeps doubling. */
static const long SR_INTERVALS[] = {1, 3, 7, 16, 35, 75};
#define SR_NLADDER ((int)(sizeof SR_INTERVALS / sizeof SR_INTERVALS[0]))

static inline long sr_review_interval(int reps)
{
    int i = (reps < 1 ? 1 : reps) - 1;
    if (i < SR_NLADDER)
        return SR_INTERVALS[i];
    long v = SR_INTERVALS[SR_NLADDER - 1];
    for (int k = 0; k < i - SR_NLADDER + 1; k++)
        v *= 2;
    return v;
}

/* How many days past its review date a task is: >= 0 means due now, < 0 means
 * still in the future. Caller decides what "unscheduled" means (see sr_choose,
 * which treats a passed-but-unscheduled task as due). */
static inline long sr_days_overdue(long due_ord, long today_ord)
{
    return today_ord - due_ord;
}

/* ---- fundamentals-first teaching order ----------------------------------- */

/* Explicit teaching order for NEW material, keyed by the task's domain
 * directory. Fundamentals first, then things that build on them:
 *
 *   foundations   the ambient toolchain itself — cc, compiler errors, make,
 *                 crashes, gdb — taught before any task quietly assumes it
 *   tutorial      the toolchain and first programs — everything uses it
 *   types         values, operators, expressions, conversions
 *   control       if / loops / switch — flow over those values
 *   functions     factoring code, scope, recursion, program structure
 *   pointers      pointers & arrays — the heart of C, needs all the above
 *   structs       aggregating data once you can point at it
 *   io            stdio: files and formatted I/O
 *   system        the raw UNIX system-call interface under stdio
 *   stdlib        the standard-library toolbox
 *   preprocessor  macros and conditional compilation
 *   ub            undefined behaviour — traps you only see once you can read C
 *   dsa           data structures & algorithms built from all the above
 *   tooling       make, gdb, sanitizers — engineering the code you can write
 *   projects      multi-file capstones combining everything
 *
 * The directories already carry NN- prefixes, so lexical id order happens to
 * match this; making it explicit keeps the intent clear and survives renames.
 * Anything unlisted sorts after these (rank 99), then by id. */
static const char *SR_DOMAIN_ORDER[] = {
    "00-foundations",
    "01-tutorial", "02-types",        "03-control", "04-functions",
    "05-pointers", "06-structs",      "07-io",      "08-system",
    "09-stdlib",   "10-preprocessor", "11-ub",      "12-dsa",
    "13-tooling",  "14-projects",
};
#define SR_NDOMAINS ((int)(sizeof SR_DOMAIN_ORDER / sizeof SR_DOMAIN_ORDER[0]))

/* Rank of a domain directory name in the teaching order (99 if unknown). */
static inline int sr_domain_rank(const char *domain)
{
    for (int i = 0; i < SR_NDOMAINS; i++)
        if (strcmp(domain, SR_DOMAIN_ORDER[i]) == 0)
            return i;
    return 99;
}

/* Order two task ids ("domain/nn-name") in teaching order: by the domain's
 * rank, then lexically by full id. Suitable as a comparator core. */
static inline int sr_curriculum_cmp(const char *id_a, const char *id_b)
{
    char da[128] = "", db[128] = "";
    const char *sa = strchr(id_a, '/'), *sb = strchr(id_b, '/');
    size_t la = sa ? (size_t)(sa - id_a) : strlen(id_a);
    size_t lb = sb ? (size_t)(sb - id_b) : strlen(id_b);
    if (la >= sizeof da) la = sizeof da - 1;
    if (lb >= sizeof db) lb = sizeof db - 1;
    memcpy(da, id_a, la); da[la] = '\0';
    memcpy(db, id_b, lb); db[lb] = '\0';
    int ra = sr_domain_rank(da), rb = sr_domain_rank(db);
    if (ra != rb)
        return ra < rb ? -1 : 1;
    return strcmp(id_a, id_b);
}

/* ---- choosing what to do next -------------------------------------------- */

enum { SR_NONE = -1, SR_NEW = 0, SR_REVIEW = 1 };

/* A curriculum-ordered snapshot of one task's progress, as sr_choose sees it. */
typedef struct {
    int  passed;    /* has been passed at least once */
    int  attempts;  /* attempts so far (used only for the caller's reason text) */
    int  scheduled; /* 1 if it has a due date, 0 if passed but never scheduled */
    long due_ord;   /* due date as a day-ordinal (valid iff scheduled) */
} sr_task;

/* Decide what to practise next. Pure — no side effects, so it is testable.
 *
 *   ts[0..n)     tasks in fundamentals-first teaching order (index 0 first)
 *   today_ord    today's day-ordinal
 *   recent       the last few pick kinds (SR_NEW / SR_REVIEW), oldest first
 *   n_recent     how many entries in `recent`
 *   kind_out     set to SR_NEW / SR_REVIEW / SR_NONE
 *
 * Returns the chosen task index, or -1 (with *kind_out == SR_NONE) when
 * everything is done and nothing is due. Reviews win once due (most overdue
 * first; a passed-but-unscheduled task counts as due so it re-enters rotation)
 * but never three in a row while new material is still waiting. */
static inline int sr_choose(const sr_task *ts, int n, long today_ord,
                            const int *recent, int n_recent, int *kind_out)
{
    int  best_rev = -1;
    long best_over = 0;
    for (int i = 0; i < n; i++) {
        if (!ts[i].passed)
            continue;
        long over;
        if (!ts[i].scheduled) {
            over = 0; /* passed before scheduling existed — treat as due now */
        } else {
            over = sr_days_overdue(ts[i].due_ord, today_ord);
            if (over < 0)
                continue; /* not due yet */
        }
        if (best_rev < 0 || over > best_over) {
            best_rev = i;
            best_over = over;
        }
    }

    int new_i = -1;
    for (int i = 0; i < n; i++)
        if (!ts[i].passed) {
            new_i = i;
            break;
        }

    int wall = n_recent >= 2 &&
               recent[n_recent - 1] == SR_REVIEW &&
               recent[n_recent - 2] == SR_REVIEW;

    if (best_rev >= 0 && !(wall && new_i >= 0)) {
        *kind_out = SR_REVIEW;
        return best_rev;
    }
    if (new_i >= 0) {
        *kind_out = SR_NEW;
        return new_i;
    }
    if (best_rev >= 0) {
        *kind_out = SR_REVIEW;
        return best_rev;
    }
    *kind_out = SR_NONE;
    return -1;
}

#endif /* CTRAIN_SR_H */
