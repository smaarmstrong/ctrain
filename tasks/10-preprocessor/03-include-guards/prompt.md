# Include guards across a small header family

You inherit three files: `temperature.h`, `alarm.h` and `main.c`.
`alarm.h` needs the struct from `temperature.h`, so it includes it. The
program builds and runs — but only because `main.c` carefully includes
`alarm.h` alone. The headers themselves are fragile: include either of
them twice in one translation unit (directly, or once directly and once
through the other header) and compilation explodes with redefinition
errors, because each inclusion pastes the `struct`, `enum` and function
definitions again.

Fix the headers so that **any** combination works: each header may be
included any number of times, in any order, in the same translation
unit. The classic tool is an include guard —

```c
#ifndef SOMETHING_UNIQUE_H
#define SOMETHING_UNIQUE_H
/* ...the whole header body... */
#endif
```

— where every header uses its **own, unique** guard macro (a copy-pasted
guard name would make the second header silently disappear).
`#pragma once` is also accepted; the grader only checks behaviour.

Rules:

- Do not change what the headers define: `struct temp_reading`,
  `temp_whole()`, `enum alarm_level` with `ALARM_NONE`/`ALARM_WARN`/
  `ALARM_CRIT`, and `temp_alarm()` must keep exactly their current
  behaviour, visible to any file that includes the respective header.
- `alarm.h` must remain self-sufficient: including only `alarm.h` still
  provides `struct temp_reading` (keep its `#include "temperature.h"`).
- `main.c` needs no changes, and its output must stay byte-identical:

```
21.50C -> whole 21, alarm none
35.00C -> whole 35, alarm warn
41.20C -> whole 41, alarm crit
```

The grader (a) builds and runs your program and compares the output, and
(b) compiles a harness that includes both headers repeatedly and in both
orders, then calls the functions.

Grade with `ctrain check include-guards`.
