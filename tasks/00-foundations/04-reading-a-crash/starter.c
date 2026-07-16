/* Prints the value part of each key:value setting. Compiles clean.
 * Segfaults. Work back from the symptom — see prompt.md. */
#include <stdio.h>
#include <string.h>

int main(void)
{
    const char *settings[] = { "host:db01", "port:5432", "retries" };

    for (int i = 0; i < 3; i++) {
        const char *sep = strchr(settings[i], ':');
        printf("%s\n", sep + 1);
    }
    return 0;
}
