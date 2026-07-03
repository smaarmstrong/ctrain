# points and rectangles

Implement the functions below in `main.c`. Do **not** define `main` — the
grader links your file against a test harness that provides it.

Copy these struct definitions into your file exactly as written (the
harness uses the same definitions):

```c
struct point { int x; int y; };
struct rect  { struct point ll; struct point ur; };
```

A `struct rect` is **canonical** when `ll` holds the smaller x and the
smaller y of the two corners and `ur` holds the larger ones.

Implement:

```c
/* Build a point from two coordinates. */
struct point point_make(int x, int y);

/* Component-wise sum: result.x = a.x + b.x, result.y = a.y + b.y. */
struct point point_add(struct point a, struct point b);

/* p1 and p2 are two opposite corners of a rectangle, in any order.
 * Return the canonical rect. The corners may share an x or a y
 * (a degenerate, zero-area rectangle is fine). */
struct rect rect_canonical(struct point p1, struct point p2);

/* Area of a canonical rect: width times height. May be 0. */
int rect_area(struct rect r);

/* 1 if p lies inside canonical rect r, else 0.
 * "Inside" is half-open: ll.x <= p.x < ur.x  AND  ll.y <= p.y < ur.y.
 * So the lower-left corner is inside, the upper-right corner is not. */
int rect_contains(struct rect r, struct point p);
```

Everything is passed and returned **by value** — no pointers, no
allocation. Coordinates stay small; overflow is not tested.

Grade with `ctrain check points-and-rects`.
