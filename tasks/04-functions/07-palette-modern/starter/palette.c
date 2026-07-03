#include "palette.h"

/* TODO: a static const table of PAL_COUNT entries, indexed with
 * designated initializers:
 *
 *     [PAL_RED] = {255, 0, 0, "red"},
 *
 * plus a _Static_assert that its size matches PAL_COUNT. */

struct color palette_get(int id)
{
    (void)id;
    struct color unknown = {0, 0, 0, "unknown"};
    return unknown; /* TODO */
}

int palette_brightest(void)
{
    return 0; /* TODO: scan every id with color_luma */
}
