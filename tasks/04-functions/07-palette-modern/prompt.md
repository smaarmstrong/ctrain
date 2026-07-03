# a color palette, the modern way

Build a tiny two-file color-palette module using three modern-C
idioms: an enum-indexed table written with **designated
initializers**, a **`static inline`** helper living in the header, and
a **`_Static_assert`** that keeps the table honest at compile time.

Your work directory has two files — keep these exact names:

- `palette.h` — the interface, already sketched. The enum, struct and
  prototypes must stay exactly as given; you implement `color_luma`
  here, *in the header*.
- `palette.c` — implement `palette_get` and `palette_brightest` here.

Do **not** define `main` — the grader compiles its own test program
(which includes `palette.h`) against your `palette.c`.

## The interface (fixed)

```c
enum color_id { PAL_BLACK, PAL_RED, PAL_GREEN, PAL_BLUE,
                PAL_YELLOW, PAL_WHITE, PAL_COUNT };

struct color {
    unsigned char r, g, b;
    const char *name;
};

static inline int color_luma(struct color c);
struct color palette_get(int id);
int palette_brightest(void);
```

The grader's test program itself contains
`_Static_assert(PAL_COUNT == 6, ...)` — the enum shape is checked
before a single instruction runs.

## Required behaviour

`palette_get(id)` returns the entry for a valid id:

| id           | r, g, b       | name       |
|--------------|---------------|------------|
| `PAL_BLACK`  | 0, 0, 0       | `black`    |
| `PAL_RED`    | 255, 0, 0     | `red`      |
| `PAL_GREEN`  | 0, 255, 0     | `green`    |
| `PAL_BLUE`   | 0, 0, 255     | `blue`     |
| `PAL_YELLOW` | 255, 255, 0   | `yellow`   |
| `PAL_WHITE`  | 255, 255, 255 | `white`    |

For any out-of-range `id` (negative, or `>= PAL_COUNT`) it returns
`{0, 0, 0}` with the name `unknown`. Names are exact, lowercase,
no trailing whitespace.

`color_luma(c)` is the integer luma `(299*c.r + 587*c.g + 114*c.b) /
1000` (plain integer division; white gives 255, black 0). It works for
*any* `struct color`, not just palette entries.

`palette_brightest()` returns the id of the palette color with the
highest luma (ties: the lowest id — with this table it is
unambiguous).

## The idioms to practise

- Store the palette as one file-scope `static const` array in
  `palette.c`, indexed by the enum with designated initializers
  (`[PAL_RED] = {255, 0, 0, "red"}, ...`) so reordering the enum could
  never silently misfile an entry.
- Add your own `_Static_assert` in `palette.c` that the table size
  equals `PAL_COUNT`.
- `color_luma` must be defined in `palette.h`. A plain function
  defined in a header would be defined in every file that includes it
  and the link would fail with duplicate symbols; `static inline`
  gives each includer its own private copy, so it links — that is why
  small header helpers are written this way.

The grader checks behaviour (it cannot see your initializers), then
repeats every check under AddressSanitizer/UBSan.

Grade with `ctrain check palette-modern`.
