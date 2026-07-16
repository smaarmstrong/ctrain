# Reading a crash

The starter `main.c` prints the value part of three `key:value` settings.
It compiles cleanly with no warnings — and segfaults. Reproduce the crash
first, exactly as the lesson taught:

```sh
cc -Wall -Werror -o settings main.c
./settings
echo $?        # expect 139: killed by a segfault
```

Note which lines printed before it died, work back from the symptom to
the pointer that went bad on the third setting, and fix it: a setting
with no `:` separator must not be dereferenced — print `(missing)` as
its value instead.

The fixed program must print exactly:

```
db01
5432
(missing)
```

and exit with status 0. Do not change the `settings` array — fix the
code that reads it. If your machine has ASan, run the crashing version
under it once (`cc -g -fsanitize=address ...`) and find the guilty line
in frame #0 before you fix anything; if not, the grader's sanitizer pass
degrades gracefully and CI's container covers it.

Your code goes in `main.c` in the work directory. Grade with
`ctrain check reading-a-crash`.
