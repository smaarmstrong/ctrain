#ifndef PALETTE_H
#define PALETTE_H

enum color_id { PAL_BLACK, PAL_RED, PAL_GREEN, PAL_BLUE,
                PAL_YELLOW, PAL_WHITE, PAL_COUNT };

struct color {
    unsigned char r, g, b;
    const char *name;
};

/* Integer luma, 0..255. static inline: every includer gets its own
 * private copy, so nothing clashes at link time. */
static inline int color_luma(struct color c)
{
    return (299 * c.r + 587 * c.g + 114 * c.b) / 1000;
}

struct color palette_get(int id);
int palette_brightest(void);

#endif /* PALETTE_H */
