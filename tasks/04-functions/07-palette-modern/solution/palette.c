#include "palette.h"

/* Designated initializers: each entry is filed under its enum id, so
 * reordering the enum could never silently misfile a color. */
static const struct color palette[PAL_COUNT] = {
    [PAL_BLACK] = {0, 0, 0, "black"},
    [PAL_RED] = {255, 0, 0, "red"},
    [PAL_GREEN] = {0, 255, 0, "green"},
    [PAL_BLUE] = {0, 0, 255, "blue"},
    [PAL_YELLOW] = {255, 255, 0, "yellow"},
    [PAL_WHITE] = {255, 255, 255, "white"},
};

_Static_assert(sizeof palette / sizeof palette[0] == PAL_COUNT,
               "palette table must cover every color id");

struct color palette_get(int id)
{
    if (id < 0 || id >= PAL_COUNT) {
        struct color unknown = {0, 0, 0, "unknown"};
        return unknown;
    }
    return palette[id];
}

int palette_brightest(void)
{
    int best = 0;
    for (int id = 1; id < PAL_COUNT; id++)
        if (color_luma(palette[id]) > color_luma(palette[best]))
            best = id;
    return best;
}
