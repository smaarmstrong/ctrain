/* Parts inventory report.
 * Correct output, warning-riddled build. Fix every warning without
 * changing what the program prints. See prompt.md.
 */
#include <stdio.h>

struct part {
    const char *name;
    int qty;
    long price_cents;
};

static const struct part parts[] = {
    {"bolt", 12, 35},
    {"washer", 40, 5},
    {"bracket", 3, 249},
    {"rail", 2, 1200},
};

static long line_total(int qty, long price_cents, int verbose)
{
    return qty * price_cents;
}

int main(void)
{
    long grand = 0;
    int count = 0;
    char sep = '-';

    for (int i = 0; i < sizeof parts / sizeof parts[0]; i++) {
        long t = line_total(parts[i].qty, parts[i].price_cents, 0);
        printf("%-8s x%-3d = %d\n", parts[i].name, parts[i].qty, t);
        grand += t;
        count++;
    }
    printf("parts: %d grand total: %ld\n", count, grand);
    return 0;
}
