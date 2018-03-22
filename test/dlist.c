#include <string.h>
#include "test.h"
#include "dlist.h"

#define A() memset(a, -1, sizeof(a))

int dlist_test (void)
{
    zlx_np_t l, k;
    int_dlist_entry_t e[10];
    int a[10];
    size_t n;

    T(dlist_init(&l) == &l);
    T(dlist_is_empty(&l));
    T(l.next == &l);
    T(l.prev == &l);

    T(dlist_init(&k) == &k);
    T(dlist_is_empty(&k));

    e[0].v = 5;
    dlist_insert(&l, &e[0].list_entry, ZLX_NEXT);
    n = int_dlist_to_array(&l, memset(a, -1, sizeof(a)), 10);
    T(n == 1); T(a[0] == 5);

    e[1].v = 2;
    dlist_insert(&e[0].list_entry, &e[1].list_entry, ZLX_PREV);
    n = int_dlist_to_array(&l, memset(a, -1, sizeof(a)), 10);
    T(n == 2); T(a[0] == 2); T(a[1] == 5);

    e[2].v = 4;
    int_dlist_append(&k, &e[2]);
    e[3].v = 1;
    int_dlist_prepend(&k, &e[3]);
    n = int_dlist_to_array(&k, memset(a, -1, sizeof(a)), 1);
    T(n == 1); T(a[0] == 1);
    n = int_dlist_to_array(&k, memset(a, -1, sizeof(a)), 10);
    T(n == 2); T(a[0] == 1); T(a[1] == 4);

    dlist_extend(&l, &k, ZLX_NEXT);
    n = int_dlist_to_array(&l, memset(a, -1, sizeof(a)), 10);
    TE(n == 4, "n=%u", (int) n); 
    T(a[0] == 2); T(a[1] == 5); T(a[2] == 1); T(a[3] == 4);

    dlist_delete(&e[0].list_entry);
    n = int_dlist_to_array(&l, memset(a, -1, sizeof(a)), 10);
    TE(n == 3, "n=%u", (int) n); 
    T(a[0] == 2); T(a[1] == 1); T(a[2] == 4);


    return 0;
}

