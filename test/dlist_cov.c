#include "dlist.h"

/* dlist_init ***************************************************************/
zlx_np_t * dlist_init (zlx_np_t * ZLX_RESTRICT list_p)
{
    return zlx_dlist_init(list_p);
}

/* dlist_is_empty ***********************************************************/
int dlist_is_empty (zlx_np_t * ZLX_RESTRICT list_p)
{
    return zlx_dlist_is_empty(list_p);
}

/* dlist_insert *************************************************************/
void dlist_insert (zlx_np_t * anchor_p, zlx_np_t * to_ins_p, int dir)
{
    zlx_dlist_insert(anchor_p, to_ins_p, dir);
}

/* dlist_extend *************************************************************/
void dlist_extend (zlx_np_t * dest, zlx_np_t * src, int dir)
{
    zlx_dlist_extend(dest, src, dir);
}

/* dlist_delete *************************************************************/
void dlist_delete (zlx_np_t * to_del_p)
{
    zlx_dlist_delete(to_del_p);
}

/* int_dlist_append *********************************************************/
void int_dlist_append (zlx_np_t * list, int_dlist_entry_t * item)
{
    ZLX_DLIST_APPEND(*list, item, list_entry);
}

/* int_dlist_prepend ********************************************************/
void int_dlist_prepend (zlx_np_t * list, int_dlist_entry_t * item)
{
    ZLX_DLIST_PREPEND(*list, item, list_entry);
}

size_t int_dlist_to_array
(
    zlx_np_t * list,
    int * a,
    size_t n
)
{
    size_t m = 0;
    ZLX_DLIST_EACH(*list, e)
    {
        int_dlist_entry_t * ie =
            ZLX_STRUCT_FROM_FIELD(int_dlist_entry_t, list_entry, e);
        a[m++] = ie->v;
        if (m == n) break;
    }
    return m;
}


