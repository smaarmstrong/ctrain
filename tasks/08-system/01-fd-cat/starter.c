#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <unistd.h>

/* TODO:
 *   - no arguments: copy fd 0 to fd 1 until read() returns 0
 *   - otherwise: open each argv[i] read-only, copy it to fd 1, close it
 *   - loop on short writes; on any error write a message to fd 2 and
 *     exit with status 1
 */

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    static const char msg[] = "TODO\n";
    write(2, msg, sizeof msg - 1);
    return 0;
}
