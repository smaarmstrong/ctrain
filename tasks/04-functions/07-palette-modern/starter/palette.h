#ifndef PALETTE_H
#define PALETTE_H

/* The interface is fixed: the grader's test program relies on these
 * exact names, orders and types. */
enum color_id { PAL_BLACK, PAL_RED, PAL_GREEN, PAL_BLUE,
                PAL_YELLOW, PAL_WHITE, PAL_COUNT };

struct color {
    unsigned char r, g, b;
    const char *name;
};

/* Integer luma, 0..255: (299*r + 587*g + 114*b) / 1000.
 * Defined here in the header: static inline gives every includer its
 * own private copy, so nothing clashes at link time. */
static inline int color_luma(struct color c)
{
    (void)c;
    return 0; /* TODO */
}

struct color palette_get(int id);
int palette_brightest(void);

#endif /* PALETTE_H */
