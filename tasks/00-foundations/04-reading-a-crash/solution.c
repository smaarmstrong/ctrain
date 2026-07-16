#include <stdio.h>
#include <string.h>

int main(void)
{
    const char *settings[] = { "host:db01", "port:5432", "retries" };

    for (int i = 0; i < 3; i++) {
        const char *sep = strchr(settings[i], ':');
        if (sep == NULL)
            printf("(missing)\n");
        else
            printf("%s\n", sep + 1);
    }
    return 0;
}
