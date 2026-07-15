/*
 * lesson.h — parse a learn.md tutor script into a sequence of "beats".
 *
 * Pure and I/O-free (like sr.h) so it can be unit-tested. ctrain.c walks the
 * beats: prose is printed, a pause waits for the learner, and a run block is
 * COMPILED AND RUN as a C program so the learner sees real output — the C
 * analogue of pytrain running a Python snippet.
 *
 * A lesson is plain prose with two markers (mirroring redhat/pytrain):
 *   - a line that is exactly `---`            -> a pause beat
 *   - a fenced ```run ... ``` block           -> a run beat (C source, verbatim)
 * Everything else accumulates into prose beats.
 */
#ifndef CTRAIN_LESSON_H
#define CTRAIN_LESSON_H

#include <stdlib.h>
#include <string.h>

enum { BEAT_PROSE, BEAT_RUN, BEAT_PAUSE };

typedef struct {
    int   kind;  /* BEAT_PROSE | BEAT_RUN | BEAT_PAUSE */
    char *text;  /* prose text, or verbatim run-block source; NULL for a pause */
} lesson_beat;

typedef struct {
    lesson_beat *beats;
    int          n;
} lesson;

/* trim trailing '\n'/'\r'/spaces used only for marker detection */
static inline int lesson__is_marker(const char *line, const char *marker)
{
    while (*line == ' ' || *line == '\t')
        line++;
    size_t m = strlen(marker);
    if (strncmp(line, marker, m) != 0)
        return 0;
    if (strcmp(marker, "---") == 0) {
        const char *p = line + m;
        while (*p == ' ' || *p == '\t' || *p == '\r')
            p++;
        return *p == '\0';
    }
    return 1; /* fence prefix match is enough for ``` / ```run */
}

static inline void lesson__push(lesson *L, int kind, const char *s, size_t len)
{
    L->beats = realloc(L->beats, (size_t)(L->n + 1) * sizeof *L->beats);
    L->beats[L->n].kind = kind;
    if (s) {
        char *t = malloc(len + 1);
        memcpy(t, s, len);
        t[len] = '\0';
        L->beats[L->n].text = t;
    } else {
        L->beats[L->n].text = NULL;
    }
    L->n++;
}

/* True if a buffer holds anything other than whitespace. */
static inline int lesson__has_content(const char *s)
{
    for (; *s; s++)
        if (*s != ' ' && *s != '\t' && *s != '\n' && *s != '\r')
            return 1;
    return 0;
}

/* Parse `text` into beats. Caller frees with lesson_free. `text` is not
 * modified. Run blocks are captured verbatim (indentation preserved) with
 * blank leading/trailing lines trimmed. */
static inline lesson lesson_parse(const char *text)
{
    lesson L = {NULL, 0};
    if (!text)
        text = "";
    size_t tlen = strlen(text);
    char  *copy = malloc(tlen + 1);
    memcpy(copy, text, tlen + 1); /* private, mutable copy (feature-macro free) */
    /* Split into lines, keeping the array of pointers. */
    int    cap = 0, nlines = 0;
    char **lines = NULL;
    for (char *p = copy;;) {
        if (nlines >= cap) {
            cap = cap ? cap * 2 : 64;
            lines = realloc(lines, (size_t)cap * sizeof *lines);
        }
        lines[nlines++] = p;
        char *nl = strchr(p, '\n');
        if (!nl)
            break;
        *nl = '\0';
        p = nl + 1;
        if (*p == '\0')
            break;
    }

    char  *prose = NULL;
    size_t plen = 0, pcap = 0;
    /* flush accumulated prose as a beat if it has real content */
#define LESSON_FLUSH()                                                    \
    do {                                                                  \
        if (prose && lesson__has_content(prose))                         \
            lesson__push(&L, BEAT_PROSE, prose, strlen(prose));          \
        plen = 0;                                                         \
        if (prose)                                                       \
            prose[0] = '\0';                                             \
    } while (0)

    for (int i = 0; i < nlines; i++) {
        char *line = lines[i];
        if (lesson__is_marker(line, "---")) {
            LESSON_FLUSH();
            lesson__push(&L, BEAT_PAUSE, NULL, 0);
        } else if (lesson__is_marker(line, "```run")) {
            LESSON_FLUSH();
            /* collect verbatim until the closing fence */
            char  *blk = NULL;
            size_t blen = 0, bcap = 0;
            i++;
            for (; i < nlines; i++) {
                if (lesson__is_marker(lines[i], "```"))
                    break;
                size_t ll = strlen(lines[i]);
                if (blen + ll + 2 > bcap) {
                    bcap = (blen + ll + 2) * 2;
                    blk = realloc(blk, bcap);
                }
                memcpy(blk + blen, lines[i], ll);
                blen += ll;
                blk[blen++] = '\n';
            }
            if (blk) {
                blk[blen] = '\0';
                /* trim leading/trailing blank lines */
                char *start = blk;
                while (*start == '\n')
                    start++;
                char *end = blk + strlen(blk);
                while (end > start && (end[-1] == '\n' || end[-1] == ' ' ||
                                       end[-1] == '\t' || end[-1] == '\r'))
                    end--;
                if (end > start)
                    lesson__push(&L, BEAT_RUN, start, (size_t)(end - start));
                free(blk);
            }
        } else {
            size_t ll = strlen(line);
            if (plen + ll + 2 > pcap) {
                pcap = (plen + ll + 2) * 2 + 64;
                prose = realloc(prose, pcap);
            }
            if (plen)
                prose[plen++] = '\n';
            memcpy(prose + plen, line, ll);
            plen += ll;
            prose[plen] = '\0';
        }
    }
    LESSON_FLUSH();
#undef LESSON_FLUSH
    free(prose);
    free(lines);
    free(copy);
    return L;
}

static inline void lesson_free(lesson *L)
{
    for (int i = 0; i < L->n; i++)
        free(L->beats[i].text);
    free(L->beats);
    L->beats = NULL;
    L->n = 0;
}

#endif /* CTRAIN_LESSON_H */
