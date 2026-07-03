/* Test harness — provides main() and calls the learner's functions. */
#include <stdio.h>

struct point { int x; int y; };
struct rect  { struct point ll; struct point ur; };

struct point point_make(int x, int y);
struct point point_add(struct point a, struct point b);
struct rect rect_canonical(struct point p1, struct point p2);
int rect_area(struct rect r);
int rect_contains(struct rect r, struct point p);

static int failures;

#define CHECK(cond, msg)                                        \
    do {                                                        \
        if (cond) {                                             \
            printf("  ok    %s\n", msg);                        \
        } else {                                                \
            printf("  FAIL  %s\n", msg);                        \
            failures++;                                         \
        }                                                       \
    } while (0)

int main(void)
{
    struct point p = point_make(3, -4);
    CHECK(p.x == 3 && p.y == -4, "point_make sets both fields");

    struct point s = point_add(point_make(1, 2), point_make(10, -20));
    CHECK(s.x == 11 && s.y == -18, "point_add sums component-wise");

    struct rect r = rect_canonical(point_make(5, 1), point_make(2, 7));
    CHECK(r.ll.x == 2 && r.ll.y == 1 && r.ur.x == 5 && r.ur.y == 7,
          "rect_canonical reorders mixed corners");

    struct rect r2 = rect_canonical(point_make(-3, -3), point_make(4, 0));
    CHECK(r2.ll.x == -3 && r2.ll.y == -3 && r2.ur.x == 4 && r2.ur.y == 0,
          "rect_canonical keeps already-canonical corners");

    CHECK(rect_area(r) == 18, "area of a 3 x 6 rectangle is 18");
    struct rect flat = rect_canonical(point_make(1, 5), point_make(9, 5));
    CHECK(rect_area(flat) == 0, "degenerate rectangle has area 0");

    CHECK(rect_contains(r, point_make(3, 4)) == 1, "interior point is inside");
    CHECK(rect_contains(r, r.ll) == 1, "lower-left corner is inside");
    CHECK(rect_contains(r, r.ur) == 0, "upper-right corner is outside");
    CHECK(rect_contains(r, point_make(5, 3)) == 0, "right edge is outside");
    CHECK(rect_contains(r, point_make(3, 7)) == 0, "top edge is outside");
    CHECK(rect_contains(r, point_make(-1, 4)) == 0, "point left of the rect is outside");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
