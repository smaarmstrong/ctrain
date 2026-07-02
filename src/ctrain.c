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

#define MAX_TASKS 512
#define SLEN 256
#define PLEN PATH_MAX

/* ---- colours (disabled when stdout is not a terminal) ------------------- */

static int tty;
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

/* ---- task discovery ------------------------------------------------------ */

struct task {
    char id[SLEN];      /* "05-pointers/03-swap" */
    char name[SLEN];    /* "03-swap" */
    char dir[PLEN];     /* absolute path to the task directory */
    char title[SLEN];
    char domain[SLEN];
    char objective[SLEN];
    char difficulty[32];
    int est_min;
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
        fprintf(stderr, "No task matching '%s'. Try: ctrain list\n", ident);
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
};

static struct entry entries[MAX_TASKS];
static int nentries;
static int xp;
static int streak_count;
static char streak_last[11];

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
        char a[SLEN], b[SLEN], c[SLEN];
        int n;
        if (sscanf(line, "xp %d", &n) == 1) {
            xp = n;
        } else if (sscanf(line, "streak %d %10s", &n, a) == 2) {
            streak_count = n;
            pathf(streak_last, sizeof streak_last, "%s", a);
        } else if (sscanf(line, "task %255s %15s %d %10s", a, b, &n, c) == 4) {
            struct entry *e = entry_for(a, 1);
            if (e) {
                pathf(e->status, sizeof e->status, "%s", b);
                e->attempts = n;
                pathf(e->completed, sizeof e->completed, "%s", strcmp(c, "-") ? c : "");
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
    for (int i = 0; i < nentries; i++)
        fprintf(f, "task %s %s %d %s\n", entries[i].id, entries[i].status,
                entries[i].attempts, entries[i].completed[0] ? entries[i].completed : "-");
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
    printf("%sStart one with:  ctrain start <name>%s\n", dim_on(), col_off());
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

static void cmd_start(const char *ident)
{
    struct task *t = resolve(ident);
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
    save_state();
    printf("%sReady.%s%s  Edit %s/main.c then:  %sctrain check %s\n",
           tty ? "\033[32m" : "", col_off(), dim_on(), wdir, col_off(), t->name);
}

static void cmd_check(const char *ident)
{
    struct task *t = resolve(ident);
    char wdir[PLEN];
    work_dir(t, wdir);
    struct stat st;
    if (stat(wdir, &st) != 0) {
        printf("No work directory yet — run:  ctrain start %s\n", t->name);
        exit(1);
    }
    printf("\n%sGrading: %s%s\n", bld_on(), t->title, col_off());
    int rc = run_script(t, "grade.sh");
    struct entry *e = entry_for(t->id, 1);
    e->attempts++;
    if (rc == 0) {
        int first = strcmp(e->status, "done") != 0;
        pathf(e->status, sizeof e->status, "done");
        today_str(e->completed);
        if (first) {
            int gain = 10 + t->est_min;
            xp += gain;
            bump_streak();
            printf("\n%s+%d XP%s   (streak %d\xF0\x9F\x94\xA5)\n",
                   tty ? "\033[32m" : "", gain, col_off(), streak_count);
        }
    }
    save_state();
    exit(rc == 0 ? 0 : 1);
}

static void cmd_solution(const char *ident)
{
    struct task *t = resolve(ident);
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
    struct task *t = resolve(ident);
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

static void usage(void)
{
    printf("ctrain — a console trainer for learning C (K&R-structured)\n\n"
           "  ctrain list              all tasks, grouped by chapter, with status\n"
           "  ctrain start  <id>       show a task and create its work directory\n"
           "  ctrain check  <id>       compile + grade your code\n"
           "  ctrain solution <id>     reveal a reference solution\n"
           "  ctrain reset  <id>       restore the starter (yours -> main.c.bak)\n"
           "  ctrain status            XP, streak, completion\n\n"
           "<id> is \"domain/nn-name\" or just the unique trailing name.\n");
}

int main(int argc, char **argv)
{
    tty = isatty(1);
    find_root(argv[0]);
    discover();
    load_state();

    if (argc < 2 || strcmp(argv[1], "help") == 0 || strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "--help") == 0) {
        usage();
        return 0;
    }
    const char *cmd = argv[1];
    if (strcmp(cmd, "list") == 0) {
        cmd_list();
    } else if (strcmp(cmd, "status") == 0) {
        cmd_status();
    } else if (strcmp(cmd, "start") == 0 || strcmp(cmd, "check") == 0 ||
               strcmp(cmd, "solution") == 0 || strcmp(cmd, "reset") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Usage: ctrain %s <id>\n", cmd);
            return 1;
        }
        if (strcmp(cmd, "start") == 0)
            cmd_start(argv[2]);
        else if (strcmp(cmd, "check") == 0)
            cmd_check(argv[2]);
        else if (strcmp(cmd, "solution") == 0)
            cmd_solution(argv[2]);
        else
            cmd_reset(argv[2]);
    } else {
        fprintf(stderr, "Unknown command '%s'. Try: ctrain help\n", cmd);
        return 1;
    }
    return 0;
}
