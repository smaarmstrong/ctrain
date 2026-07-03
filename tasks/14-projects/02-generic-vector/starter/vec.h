#ifndef VEC_H
#define VEC_H

#include <stddef.h>

/* A growable array of fixed-size elements. Opaque: only vec.c knows the
 * layout. All element data is copied in and out by value. */
typedef struct vec vec;

vec   *vec_new(size_t elem_size);            /* NULL if elem_size==0 or OOM */
void   vec_free(vec *v);                     /* vec_free(NULL) is a no-op   */
size_t vec_len(const vec *v);

int    vec_push(vec *v, const void *elem);   /* 0 ok, -1 on OOM             */
void  *vec_get(const vec *v, size_t i);      /* NULL when i >= len          */
int    vec_pop(vec *v, void *out);           /* -1 when empty               */
int    vec_insert(vec *v, size_t i, const void *elem);  /* i <= len         */
int    vec_remove(vec *v, size_t i, void *out);         /* i <  len         */

#endif
