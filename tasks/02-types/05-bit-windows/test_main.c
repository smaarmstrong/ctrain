/* Test harness — provides main() and calls the learner's functions. */
#include <stdio.h>

unsigned get_bits(unsigned x, int pos, int n);
unsigned set_bits(unsigned x, int pos, int n, unsigned v);
unsigned toggle_bits(unsigned x, int pos, int n);

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
    /* get_bits */
    CHECK(get_bits(0xABCDu, 4, 8) == 0xBCu, "get_bits(0xABCD, 4, 8) == 0xBC");
    CHECK(get_bits(0xABCDu, 0, 4) == 0xDu, "low nibble of 0xABCD");
    CHECK(get_bits(0xABCDu, 12, 4) == 0xAu, "high nibble of 0xABCD");
    CHECK(get_bits(0x1u, 0, 1) == 1u, "single bit, set");
    CHECK(get_bits(0x2u, 0, 1) == 0u, "single bit, clear");
    CHECK(get_bits(0x80000000u, 31, 1) == 1u, "the very top bit");
    CHECK(get_bits(0xDEADBEEFu, 0, 32) == 0xDEADBEEFu,
          "full 32-bit window returns x (mask must not over-shift)");
    CHECK(get_bits(0xF0F0F0F0u, 28, 4) == 0xFu, "top nibble");

    /* set_bits */
    CHECK(set_bits(0xFFFFu, 4, 8, 0x12u) == 0xF12Fu,
          "set_bits(0xFFFF, 4, 8, 0x12) == 0xF12F");
    CHECK(set_bits(0x0u, 0, 4, 0x9u) == 0x9u, "set low nibble in zero");
    CHECK(set_bits(0xFFFFFFFFu, 0, 32, 0u) == 0u,
          "full-width replace with 0");
    CHECK(set_bits(0u, 0, 32, 0xCAFEBABEu) == 0xCAFEBABEu,
          "full-width replace with a value");
    CHECK(set_bits(0xAAAAu, 8, 4, 0xFFFFFFFFu) == 0xAFAAu,
          "only the low n bits of v are used");
    CHECK(set_bits(0x1234u, 31, 1, 1u) == 0x80001234u, "set the top bit");
    CHECK(set_bits(0x1234u, 4, 4, 0x3u) == 0x1234u,
          "replacing a field with its own value is a no-op");

    /* toggle_bits */
    CHECK(toggle_bits(0x00F0u, 4, 4) == 0x0000u,
          "toggle_bits(0x00F0, 4, 4) == 0");
    CHECK(toggle_bits(0x0000u, 4, 4) == 0x00F0u, "toggling zeros sets them");
    CHECK(toggle_bits(0xFFFFFFFFu, 0, 32) == 0u, "full-width toggle inverts");
    CHECK(toggle_bits(0x5555AAAAu, 0, 16) == 0x55555555u,
          "toggle the low half only");
    CHECK(toggle_bits(0x1u, 31, 1) == 0x80000001u, "toggle the top bit");
    unsigned x = 0x13579BDFu;
    CHECK(toggle_bits(toggle_bits(x, 7, 13), 7, 13) == x,
          "toggling twice restores the original");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("all checks passed\n");
    return 0;
}
