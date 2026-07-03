#include <stdio.h>

struct item {
    const char *name;
    int qty;
    double price;
};

int main(void)
{
    static const struct item items[] = {
        {"bolt", 1250, 0.08},
        {"camshaft", 34, 145.50},
        {"gasket", 0, 12.99},
        {"oil-filter", 7, 8.50},
    };
    enum { N = sizeof items / sizeof items[0] };

    double total = 0.0;
    for (int i = 0; i < N; i++)
        total += items[i].qty * items[i].price;

    printf("%-12s%6s%10s%12s%7s\n", "item", "qty", "price", "value", "share");
    for (int i = 0; i < N; i++) {
        double value = items[i].qty * items[i].price;
        printf("%-12s%6d%10.2f%12.2f%6.1f%%\n",
               items[i].name, items[i].qty, items[i].price,
               value, 100.0 * value / total);
    }
    printf("%-12s%6s%10s%12.2f%6.1f%%\n", "TOTAL", "", "", total, 100.0);
    return 0;
}
