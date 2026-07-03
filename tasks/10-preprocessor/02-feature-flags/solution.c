#include <stdio.h>

#if defined(PRO) && defined(TRIAL)
#error "PRO and TRIAL are mutually exclusive"
#endif

#ifndef MAX_ITEMS
#define MAX_ITEMS 4
#endif

#if MAX_ITEMS < 1
#error "MAX_ITEMS must be at least 1"
#endif

int main(void)
{
#ifdef PRO
    printf("edition: pro\n");
#else
    printf("edition: community\n");
#endif
    printf("items: %d\n", MAX_ITEMS);
#ifdef ENABLE_LOG
    printf("logging: on\n");
#else
    printf("logging: off\n");
#endif
    return 0;
}
