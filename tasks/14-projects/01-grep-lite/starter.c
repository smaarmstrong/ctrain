#include <stdio.h>
#include <string.h>

/* greplite [-n] [-c] [-v] PATTERN [FILE...]
 *
 * TODO:
 *   1. walk argv: collect -n/-c/-v until the first non-option (the pattern)
 *   2. no pattern -> usage on stderr, exit 2
 *   3. no files -> filter stdin; otherwise open each file in turn
 *   4. strstr() decides a match; -v inverts; -c counts instead of printing
 *   5. two or more FILE args -> "name:" prefix on every output line
 *   6. exit 0 if anything was selected, 1 if nothing, 2 on any error
 */

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    fprintf(stderr, "greplite: not implemented yet\n");
    return 2;
}
