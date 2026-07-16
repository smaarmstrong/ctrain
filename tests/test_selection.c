/*
 * test_selection.c — unit tests for ctrain's train/learn selection and
 * spaced-repetition logic (src/sr.h) and the lesson parser (src/lesson.h).
 *
 * Stdlib only, no framework: compile and run it directly (see the Makefile's
 * `test` target). It uses synthetic task/state arrays, so it never touches the
 * real task tree or a learner's progress file. Mirrors pytrain's
 * tests/test_selection.py.
 *
 *     cc -std=c11 -Wall -Wextra -Werror -o test_selection tests/test_selection.c
 *     ./test_selection
 */
#include <stdio.h>
#include <string.h>

#include "../src/sr.h"
#include "../src/lesson.h"

static int fails = 0;

static void check(const char *name, int cond)
{
    printf("  %s %s\n", cond ? "ok  " : "FAIL", name);
    if (!cond)
        fails++;
}

/* A fixed "today" so the tests don't depend on the wall clock. */
#define TODAY 1000L

int main(void)
{
    /* ---- spaced-repetition ladder ------------------------------------- */
    check("review_interval ladder is 1,3,7,16,35,75",
          sr_review_interval(1) == 1 && sr_review_interval(2) == 3 &&
          sr_review_interval(3) == 7 && sr_review_interval(4) == 16 &&
          sr_review_interval(5) == 35 && sr_review_interval(6) == 75);
    check("review_interval doubles past the ladder",
          sr_review_interval(7) == 150 && sr_review_interval(8) == 300);
    check("review_interval clamps reps < 1 to the first step",
          sr_review_interval(0) == 1 && sr_review_interval(-3) == 1);

    /* ---- days overdue -------------------------------------------------- */
    check("days_overdue: 0 when due today", sr_days_overdue(TODAY, TODAY) == 0);
    check("days_overdue: positive when past due", sr_days_overdue(TODAY - 3, TODAY) == 3);
    check("days_overdue: negative when in the future", sr_days_overdue(TODAY + 5, TODAY) == -5);

    /* ---- fundamentals-first teaching order ---------------------------- */
    check("domain rank: foundations sorts before everything",
          sr_domain_rank("00-foundations") == 0 &&
          sr_domain_rank("00-foundations") < sr_domain_rank("01-tutorial"));
    check("curriculum_cmp orders foundations before tutorial",
          sr_curriculum_cmp("00-foundations/01-compile-run",
                            "01-tutorial/01-hello") < 0);
    check("domain rank: tutorial < types < control < pointers",
          sr_domain_rank("01-tutorial") < sr_domain_rank("02-types") &&
          sr_domain_rank("02-types") < sr_domain_rank("03-control") &&
          sr_domain_rank("03-control") < sr_domain_rank("05-pointers"));
    check("domain rank: unknown domain sorts last (99)",
          sr_domain_rank("99-nope") == 99);
    check("curriculum_cmp orders tutorial before pointers",
          sr_curriculum_cmp("01-tutorial/01-hello", "05-pointers/01-swap") < 0);
    check("curriculum_cmp orders within a domain by id",
          sr_curriculum_cmp("02-types/01-a", "02-types/02-b") < 0);

    /* Synthetic task set in teaching order: index 0 is the first new task. */
    enum { A, B, C, D };
    sr_task ts[4];
    int kind;

    /* clean slate: everything unpassed -> first new task */
    memset(ts, 0, sizeof ts);
    check("choose on a clean slate gives the first new task (index 0)",
          sr_choose(ts, 4, TODAY, NULL, 0, &kind) == A && kind == SR_NEW);

    /* first task passed -> next new task */
    memset(ts, 0, sizeof ts);
    ts[A].passed = 1; ts[A].scheduled = 1; ts[A].due_ord = TODAY + 5; /* not due */
    check("choose skips a passed, not-yet-due task and gives the next new one",
          sr_choose(ts, 4, TODAY, NULL, 0, &kind) == B && kind == SR_NEW);

    /* a due review beats new material */
    memset(ts, 0, sizeof ts);
    ts[A].passed = 1; ts[A].scheduled = 1; ts[A].due_ord = TODAY - 2; /* 2d overdue */
    check("choose prefers a due review over new material",
          sr_choose(ts, 4, TODAY, NULL, 0, &kind) == A && kind == SR_REVIEW);

    /* most-overdue review wins */
    memset(ts, 0, sizeof ts);
    ts[A].passed = 1; ts[A].scheduled = 1; ts[A].due_ord = TODAY - 1;
    ts[B].passed = 1; ts[B].scheduled = 1; ts[B].due_ord = TODAY - 5; /* more overdue */
    ts[C].passed = 1; ts[C].scheduled = 1; ts[C].due_ord = TODAY + 3; /* not due */
    check("choose picks the most-overdue review first",
          sr_choose(ts, 4, TODAY, NULL, 0, &kind) == B && kind == SR_REVIEW);

    /* a passed-but-unscheduled task counts as due (re-enters rotation) */
    memset(ts, 0, sizeof ts);
    ts[A].passed = 1; ts[A].scheduled = 0; /* passed before scheduling existed */
    /* make all others passed+future so no NEW competes */
    ts[B].passed = 1; ts[B].scheduled = 1; ts[B].due_ord = TODAY + 9;
    ts[C].passed = 1; ts[C].scheduled = 1; ts[C].due_ord = TODAY + 9;
    ts[D].passed = 1; ts[D].scheduled = 1; ts[D].due_ord = TODAY + 9;
    check("choose treats a passed, unscheduled task as due",
          sr_choose(ts, 4, TODAY, NULL, 0, &kind) == A && kind == SR_REVIEW);

    /* the two-in-a-row wall: after two reviews, give new work if any waits */
    memset(ts, 0, sizeof ts);
    ts[A].passed = 1; ts[A].scheduled = 1; ts[A].due_ord = TODAY - 2; /* review due */
    /* B,C,D unpassed -> new material waiting */
    {
        int recent[] = {SR_REVIEW, SR_REVIEW};
        check("choose never gives 3 reviews in a row while new work waits",
              sr_choose(ts, 4, TODAY, recent, 2, &kind) == B && kind == SR_NEW);
    }

    /* ...but a review is fine after two if no new material remains */
    memset(ts, 0, sizeof ts);
    for (int i = 0; i < 4; i++) { ts[i].passed = 1; ts[i].scheduled = 1; ts[i].due_ord = TODAY - 2; }
    {
        int recent[] = {SR_REVIEW, SR_REVIEW};
        int chosen = sr_choose(ts, 4, TODAY, recent, 2, &kind);
        check("choose falls back to review when all new material is done",
              chosen >= 0 && kind == SR_REVIEW);
    }

    /* nothing due, nothing new -> caught up */
    memset(ts, 0, sizeof ts);
    for (int i = 0; i < 4; i++) { ts[i].passed = 1; ts[i].scheduled = 1; ts[i].due_ord = TODAY + 30; }
    check("choose returns -1/SR_NONE when caught up",
          sr_choose(ts, 4, TODAY, NULL, 0, &kind) == -1 && kind == SR_NONE);

    /* ---- lesson parsing ----------------------------------------------- */
    {
        lesson L = lesson_parse(
            "intro line\n"
            "---\n"
            "more prose\n"
            "```run\n"
            "#include <stdio.h>\n"
            "int main(void){ printf(\"hi\\n\"); }\n"
            "```\n"
            "tail\n");
        check("parse splits prose / pause / prose / run / prose",
              L.n == 5 &&
              L.beats[0].kind == BEAT_PROSE && L.beats[1].kind == BEAT_PAUSE &&
              L.beats[2].kind == BEAT_PROSE && L.beats[3].kind == BEAT_RUN &&
              L.beats[4].kind == BEAT_PROSE);
        check("parse keeps a run block verbatim (both lines, indentation)",
              L.beats[3].kind == BEAT_RUN &&
              strcmp(L.beats[3].text,
                     "#include <stdio.h>\nint main(void){ printf(\"hi\\n\"); }") == 0);
        check("parse captures leading prose text",
              L.beats[0].kind == BEAT_PROSE && strcmp(L.beats[0].text, "intro line") == 0);
        lesson_free(&L);
    }
    {
        lesson L = lesson_parse("just one paragraph\nof prose\n");
        check("parse of marker-free text is a single prose beat",
              L.n == 1 && L.beats[0].kind == BEAT_PROSE);
        lesson_free(&L);
    }

    printf("----\n");
    if (fails) {
        printf("FAILED %d check(s)\n", fails);
        return 1;
    }
    printf("all selection/SR/lesson checks passed\n");
    return 0;
}
