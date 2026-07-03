# vec — a generic growable array

Implement the small container library declared in `vec.h` (already in your
work directory — do not change the declarations). A `vec` stores elements
of one fixed byte size, chosen when the vector is created, so the same
code can hold `int`s, `double`s or whole structs. Elements are always
**copied in and out by value** (`memcpy`-style); the vector owns its own
storage.

Write the implementation in `vec.c` (you may add more `.c` files; every
`.c` file in your work directory is compiled together). **Do not define
`main`** — the grader links its own test harness.

## The API

```c
typedef struct vec vec;          /* opaque: callers never see the fields */

vec   *vec_new(size_t elem_size);
void   vec_free(vec *v);
size_t vec_len(const vec *v);
int    vec_push(vec *v, const void *elem);
void  *vec_get(const vec *v, size_t i);
int    vec_pop(vec *v, void *out);
int    vec_insert(vec *v, size_t i, const void *elem);
int    vec_remove(vec *v, size_t i, void *out);
```

Every function that can fail returns `0` on success and `-1` on failure;
none of them ever crash on the failure cases listed here.

- `vec_new(elem_size)` — a new empty vector whose elements are
  `elem_size` bytes each. Returns `NULL` if `elem_size` is `0` or memory
  runs out.
- `vec_free(v)` — release the vector and everything it owns.
  `vec_free(NULL)` is a harmless no-op.
- `vec_len(v)` — the current number of elements.
- `vec_push(v, elem)` — copy `elem_size` bytes from `elem` onto the end,
  growing the storage as needed. Growth must be amortised: pushing tens
  of thousands of elements has to stay well under a second.
- `vec_get(v, i)` — a pointer to element `i` **inside the vector's own
  storage** (so writing through it changes the stored element), or
  `NULL` when `i >= vec_len(v)`. The pointer is only guaranteed valid
  until the next call that can reallocate or shift (push/pop/insert/
  remove).
- `vec_pop(v, out)` — remove the last element; if `out` is non-NULL,
  copy the removed element into it first. `-1` if the vector is empty.
- `vec_insert(v, i, elem)` — insert a copy at index `i`, shifting
  elements `i..len-1` up by one. Any `i` from `0` to `len` inclusive is
  valid (`i == len` appends); larger `i` returns `-1` and changes
  nothing.
- `vec_remove(v, i, out)` — remove element `i`, copying it into `out`
  first when `out` is non-NULL, and shift the tail down. `-1` when
  `i >= len`.

## Quality bar

The grader compiles your files together with its harness and also runs
the whole suite under AddressSanitizer/UBSan: no leaks (a fully used and
freed vector must leave nothing behind), no out-of-bounds reads or
writes, no undefined behaviour. Failure paths (`vec_new(0)`, popping an
empty vector, out-of-range indexes) must fail cleanly with the return
values above.

Grade with `ctrain check generic-vector`.
