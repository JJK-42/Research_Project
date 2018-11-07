#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include <util/list.h>
#include <util/runtime.h>


list_t *
list_create (size_t size)
{
    return RTmallocZero (sizeof(int[size + 1]));
}

void
list_free (list_t *list)
{
    RTfree (list);
}

static inline void
list_print_int (list_t *list)
{
    if (list->count > 0)
        Print ("%d", list_get(list, 0));
    for (size_t i = 1; i < list->count; i++) {
        Print (", %d", list_get(list, i));
    }
}

void
list_print (list_t *list)
{
    list_print_int (list);
}

static int
compint (const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

void
list_sort (list_t *list)
{
    qsort (list->data, list->count, sizeof(int), compint);
}
