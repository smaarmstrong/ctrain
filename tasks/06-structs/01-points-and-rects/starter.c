struct point { int x; int y; };
struct rect  { struct point ll; struct point ur; };

struct point point_make(int x, int y)
{
    /* TODO */
    (void)x;
    (void)y;
    struct point p = {0, 0};
    return p;
}

struct point point_add(struct point a, struct point b)
{
    /* TODO */
    (void)b;
    return a;
}

struct rect rect_canonical(struct point p1, struct point p2)
{
    /* TODO: smaller coords go in ll, larger in ur. */
    struct rect r = {p1, p2};
    return r;
}

int rect_area(struct rect r)
{
    /* TODO */
    (void)r;
    return 0;
}

int rect_contains(struct rect r, struct point p)
{
    /* TODO: half-open — ll inclusive, ur exclusive. */
    (void)r;
    (void)p;
    return 0;
}
