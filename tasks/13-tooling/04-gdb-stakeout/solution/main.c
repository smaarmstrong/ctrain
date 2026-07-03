/* ledger — replays a batch of transaction records through a rolling
 * digest. It prints only the truncated final digest; every intermediate
 * state stays inside the process. See prompt.md: your job is to observe
 * one of those intermediate states with gdb, not to edit this file.
 */
#include <stdio.h>

static const unsigned records[] = {
    9021, 4417, 7788, 1203, 664,  9902, 3141, 774,  5926, 5358,
    9793, 2384, 6264, 3383, 2795, 288,  4197, 1693, 9937, 5105,
};
#define NREC (sizeof records / sizeof records[0])

static unsigned long mix(unsigned long acc, unsigned rec)
{
    acc ^= (unsigned long)rec * 2654435761u;
    acc = (acc << 7) | (acc >> 57);
    acc += rec >> 3;
    return acc;
}

int main(void)
{
    unsigned long acc = 0xfeed;
    for (size_t i = 0; i < NREC; i++)
        acc = mix(acc, records[i]);
    printf("processed %zu records\n", NREC);
    printf("final digest: %lu\n", acc % 100000);
    return 0;
}
