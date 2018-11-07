#ifndef UTIL_LTSMIN_H
#define UTIL_LTSMIN_H

#include <stdbool.h>
#include <stdint.h>

#include <util/util.h>


typedef struct list_s
{
    int count;
    int data[];
} list_t;

extern list_t *list_create (size_t size);

extern void list_free (list_t *list);

extern void list_print (list_t *list);

extern void list_debug (list_t *list);

extern void list_sort (list_t *list);

static inline int
list_get (list_t *list, int index)
{
    return list->data[index];
}

/* begin iterator */
static inline int *
list_begin (list_t *list)
{
    return &list->data[0];
}

static inline int *
list_end (list_t *list)
{
    return &list->data[list->count];
}
/* end iterator */

static inline int
list_top (list_t *list)
{
    Assert(list->count >= 0);
    return list->data[list->count - 1];
}

static inline int
list_pop (list_t *list)
{
    Assert(list->count >= 0);
    return list->data[--list->count];
}

static inline int
list_find (list_t *list, int e)
{
    for (int *a = list_begin(list); a != list_end(list); a++)
         if (*a == e) return a - list_begin(list);
    return -1;
}

static inline int
list_count (list_t *list)
{
    return list->count;
}

static inline void
list_clear (list_t *list)
{
    list->count = 0;
}

static inline void
list_add (list_t *list, int num)
{
    list->data[list->count++] = num;
}

static inline int
list_binary_search (list_t *list, int key)
{
    int             imin = 0;
    int             imax = list->count - 1;
    while (imax >= imin) {
        int imid = imin + ((imax - imin) >> 1);
        if (list->data[imid] == key) {
            return imid;
        } else if (list->data[imid] < key) {
            imin = imid + 1;
        } else {
            imax = imid - 1;
        }
    }
    return -1;
}

static inline void
list_add_if (list_t *list, int num, int condition)
{
    list->data[list->count] = num;
    list->count += condition != 0;
}

static inline void
list_invert (list_t *list)
{
    for (int i = 0; i < list->count / 2; i++) {
        swap (list->data[i], list->data[list->count - i - 1]);
    }
}

#endif // UTIL_LTSMIN_H

