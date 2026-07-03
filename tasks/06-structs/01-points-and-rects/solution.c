struct point { int x; int y; };
struct rect  { struct point ll; struct point ur; };

struct point point_make(int x, int y)
{
    struct point p = {x, y};
    return p;
}

struct point point_add(struct point a, struct point b)
{
    return point_make(a.x + b.x, a.y + b.y);
}

static int imin(int a, int b) { return a < b ? a : b; }
static int imax(int a, int b) { return a > b ? a : b; }

struct rect rect_canonical(struct point p1, struct point p2)
{
    struct rect r;
    r.ll = point_make(imin(p1.x, p2.x), imin(p1.y, p2.y));
    r.ur = point_make(imax(p1.x, p2.x), imax(p1.y, p2.y));
    return r;
}

int rect_area(struct rect r)
{
    return (r.ur.x - r.ll.x) * (r.ur.y - r.ll.y);
}

int rect_contains(struct rect r, struct point p)
{
    return r.ll.x <= p.x && p.x < r.ur.x &&
           r.ll.y <= p.y && p.y < r.ur.y;
}
