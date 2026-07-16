/*
 * ctrain — a console trainer for learning C, structured after K&R.
 *
 * The trainer is itself written in C11 (build it with `make` at the repo
 * root — that is lesson zero). Each task is a directory containing a spec
 * (prompt.md), optional scaffold (starter.c), a grader (grade.sh) that
 * compiles YOUR code with warnings-as-errors and runs it against tests —
 * including under AddressSanitizer/UBSan — and a reference solution.
 *
 * Usage:
 *     ctrain list                 all tasks, grouped by chapter, with status
 *     ctrain start  <id>          show a task and create its work directory
 *     ctrain check  <id>          compile + grade your code
 *     ctrain solution <id>        reveal a reference solution
 *     ctrain reset  <id>          restore the starter (your code -> main.c.bak)
 *     ctrain status               XP, streak, completion
 *
 * <id> is "domain/nn-name" or just the unique trailing name.
 * Progress lives in $XDG_STATE_HOME/ctrain/progress (~/.local/state).
 */
#define _XOPEN_SOURCE 700

#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdarg.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "sr.h"
#include "lesson.h"

#define MAX_TASKS 512
#define SLEN 256
#define PLEN PATH_MAX

/* ---- colours (disabled when stdout is not a terminal) ------------------- */

static int tty;

/* How the learner invoked us, so every hint names a command they can actually
 * run. self_cmd is the runnable path (for hints that take an <id>, e.g.
 * "ctrain solution 03-swap"); invoke_cmd is what to suggest for the plain
 * verbs — "make <verb>" when run under make (MAKELEVEL set), else self_cmd. */
static const char *self_cmd = "./ctrain";
static const char *invoke_cmd = "./ctrain";

#define GRN(s) (tty ? "\033[32m" s "\033[0m" : s)
#define RED(s) (tty ? "\033[31m" s "\033[0m" : s)
#define YEL(s) (tty ? "\033[33m" s "\033[0m" : s)
#define BLU(s) (tty ? "\033[34m" s "\033[0m" : s)
#define DIM(s) (tty ? "\033[2m" s "\033[0m" : s)
#define BLD(s) (tty ? "\033[1m" s "\033[0m" : s)

static const char *dim_on(void)  { return tty ? "\033[2m" : ""; }
static const char *bld_on(void)  { return tty ? "\033[1m" : ""; }
static const char *col_off(void) { return tty ? "\033[0m" : ""; }

/* ---- small utilities ----------------------------------------------------- */

static void die(const char *msg)
{
    fprintf(stderr, "%s%s%s\n", tty ? "\033[31m" : "", msg, col_off());
    exit(1);
}

/* snprintf that treats truncation as a hard error instead of a warning */
static void pathf(char *dst, size_t sz, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(dst, sz, fmt, ap);
    va_end(ap);
    if (n < 0 || (size_t)n >= sz)
        die("internal error: string too long");
}

static char *read_file(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    if (n < 0 || n > 4L * 1024 * 1024) {
        fclose(f);
        return NULL;
    }
    rewind(f);
    char *buf = malloc((size_t)n + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }
    size_t got = fread(buf, 1, (size_t)n, f);
    fclose(f);
    buf[got] = '\0';
    return buf;
}

static int mkdir_p(const char *path)
{
    char tmp[PLEN];
    pathf(tmp, sizeof tmp, "%s", path);
    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(tmp, 0755) != 0 && errno != EEXIST)
                return -1;
            *p = '/';
        }
    }
    if (mkdir(tmp, 0755) != 0 && errno != EEXIST)
        return -1;
    return 0;
}

static int copy_file(const char *from, const char *to)
{
    char *body = read_file(from);
    if (!body)
        return -1;
    FILE *out = fopen(to, "wb");
    if (!out) {
        free(body);
        return -1;
    }
    fputs(body, out);
    free(body);
    return fclose(out) == 0 ? 0 : -1;
}

static void today_str(char out[11])
{
    time_t t = time(NULL);
    struct tm tm;
    localtime_r(&t, &tm);
    strftime(out, 11, "%Y-%m-%d", &tm);
}

/* days since the epoch for a YYYY-MM-DD string; -1 on parse failure */
static long date_ordinal(const char *iso)
{
    struct tm tm = {0};
    if (!iso || sscanf(iso, "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday) != 3)
        return -1;
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    tm.tm_hour = 12; /* midday avoids DST edge cases */
    time_t t = mktime(&tm);
    return t < 0 ? -1 : (long)(t / 86400);
}

/* ---- minimal JSON field extraction ---------------------------------------
 * meta.json is a flat object of string/number values. This is not a general
 * JSON parser; it finds "key" and returns the scalar that follows the colon.
 */

static const char *json_find(const char *buf, const char *key)
{
    char pat[SLEN];
    pathf(pat, sizeof pat, "\"%s\"", key);
    const char *p = strstr(buf, pat);
    if (!p)
        return NULL;
    p += strlen(pat);
    while (*p && (isspace((unsigned char)*p) || *p == ':'))
        p++;
    return p;
}

static int json_str(const char *buf, const char *key, char *out, size_t outsz)
{
    const char *p = json_find(buf, key);
    out[0] = '\0';
    if (!p || *p != '"')
        return -1;
    p++;
    size_t i = 0;
    while (*p && *p != '"' && i + 1 < outsz) {
        if (*p == '\\' && p[1])
            p++;
        out[i++] = *p++;
    }
    out[i] = '\0';
    return 0;
}

static int json_int(const char *buf, const char *key, int fallback)
{
    const char *p = json_find(buf, key);
    if (!p || (!isdigit((unsigned char)*p) && *p != '-'))
        return fallback;
    return (int)strtol(p, NULL, 10);
}

/* "key": ["a", "b", ...] — copy up to max strings into out. Returns the count
 * (0 when the key is absent or not an array). Elements longer than SLEN-1
 * are truncated, like json_str. */
static int json_str_array(const char *buf, const char *key,
                          char (*out)[SLEN], int max)
{
    const char *p = json_find(buf, key);
    int n = 0;
    if (!p || *p != '[')
        return 0;
    p++;
    while (*p && *p != ']' && n < max) {
        while (*p && *p != '"' && *p != ']')
            p++;
        if (*p != '"')
            break;
        p++;
        size_t i = 0;
        while (*p && *p != '"') {
            if (*p == '\\' && p[1])
                p++;
            if (i + 1 < SLEN)
                out[n][i++] = *p;
            p++;
        }
        out[n][i] = '\0';
        if (*p == '"')
            p++;
        n++;
    }
    return n;
}

/* ---- task discovery ------------------------------------------------------ */

#define MAX_PREREQ 4

struct task {
    char id[SLEN];      /* "05-pointers/03-swap" */
    char name[SLEN];    /* "03-swap" */
    char dir[PLEN];     /* absolute path to the task directory */
    char title[SLEN];
    char domain[SLEN];
    char objective[SLEN];
    char difficulty[32];
    int est_min;
    char prereq[MAX_PREREQ][SLEN]; /* soft prerequisites: ids of lessons */
    int nprereq;
};

static struct task tasks[MAX_TASKS];
static int ntasks;
static char root[PLEN]; /* repo root: parent of the running binary */

static void find_root(const char *argv0)
{
    char self[PLEN];
    ssize_t n = readlink("/proc/self/exe", self, sizeof self - 1);
    if (n > 0) {
        self[n] = '\0';
    } else {
        pathf(self, sizeof self, "%s", argv0);
    }
    char *slash = strrchr(self, '/');
    if (slash)
        *slash = '\0';
    else
        pathf(self, sizeof self, ".");
    if (!realpath(self, root))
        die("cannot resolve repo root");
    /* allow running from src/ or a build dir: walk up until tasks/ exists */
    for (int up = 0; up < 3; up++) {
        char probe[PLEN];
        pathf(probe, sizeof probe, "%s/tasks", root);
        struct stat st;
        if (stat(probe, &st) == 0 && S_ISDIR(st.st_mode))
            return;
        char *cut = strrchr(root, '/');
        if (!cut || cut == root)
            break;
        *cut = '\0';
    }
}

static int name_cmp(const struct dirent **a, const struct dirent **b)
{
    return strcmp((*a)->d_name, (*b)->d_name);
}

static void discover(void)
{
    char base[PLEN];
    pathf(base, sizeof base, "%s/tasks", root);

    struct dirent **domains;
    int nd = scandir(base, &domains, NULL, name_cmp);
    if (nd < 0)
        return;
    for (int i = 0; i < nd; i++) {
        if (domains[i]->d_name[0] == '.')
            goto next_domain;
        char dpath[PLEN];
        pathf(dpath, sizeof dpath, "%s/%s", base, domains[i]->d_name);

        struct dirent **entries;
        int ne = scandir(dpath, &entries, NULL, name_cmp);
        if (ne < 0)
            goto next_domain;
        for (int j = 0; j < ne; j++) {
            if (entries[j]->d_name[0] == '.' || ntasks >= MAX_TASKS)
                goto next_entry;
            char mpath[PLEN];
            pathf(mpath, sizeof mpath, "%s/%s/meta.json", dpath, entries[j]->d_name);
            char *meta = read_file(mpath);
            if (!meta)
                goto next_entry;

            struct task *t = &tasks[ntasks];
            pathf(t->id, sizeof t->id, "%s/%s", domains[i]->d_name, entries[j]->d_name);
            pathf(t->name, sizeof t->name, "%s", entries[j]->d_name);
            pathf(t->dir, sizeof t->dir, "%s/%s", dpath, entries[j]->d_name);
            json_str(meta, "title", t->title, sizeof t->title);
            json_str(meta, "domain", t->domain, sizeof t->domain);
            json_str(meta, "objective", t->objective, sizeof t->objective);
            json_str(meta, "difficulty", t->difficulty, sizeof t->difficulty);
            t->est_min = json_int(meta, "est_min", 5);
            t->nprereq = json_str_array(meta, "prereq", t->prereq, MAX_PREREQ);
            free(meta);
            ntasks++;
        next_entry:
            free(entries[j]);
        }
        free(entries);
    next_domain:
        free(domains[i]);
    }
    free(domains);
}

/* strip a leading "nn-" ordering prefix for fuzzy matching */
static const char *strip_num(const char *s)
{
    const char *p = s;
    while (isdigit((unsigned char)*p))
        p++;
    return (p > s && *p == '-') ? p + 1 : s;
}

/* Like resolve() below but soft: NULL when nothing (or more than one thing)
 * matches, instead of exiting. Used for advisory lookups (prereq pointers)
 * where a bad id must never break the command being run. */
static struct task *find_task(const char *ident)
{
    for (int i = 0; i < ntasks; i++)
        if (strcmp(tasks[i].id, ident) == 0)
            return &tasks[i];
    struct task *found = NULL;
    int nfound = 0;
    for (int i = 0; i < ntasks; i++) {
        if (strcmp(tasks[i].name, ident) == 0 ||
            strcmp(strip_num(tasks[i].name), strip_num(ident)) == 0) {
            found = &tasks[i];
            nfound++;
        }
    }
    return nfound == 1 ? found : NULL;
}

static struct task *resolve(const char *ident)
{
    for (int i = 0; i < ntasks; i++)
        if (strcmp(tasks[i].id, ident) == 0)
            return &tasks[i];

    struct task *found = NULL;
    int nfound = 0;
    for (int i = 0; i < ntasks; i++) {
        if (strcmp(tasks[i].name, ident) == 0 ||
            strcmp(strip_num(tasks[i].name), strip_num(ident)) == 0) {
            found = &tasks[i];
            nfound++;
        }
    }
    if (nfound == 1)
        return found;
    if (nfound == 0) {
        fprintf(stderr, "No task matching '%s'. Try: %s list\n", ident, self_cmd);
    } else {
        fprintf(stderr, "'%s' is ambiguous:", ident);
        for (int i = 0; i < ntasks; i++)
            if (strcmp(tasks[i].name, ident) == 0 ||
                strcmp(strip_num(tasks[i].name), strip_num(ident)) == 0)
                fprintf(stderr, " %s", tasks[i].id);
        fputc('\n', stderr);
    }
    exit(1);
}

/* ---- progress state -------------------------------------------------------
 * Plain-text, one record per line — trivially diffable and parseable:
 *     xp 120
 *     streak 3 2026-07-01
 *     task 05-pointers/03-swap done 4 2026-07-01
 */

struct entry {
    char id[SLEN];
    char status[16]; /* "done" | "started" */
    int attempts;
    char completed[11];
    int reps;        /* successful passes so far (spaced repetition) */
    char due[11];    /* next review date YYYY-MM-DD, "" = none scheduled */
};

static struct entry entries[MAX_TASKS];
static int nentries;
static int xp;
static int streak_count;
static char streak_last[11];
static char current_id[SLEN];   /* task train/learn/start last handed you */
static int recent_picks[8];     /* recent train pick kinds (SR_NEW/SR_REVIEW) */
static int nrecent;

static void state_path(char out[PLEN])
{
    const char *xdg = getenv("XDG_STATE_HOME");
    if (xdg && *xdg)
        pathf(out, PLEN, "%s/ctrain/progress", xdg);
    else
        pathf(out, PLEN, "%s/.local/state/ctrain/progress", getenv("HOME") ? getenv("HOME") : ".");
}

static struct entry *entry_for(const char *id, int create)
{
    for (int i = 0; i < nentries; i++)
        if (strcmp(entries[i].id, id) == 0)
            return &entries[i];
    if (!create || nentries >= MAX_TASKS)
        return NULL;
    struct entry *e = &entries[nentries++];
    pathf(e->id, sizeof e->id, "%s", id);
    pathf(e->status, sizeof e->status, "started");
    e->attempts = 0;
    e->completed[0] = '\0';
    e->reps = 0;
    e->due[0] = '\0';
    return e;
}

static void load_state(void)
{
    char path[PLEN];
    state_path(path);
    char *body = read_file(path);
    if (!body)
        return;
    char *save = NULL;
    for (char *line = strtok_r(body, "\n", &save); line; line = strtok_r(NULL, "\n", &save)) {
        char a[SLEN], b[SLEN], c[SLEN], d[SLEN];
        int n, reps;
        if (sscanf(line, "xp %d", &n) == 1) {
            xp = n;
        } else if (sscanf(line, "streak %d %10s", &n, a) == 2) {
            streak_count = n;
            pathf(streak_last, sizeof streak_last, "%s", a);
        } else if (sscanf(line, "current %255s", a) == 1) {
            pathf(current_id, sizeof current_id, "%s", a);
        } else if (strncmp(line, "recent ", 7) == 0) {
            /* "recent new review ..." — kinds of the last few train picks */
            char *rs = NULL;
            nrecent = 0;
            for (char *tok = strtok_r(line + 7, " ", &rs);
                 tok && nrecent < (int)(sizeof recent_picks / sizeof *recent_picks);
                 tok = strtok_r(NULL, " ", &rs))
                recent_picks[nrecent++] = strcmp(tok, "review") == 0 ? SR_REVIEW : SR_NEW;
        } else {
            /* task <id> <status> <attempts> <completed> [reps] [due]
             * The last two fields are optional so older progress files load. */
            int got = sscanf(line, "task %255s %15s %d %10s %d %10s",
                             a, b, &n, c, &reps, d);
            if (got >= 4) {
                struct entry *e = entry_for(a, 1);
                if (e) {
                    pathf(e->status, sizeof e->status, "%s", b);
                    e->attempts = n;
                    pathf(e->completed, sizeof e->completed, "%s", strcmp(c, "-") ? c : "");
                    e->reps = got >= 5 ? reps : 0;
                    if (got >= 6 && strcmp(d, "-") != 0)
                        pathf(e->due, sizeof e->due, "%s", d);
                    else
                        e->due[0] = '\0';
                }
            }
        }
    }
    free(body);
}

static void save_state(void)
{
    char path[PLEN], dir[PLEN];
    state_path(path);
    pathf(dir, sizeof dir, "%s", path);
    char *slash = strrchr(dir, '/');
    if (slash) {
        *slash = '\0';
        mkdir_p(dir);
    }
    FILE *f = fopen(path, "w");
    if (!f) {
        perror("saving progress");
        return;
    }
    fprintf(f, "xp %d\n", xp);
    fprintf(f, "streak %d %s\n", streak_count, streak_last[0] ? streak_last : "-");
    if (current_id[0])
        fprintf(f, "current %s\n", current_id);
    if (nrecent > 0) {
        fputs("recent", f);
        for (int i = 0; i < nrecent; i++)
            fprintf(f, " %s", recent_picks[i] == SR_REVIEW ? "review" : "new");
        fputc('\n', f);
    }
    for (int i = 0; i < nentries; i++)
        fprintf(f, "task %s %s %d %s %d %s\n", entries[i].id, entries[i].status,
                entries[i].attempts, entries[i].completed[0] ? entries[i].completed : "-",
                entries[i].reps, entries[i].due[0] ? entries[i].due : "-");
    fclose(f);
}

static void bump_streak(void)
{
    char today[11];
    today_str(today);
    if (strcmp(streak_last, today) == 0)
        return;
    long last = date_ordinal(streak_last);
    long now = date_ordinal(today);
    streak_count = (last >= 0 && now - last == 1) ? streak_count + 1 : 1;
    pathf(streak_last, sizeof streak_last, "%s", today);
}

static long today_ordinal(void)
{
    char t[11];
    today_str(t);
    return date_ordinal(t);
}

/* Record the next review date, sr_review_interval(reps) days out from today. */
static void schedule_review(struct entry *e, int reps)
{
    e->reps = reps;
    char today[11];
    today_str(today);
    struct tm tm = {0};
    if (sscanf(today, "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday) != 3) {
        e->due[0] = '\0';
        return;
    }
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    tm.tm_hour = 12; /* midday avoids DST edge cases, as in date_ordinal */
    tm.tm_mday += (int)sr_review_interval(reps);
    mktime(&tm); /* normalises an overflowed day-of-month */
    strftime(e->due, sizeof e->due, "%Y-%m-%d", &tm);
}

/* ---- fundamentals-first ordering ----------------------------------------- */

static int curriculum_cmp(const void *a, const void *b)
{
    int ia = *(const int *)a, ib = *(const int *)b;
    return sr_curriculum_cmp(tasks[ia].id, tasks[ib].id);
}

/* Fill idx[0..ntasks) with task indices in teaching order (see sr.h). */
static void curriculum_order(int *idx)
{
    for (int i = 0; i < ntasks; i++)
        idx[i] = i;
    qsort(idx, (size_t)ntasks, sizeof *idx, curriculum_cmp);
}

/* First not-yet-done task in teaching order; resumes in-progress work too. */
static struct task *next_new(void)
{
    int idx[MAX_TASKS];
    curriculum_order(idx);
    for (int k = 0; k < ntasks; k++) {
        struct entry *e = entry_for(tasks[idx[k]].id, 0);
        if (!(e && strcmp(e->status, "done") == 0))
            return &tasks[idx[k]];
    }
    return NULL;
}

/* ---- soft prerequisite pointers -------------------------------------------
 * A task's meta.json may list `prereq` — ids of foundations lessons it leans
 * on. If the learner hasn't passed one yet, learn/train print a one-line
 * nudge to take that lesson first. Advisory only: it NEVER blocks or changes
 * what runs, and goes silent once the lesson has been passed. */

static void prereq_advice(const struct task *t)
{
    for (int i = 0; i < t->nprereq; i++) {
        const struct task *p = find_task(t->prereq[i]);
        if (!p || p == t)
            continue;
        struct entry *e = entry_for(p->id, 0);
        if (e && strcmp(e->status, "done") == 0)
            continue;
        printf("%sheads-up%s%s  this task leans on \"%s\" — new to it? "
               "learn it first:  %s%s learn %s\n",
               tty ? "\033[33m" : "", col_off(), dim_on(),
               p->title[0] ? p->title : p->name, col_off(), self_cmd, p->name);
    }
}

/* ---- work directory ------------------------------------------------------ */

static void work_dir(const struct task *t, char out[PLEN])
{
    pathf(out, PLEN, "%s/work/%s", root, t->id);
}

/* copy starter.c (or starter/ tree) into the work dir as the learner's file */
static void materialise_starter(const struct task *t, const char *wdir)
{
    char from[PLEN], to[PLEN];
    struct stat st;

    pathf(from, sizeof from, "%s/starter", t->dir);
    if (stat(from, &st) == 0 && S_ISDIR(st.st_mode)) {
        struct dirent **files;
        int nf = scandir(from, &files, NULL, name_cmp);
        for (int i = 0; i < nf; i++) {
            if (files[i]->d_name[0] != '.') {
                char f2[PLEN], t2[PLEN];
                pathf(f2, sizeof f2, "%s/%s", from, files[i]->d_name);
                pathf(t2, sizeof t2, "%s/%s", wdir, files[i]->d_name);
                copy_file(f2, t2);
            }
            free(files[i]);
        }
        if (nf >= 0)
            free(files);
        return;
    }

    pathf(from, sizeof from, "%s/starter.c", t->dir);
    pathf(to, sizeof to, "%s/main.c", wdir);
    if (stat(from, &st) == 0) {
        copy_file(from, to);
    } else {
        FILE *f = fopen(to, "w");
        if (f) {
            fprintf(f, "/* %s — write your solution here. */\n", t->title);
            fclose(f);
        }
    }
}

static int run_script(const struct task *t, const char *script)
{
    char spath[PLEN];
    pathf(spath, sizeof spath, "%s/%s", t->dir, script);
    if (access(spath, R_OK) != 0)
        return 0;

    char wdir[PLEN], src[PLEN];
    work_dir(t, wdir);
    pathf(src, sizeof src, "%s/main.c", wdir);
    setenv("CTRAIN_ROOT", root, 1);
    setenv("TASK_DIR", t->dir, 1);
    setenv("WORK_DIR", wdir, 1);
    setenv("SRC", src, 1);

    fflush(stdout); /* keep our output ordered ahead of the child's */
    pid_t pid = fork();
    if (pid == 0) {
        execlp("bash", "bash", spath, (char *)NULL);
        _exit(127);
    }
    int status = 0;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

/* ---- commands ------------------------------------------------------------ */

static void print_status_line(void)
{
    int done = 0;
    for (int i = 0; i < nentries; i++)
        if (strcmp(entries[i].status, "done") == 0)
            done++;
    printf("%sXP%s %d   %sStreak%s %d\xF0\x9F\x94\xA5   %sDone%s %d\n\n",
           bld_on(), col_off(), xp, bld_on(), col_off(), streak_count,
           bld_on(), col_off(), done);
}

static void cmd_list(void)
{
    if (ntasks == 0) {
        printf("%sNo tasks found yet.%s\n", dim_on(), col_off());
        return;
    }
    putchar('\n');
    char current[SLEN] = "";
    for (int i = 0; i < ntasks; i++) {
        struct task *t = &tasks[i];
        char dom[SLEN];
        pathf(dom, sizeof dom, "%s", t->id);
        char *slash = strchr(dom, '/');
        if (slash)
            *slash = '\0';
        if (strcmp(dom, current) != 0) {
            pathf(current, sizeof current, "%s", dom);
            int total = 0, done = 0;
            for (int j = 0; j < ntasks; j++)
                if (strncmp(tasks[j].id, dom, strlen(dom)) == 0 &&
                    tasks[j].id[strlen(dom)] == '/') {
                    total++;
                    struct entry *e = entry_for(tasks[j].id, 0);
                    if (e && strcmp(e->status, "done") == 0)
                        done++;
                }
            printf("%s%s━━ %s %s%s(%d/%d done) ━━━━━━━━━━━━━━━%s\n",
                   tty ? "\033[34m" : "", bld_on(),
                   t->domain[0] ? t->domain : dom,
                   col_off(), dim_on(), done, total, col_off());
        }
        struct entry *e = entry_for(t->id, 0);
        const char *mark = (e && strcmp(e->status, "done") == 0) ? GRN("✓")
                         : (e ? YEL("~") : DIM("•"));
        printf("  %s %-26s %s%s%s", mark, t->name, dim_on(), t->title, col_off());
        if (t->est_min)
            printf("  %s~%dm%s", dim_on(), t->est_min, col_off());
        if (strcmp(t->difficulty, "advanced") == 0 || strcmp(t->difficulty, "bonus") == 0)
            printf("  %s[%s]%s", tty ? "\033[33m" : "", t->difficulty, col_off());
        putchar('\n');
        if (i + 1 == ntasks || strncmp(tasks[i + 1].id, current, strlen(current)) != 0)
            putchar('\n');
    }
    print_status_line();
    printf("%sNew to this?      %s%s learn%s%s  teaches the next one first.%s\n",
           dim_on(), col_off(), invoke_cmd, dim_on(), dim_on(), col_off());
    printf("%sKnow it already?  %s%s train%s%s  just picks what to practise.%s\n",
           dim_on(), col_off(), invoke_cmd, dim_on(), dim_on(), col_off());
    printf("%sOr start one by name:  %s start <name>%s\n", dim_on(), self_cmd, col_off());
}

static void cmd_status(void)
{
    putchar('\n');
    print_status_line();
    int done = 0;
    for (int i = 0; i < nentries; i++)
        if (strcmp(entries[i].status, "done") == 0)
            done++;
    printf("%s%d/%d tasks complete%s\n", dim_on(), done, ntasks, col_off());
}

static void show_prompt(const struct task *t)
{
    printf("\n%s%s — %s%s\n", bld_on(), t->domain, t->title, col_off());
    printf("%sobjective: %s%s\n", dim_on(), t->objective, col_off());
    printf("%sid: %s   difficulty: %s   ~%dm%s\n", dim_on(), t->id,
           t->difficulty[0] ? t->difficulty : "core", t->est_min, col_off());
    for (int i = 0; i < 60; i++)
        fputs("─", stdout);
    putchar('\n');
    char ppath[PLEN];
    pathf(ppath, sizeof ppath, "%s/prompt.md", t->dir);
    char *prompt = read_file(ppath);
    if (prompt) {
        fputs(prompt, stdout);
        free(prompt);
    }
    for (int i = 0; i < 60; i++)
        fputs("─", stdout);
    putchar('\n');
}

/* Show a task's spec, create its work dir if needed, mark it current. Shared by
 * start, train and learn. */
static void launch_task(struct task *t)
{
    show_prompt(t);

    char wdir[PLEN], main_c[PLEN];
    work_dir(t, wdir);
    pathf(main_c, sizeof main_c, "%s/main.c", wdir);
    if (access(main_c, R_OK) == 0) {
        printf("%sWork dir already exists — leaving your code untouched.%s\n", dim_on(), col_off());
    } else {
        if (mkdir_p(wdir) != 0)
            die("cannot create work directory");
        materialise_starter(t, wdir);
    }
    entry_for(t->id, 1);
    pathf(current_id, sizeof current_id, "%s", t->id);
    save_state();
    printf("%sReady.%s%s  Edit %s/main.c then:  %s%s check%s\n",
           tty ? "\033[32m" : "", col_off(), dim_on(), wdir, col_off(),
           invoke_cmd, col_off());
}

static void cmd_start(const char *ident)
{
    launch_task(resolve(ident));
}

/* Resolve an id, falling back to the current task when none is given. Exits
 * with a helpful message if there is nothing to act on. */
static struct task *resolve_or_current(const char *ident, const char *verb)
{
    if (!ident || !*ident)
        ident = current_id[0] ? current_id : NULL;
    if (!ident) {
        fprintf(stderr, "nothing to %s — run '%s train' first, "
                        "or name a task: %s %s <id>\n",
                verb, invoke_cmd, self_cmd, verb);
        exit(1);
    }
    return resolve(ident);
}

static void cmd_check(const char *ident)
{
    struct task *t = resolve_or_current(ident, "check");
    char wdir[PLEN];
    work_dir(t, wdir);
    struct stat st;
    if (stat(wdir, &st) != 0) {
        printf("No work directory yet — run:  %s start %s\n", self_cmd, t->name);
        exit(1);
    }
    printf("\n%sGrading: %s%s\n", bld_on(), t->title, col_off());
    int rc = run_script(t, "grade.sh");
    struct entry *e = entry_for(t->id, 1);
    e->attempts++;
    if (rc == 0) {
        int was_review = strcmp(e->status, "done") == 0;
        pathf(e->status, sizeof e->status, "done");
        today_str(e->completed);
        int reps = was_review ? e->reps + 1 : 1;
        int gain = was_review ? 5 : 10 + t->est_min; /* reviews are cheaper */
        schedule_review(e, reps);
        xp += gain;
        bump_streak();
        printf("\n%s+%d XP%s   (streak %d\xF0\x9F\x94\xA5)\n",
               tty ? "\033[32m" : "", gain, col_off(), streak_count);
        printf("%sNext review in %ldd.  Next task:  %s train%s\n",
               dim_on(), sr_review_interval(reps), invoke_cmd, col_off());
    } else {
        if (strcmp(e->status, "done") == 0) {
            /* a lapse on a task you'd passed — relearn from the start, due now */
            e->reps = 0;
            today_str(e->due);
        }
        printf("%sFix it and re-run  %s check%s%s   (stuck?  %s solution %s)%s\n",
               dim_on(), invoke_cmd, col_off(), dim_on(), self_cmd, t->name, col_off());
    }
    save_state();
    exit(rc == 0 ? 0 : 1);
}

static void cmd_solution(const char *ident)
{
    struct task *t = resolve_or_current(ident, "solution");
    printf("\n%sReference solution — %s%s\n", bld_on(), t->title, col_off());
    printf("%s(one valid way; the grader accepts any correct implementation)%s\n",
           dim_on(), col_off());
    for (int i = 0; i < 60; i++)
        fputs("─", stdout);
    putchar('\n');

    char spath[PLEN];
    struct stat st;
    pathf(spath, sizeof spath, "%s/solution", t->dir);
    if (stat(spath, &st) == 0 && S_ISDIR(st.st_mode)) {
        struct dirent **files;
        int nf = scandir(spath, &files, NULL, name_cmp);
        for (int i = 0; i < nf; i++) {
            if (files[i]->d_name[0] != '.') {
                char fpath[PLEN];
                pathf(fpath, sizeof fpath, "%s/%s", spath, files[i]->d_name);
                printf("%s// ── %s ──%s\n", dim_on(), files[i]->d_name, col_off());
                char *body = read_file(fpath);
                if (body) {
                    fputs(body, stdout);
                    free(body);
                }
            }
            free(files[i]);
        }
        if (nf >= 0)
            free(files);
    } else {
        pathf(spath, sizeof spath, "%s/solution.c", t->dir);
        char *body = read_file(spath);
        if (body) {
            fputs(body, stdout);
            free(body);
        } else {
            printf("%sNo reference solution recorded.%s\n", dim_on(), col_off());
        }
    }
    for (int i = 0; i < 60; i++)
        fputs("─", stdout);
    putchar('\n');
}

static void cmd_reset(const char *ident)
{
    struct task *t = resolve_or_current(ident, "reset");
    char wdir[PLEN], main_c[PLEN], bak[PLEN];
    work_dir(t, wdir);
    pathf(main_c, sizeof main_c, "%s/main.c", wdir);
    pathf(bak, sizeof bak, "%s/main.c.bak", wdir);
    if (mkdir_p(wdir) != 0)
        die("cannot create work directory");
    if (access(main_c, R_OK) == 0) {
        rename(main_c, bak);
        printf("%sYour previous code was kept at %s%s\n", dim_on(), bak, col_off());
    }
    materialise_starter(t, wdir);
    struct entry *e = entry_for(t->id, 1);
    pathf(e->status, sizeof e->status, "started");
    save_state();
    printf("%sReset.%s Starter restored in %s\n", tty ? "\033[32m" : "", col_off(), wdir);
}

/* ---- train: pick the next task automatically ----------------------------- */

static void cmd_train(void)
{
    if (ntasks == 0) {
        printf("%sNo tasks found yet.%s\n", dim_on(), col_off());
        return;
    }
    int idx[MAX_TASKS];
    curriculum_order(idx);
    static sr_task sts[MAX_TASKS];
    for (int k = 0; k < ntasks; k++) {
        struct entry *e = entry_for(tasks[idx[k]].id, 0);
        sts[k].passed = e && strcmp(e->status, "done") == 0;
        sts[k].attempts = e ? e->attempts : 0;
        sts[k].scheduled = e && e->due[0];
        sts[k].due_ord = sts[k].scheduled ? date_ordinal(e->due) : 0;
    }
    long today = today_ordinal();
    int kind = SR_NONE;
    int chosen = sr_choose(sts, ntasks, today, recent_picks, nrecent, &kind);

    if (chosen < 0) {
        putchar('\n');
        printf("%sAll caught up — nothing due and no new tasks.%s\n",
               tty ? "\033[32m\033[1m" : "", col_off());
        putchar('\n');
        print_status_line();
        return;
    }

    struct task *t = &tasks[idx[chosen]];

    /* remember this pick (window of 4) for the two-in-a-row wall */
    int cap = 4;
    if (nrecent >= cap) {
        memmove(recent_picks, recent_picks + 1, (size_t)(cap - 1) * sizeof *recent_picks);
        recent_picks[cap - 1] = kind;
        nrecent = cap;
    } else {
        recent_picks[nrecent++] = kind;
    }

    char reason[128];
    if (kind == SR_REVIEW) {
        long over = sts[chosen].scheduled ? today - sts[chosen].due_ord : 0;
        if (over > 0)
            snprintf(reason, sizeof reason, "review — %ldd overdue", over);
        else
            snprintf(reason, sizeof reason, "due for review");
    } else if (sts[chosen].attempts > 0) {
        snprintf(reason, sizeof reason, "resume — started but not yet passed");
    } else {
        snprintf(reason, sizeof reason, "new material, next in sequence");
    }

    putchar('\n');
    print_status_line();
    printf("%s%s%s  %s%s%s\n",
           kind == SR_REVIEW ? (tty ? "\033[33m" : "") : (tty ? "\033[32m" : ""),
           kind == SR_REVIEW ? "REVIEW" : " NEW  ", col_off(),
           dim_on(), reason, col_off());
    if (kind == SR_REVIEW)
        printf("%s(your last solution is kept — re-solve from memory, or %s reset %s "
               "for a clean slate)%s\n", dim_on(), self_cmd, t->name, col_off());
    prereq_advice(t);
    launch_task(t);
}

/* ---- learn: an interactive tutor ----------------------------------------- */

/* Prompt the learner and return the first non-space key pressed, lowercased.
 * A closed/empty stdin (piped, non-interactive) reads as Enter ('\n'). */
static int ask_key(const char *prompt)
{
    fputs(prompt, stdout);
    fflush(stdout);
    char buf[64];
    if (!fgets(buf, sizeof buf, stdin)) {
        putchar('\n');
        return '\n';
    }
    for (const char *p = buf; *p; p++)
        if (*p != ' ' && *p != '\t')
            return tolower((unsigned char)*p);
    return '\n';
}

static void print_block(const char *src)
{
    for (const char *p = src; *p;) {
        const char *nl = strchr(p, '\n');
        int len = nl ? (int)(nl - p) : (int)strlen(p);
        printf("    %s%.*s%s\n", tty ? "\033[32m" : "", len, p, col_off());
        if (!nl)
            break;
        p = nl + 1;
    }
}

static void rm_temp(const char *dir)
{
    static const char *names[] = {"snippet.c", "snippet", "cc.log"};
    char p[PLEN];
    for (size_t i = 0; i < sizeof names / sizeof *names; i++) {
        pathf(p, sizeof p, "%s/%s", dir, names[i]);
        unlink(p);
    }
    rmdir(dir);
}

/* Compile and run a lesson's ```run``` block as a standalone C program so the
 * learner sees real output — the C analogue of pytrain running Python. */
static void compile_and_run(const char *src)
{
    const char *tmp = getenv("TMPDIR");
    if (!tmp || !*tmp)
        tmp = "/tmp";
    char dir[PLEN];
    pathf(dir, sizeof dir, "%s/ctrain-lesson.XXXXXX", tmp);
    if (!mkdtemp(dir)) {
        printf("%s  (could not create a temp dir to run the snippet)%s\n", dim_on(), col_off());
        return;
    }
    char cpath[PLEN], bpath[PLEN], log[PLEN];
    pathf(cpath, sizeof cpath, "%s/snippet.c", dir);
    pathf(bpath, sizeof bpath, "%s/snippet", dir);
    pathf(log, sizeof log, "%s/cc.log", dir);
    FILE *f = fopen(cpath, "w");
    if (f) {
        fputs(src, f);
        fputc('\n', f);
        fclose(f);
    }

    const char *cc = getenv("CC");
    if (!cc || !*cc)
        cc = "cc";

    /* compile, capturing warnings/errors to a log we can show on failure */
    fflush(stdout);
    pid_t pid = fork();
    if (pid == 0) {
        int fd = open(log, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd >= 0) {
            dup2(fd, 1);
            dup2(fd, 2);
            close(fd);
        }
        execlp(cc, cc, "-std=c11", "-Wall", "-o", bpath, cpath, "-lm", (char *)NULL);
        _exit(127);
    }
    int status = 0;
    waitpid(pid, &status, 0);
    int crc = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
    if (crc != 0) {
        printf("%s  (it didn't compile — the compiler said:)%s\n", dim_on(), col_off());
        char *cclog = read_file(log);
        if (cclog) {
            for (char *line = strtok(cclog, "\n"); line; line = strtok(NULL, "\n"))
                printf("    %s%s%s\n", dim_on(), line, col_off());
            free(cclog);
        }
        rm_temp(dir);
        return;
    }

    /* run it — inherit stdout/stderr so output appears live */
    fflush(stdout);
    pid = fork();
    if (pid == 0) {
        execl(bpath, bpath, (char *)NULL);
        _exit(127);
    }
    waitpid(pid, &status, 0);
    rm_temp(dir);
}

/* Walk a parsed lesson. Returns 0 if the learner quit early, 1 if finished. */
static int run_lesson(lesson *L)
{
    for (int i = 0; i < L->n; i++) {
        lesson_beat *b = &L->beats[i];
        if (b->kind == BEAT_PROSE) {
            fputs(b->text, stdout);
            putchar('\n');
        } else if (b->kind == BEAT_PAUSE) {
            if (ask_key(DIM("\n  [Enter to continue \xC2\xB7 q to stop] ")) == 'q')
                return 0;
            putchar('\n');
        } else { /* BEAT_RUN */
            putchar('\n');
            printf("%s  Try this — C:%s\n", bld_on(), col_off());
            print_block(b->text);
            int k = ask_key(DIM("  [Enter to compile & run it \xC2\xB7 s to skip \xC2\xB7 q to stop] "));
            if (k == 'q')
                return 0;
            if (k == 's')
                continue;
            printf("%s  \xE2\x94\x80\xE2\x94\x80 output \xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80%s\n", dim_on(), col_off());
            compile_and_run(b->text);
            printf("%s  \xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80%s\n", dim_on(), col_off());
        }
    }
    return 1;
}

static void offer_setup(struct task *t)
{
    printf("%sSet up its work directory and try it now? %s", bld_on(), col_off());
    if (ask_key(DIM("[Enter = yes, q = later] ")) != 'q')
        launch_task(t);
    else
        printf("%sWhen you're ready:  %s start %s%s\n", dim_on(), self_cmd, t->name, col_off());
}

static void cmd_learn(const char *ident)
{
    struct task *t;
    if (ident && *ident) {
        t = resolve(ident);
    } else {
        t = next_new();
        if (!t) {
            printf("%sYou've passed every task already. Revisit a lesson with:  %s%s learn <id>\n",
                   dim_on(), col_off(), self_cmd);
            return;
        }
    }

    printf("\n%sLESSON — %s: %s%s%s  [%s]%s\n", bld_on(), t->domain, t->title,
           col_off(), dim_on(), t->id, col_off());
    printf("%sobjective: %s%s\n", dim_on(), t->objective, col_off());
    prereq_advice(t);
    for (int i = 0; i < 60; i++)
        fputs("─", stdout);
    putchar('\n');

    char lpath[PLEN];
    pathf(lpath, sizeof lpath, "%s/learn.md", t->dir);
    char *body = read_file(lpath);
    if (!body) {
        printf("%sNo written lesson for this task yet — read the spec, give it a go,%s\n",
               dim_on(), col_off());
        printf("%sand reveal a worked solution with  %s%s solution%s\n\n",
               dim_on(), col_off(), invoke_cmd, col_off());
        char ppath[PLEN];
        pathf(ppath, sizeof ppath, "%s/prompt.md", t->dir);
        char *prompt = read_file(ppath);
        if (prompt) {
            fputs(prompt, stdout);
            free(prompt);
        }
        for (int i = 0; i < 60; i++)
            fputs("─", stdout);
        putchar('\n');
        offer_setup(t);
        return;
    }

    lesson L = lesson_parse(body);
    free(body);
    int finished = run_lesson(&L);
    lesson_free(&L);
    for (int i = 0; i < 60; i++)
        fputs("─", stdout);
    putchar('\n');
    if (!finished) {
        printf("%sLesson paused. Pick it back up any time:  %s%s learn %s\n",
               dim_on(), col_off(), self_cmd, t->name);
        return;
    }
    printf("%sThat's the lesson.%s%s  Best way to make it stick is to do it yourself.%s\n",
           tty ? "\033[32m" : "", col_off(), dim_on(), col_off());
    offer_setup(t);
}

static void usage(void)
{
    printf("ctrain — a console trainer for learning C (K&R-structured)\n\n"
           "New to the material?  Let it teach you first:\n"
           "  ctrain learn             teach the next new task, then set it up to try\n"
           "  ctrain train             auto-pick what to do next (a review, or new work)\n\n"
           "  ctrain list              all tasks, grouped by chapter, with status\n"
           "  ctrain start  <id>       show a task and create its work directory\n"
           "  ctrain check  [id]       compile + grade your code (defaults to current task)\n"
           "  ctrain solution [id]     reveal a reference solution\n"
           "  ctrain reset  [id]       restore the starter (yours -> main.c.bak)\n"
           "  ctrain status            XP, streak, completion\n\n"
           "<id> is \"domain/nn-name\" or just the unique trailing name. learn/train\n"
           "pick it for you; check/solution/reset default to the current task.\n");
}

int main(int argc, char **argv)
{
    tty = isatty(1);
    /* Name commands the way the learner invoked us: self_cmd is a runnable
     * path for hints that take an <id>; invoke_cmd is `make` when run under
     * make (MAKELEVEL set), so `make check` is suggested for the plain verbs. */
    self_cmd = (argv[0] && argv[0][0]) ? argv[0] : "./ctrain";
    invoke_cmd = getenv("MAKELEVEL") ? "make" : self_cmd;

    find_root(argv[0]);
    discover();
    load_state();

    if (argc < 2 || strcmp(argv[1], "help") == 0 || strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "--help") == 0) {
        usage();
        return 0;
    }
    const char *cmd = argv[1];
    const char *arg = argc >= 3 ? argv[2] : NULL;
    if (strcmp(cmd, "list") == 0) {
        cmd_list();
    } else if (strcmp(cmd, "status") == 0) {
        cmd_status();
    } else if (strcmp(cmd, "train") == 0 || strcmp(cmd, "next") == 0) {
        cmd_train();
    } else if (strcmp(cmd, "learn") == 0) {
        cmd_learn(arg);
    } else if (strcmp(cmd, "check") == 0) {
        cmd_check(arg);
    } else if (strcmp(cmd, "solution") == 0) {
        cmd_solution(arg);
    } else if (strcmp(cmd, "reset") == 0) {
        cmd_reset(arg);
    } else if (strcmp(cmd, "start") == 0) {
        if (!arg) {
            fprintf(stderr, "Usage: %s start <id>   (or just: %s train)\n", self_cmd, invoke_cmd);
            return 1;
        }
        cmd_start(arg);
    } else {
        fprintf(stderr, "Unknown command '%s'. Try: %s help\n", cmd, self_cmd);
        return 1;
    }
    return 0;
}
