# running stats through struct pointers

Implement the functions below in `main.c`. Do **not** define `main` — the
grader links your file against a test harness that provides it.

Copy this struct definition into your file exactly as written (the
harness uses the same definition and reads the fields directly):

```c
struct stats {
    int  count;   /* how many values have been added */
    long sum;     /* their running total */
    int  min;     /* smallest value added; meaningless while count == 0 */
    int  max;     /* largest value added;  meaningless while count == 0 */
};
```

All functions receive the struct **by pointer** — use `->`. Pointers are
never `NULL`. `dst` and `src` in `stats_merge` never alias.

```c
/* Reset *s to empty: count and sum become 0.
 * min and max may hold anything until the first add. */
void stats_init(struct stats *s);

/* Fold one value into *s: bump count, add to sum, update min and max.
 * The first value added becomes both min and max. */
void stats_add(struct stats *s, int value);

/* Arithmetic mean of the values in *s, as a double.
 * Return 0.0 when count is 0. */
double stats_mean(const struct stats *s);

/* Fold everything recorded in *src into *dst, as if each of src's
 * values had been stats_add-ed to dst. Either side may be empty;
 * an empty src leaves dst unchanged. *src is not modified. */
void stats_merge(struct stats *dst, const struct stats *src);
```

Values are small; `sum` fits comfortably in a `long`. The harness checks
the struct fields after each call, then reruns everything under
sanitizers.

Grade with `ctrain check stats-pointer`.
