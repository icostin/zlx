#ifndef _ZLX_DLIST_TEST_H
#define _ZLX_DLIST_TEST_H

#include "../include/zlx/dlist.h"

typedef struct int_dlist_entry int_dlist_entry_t;
struct int_dlist_entry
{
    zlx_np_t list_entry;
    int v;
};

zlx_np_t * dlist_init (zlx_np_t * ZLX_RESTRICT list_p);
int dlist_is_empty (zlx_np_t * ZLX_RESTRICT list_p);
void dlist_insert (zlx_np_t * anchor_p, zlx_np_t * to_ins_p, int dir);
void dlist_extend (zlx_np_t * dest, zlx_np_t * src, int dir);
void dlist_delete (zlx_np_t * to_del_p);

void int_dlist_append (zlx_np_t * list, int_dlist_entry_t * item);
void int_dlist_prepend (zlx_np_t * list, int_dlist_entry_t * item);
size_t int_dlist_to_array
(
    zlx_np_t * list,
    int * a,
    size_t n
);

#endif
