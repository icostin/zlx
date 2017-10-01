#ifndef _ZLX_MEM_ALLOC_H
#define _ZLX_MEM_ALLOC_H

#include "base.h"
#include "int.h"

/* zlx_ma_t *****************************************************************/
/**
 *  Memory allocator instance base structure.
 */
typedef struct zlx_ma_s zlx_ma_t;

/* zlx_realloc_func_t *******************************************************/
/**
 *  Function to allocate, reallocate or free a block of memory.
 *  @param old_ptr [in]
 *      pointer of allocated block of memory, or NULL for allocating a new block
 *  @param old_size [in]
 *      size of the already allocated block of memory, or 0 for allocating
 *      a new block
 *  @param new_size [in]
 *      new size requested for the block, or 0 for freeing the existent block
 *  @param ma [in, out]
 *      memory allocator instance
 *  @returns
 *      the new block or NULL if the block was freed or there was an error
 *      allocating the block
 */
typedef void * (ZLX_CALL * zlx_realloc_func_t)
    (
        void * old_ptr,
        size_t old_size,
        size_t new_size,
        zlx_ma_t * ZLX_RESTRICT ma
    );

struct zlx_ma_s
{
    /** Function to do the reallocation. */
    zlx_realloc_func_t realloc;

    /** Function to store information about an allocated block.
     *  This is intended to be used by memory tracker allocators to show
     *  some meaningful information to the programmer about memory leaks.
     *  A memory allocator that does not track the allocated blocks can
     *  simply use for this member the function zlx_ma_nop_info_set().
     */
    void (ZLX_CALL * info_set)
        (
            zlx_ma_t * ZLX_RESTRICT ma,
            void * ptr,
            char const * src,
            unsigned int line,
            char const * func,
            char const * info
        );

    /** Function that validates if the given block has been previously
     *  allocated by this allocator and not freed since.
     */
    void (ZLX_CALL * check)
        (
            zlx_ma_t * ZLX_RESTRICT ma,
            void * ptr,
            size_t size,
            char const * src,
            unsigned int line,
            char const * func
        );
};

/* zlxi_alloc ***************************************************************/
ZLX_INLINE void * zlxi_alloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    size_t size
#ifdef _DEBUG
    , char const * src
    , unsigned int line
    , char const * func
    , char const * info
#endif
)
{
    void * new_ptr;
    new_ptr = ma->realloc(NULL, 0, size, ma);
#ifdef _DEBUG
    if (new_ptr) ma->info_set(ma, new_ptr, src, line, func, info);
#endif
    return new_ptr;
}

/* zlxi_realloc *************************************************************/
ZLX_INLINE void * zlxi_realloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * old_ptr,
    size_t old_size,
    size_t new_size
#ifdef _DEBUG
    , char const * src
    , unsigned int line
    , char const * func
    , char const * info
#endif
)
{
    void * new_ptr;
#if defined(_DEBUG) || defined(_CHECKED)
    ma->check(ma, old_ptr, old_size,
#ifdef _DEBUG
              src, line, func
#else
              NULL, 0, NULL
#endif
              );
#endif
    new_ptr = ma->realloc(old_ptr, old_size, new_size, ma);
#ifdef _DEBUG
    if (new_ptr) ma->info_set(ma, new_ptr, src, line, func, info);
#endif
    return new_ptr;
}

/* zlxi_free ****************************************************************/
ZLX_INLINE void zlxi_free
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ptr,
    size_t size
#ifdef _DEBUG
    , char const * src
    , unsigned int line
    , char const * func
#endif
)
{
#if defined(_DEBUG) || defined(_CHECKED)
    ma->check(ma, ptr, size,
#ifdef _DEBUG
              src, line, func
#else
              NULL, 0, NULL
#endif
             );
#endif
    ma->realloc(ptr, size, 0, ma);
}

/* zlxi_array_alloc *********************************************************/
ZLX_INLINE int zlxi_array_alloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * * ZLX_RESTRICT ptr,
    size_t * ZLX_RESTRICT count,
    size_t new_count,
    size_t item_size
#ifdef _DEBUG
    , char const * src
    , unsigned int line
    , char const * func
    , char const * info
#endif
)
{
    void * new_ptr;
    if (new_count > (size_t) (PTRDIFF_MAX / item_size))
    {
        *ptr = NULL;
        *count = 0;
        return 1;
    }
    new_ptr = ma->realloc(NULL, 0, new_count * item_size, ma);
    if (new_ptr)
    {
#ifdef _DEBUG
        ma->info_set(ma, new_ptr, src, line, func, info);
#endif
        *ptr = new_ptr;
        *count = new_count;
    }
    else
    {
        *ptr = NULL;
        *count = 0;
    }

    return !new_ptr;
}

/* zlxi_array_realloc *******************************************************/
/**
 *  Resizes a block updating the pointer and size on success.
 *  @retval 0 resized ok
 *  @retval 1 failed
 */
ZLX_INLINE int zlxi_array_realloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * * ZLX_RESTRICT ptr,
    size_t * ZLX_RESTRICT count,
    size_t new_count,
    size_t item_size
#ifdef _DEBUG
    , char const * src
    , unsigned int line
    , char const * func
    , char const * info
#endif
)
{
    void * new_ptr;
#if defined(_DEBUG) || defined(_CHECKED)
    ma->check(ma, *ptr, *count * item_size,
# ifdef _DEBUG
              src, line, func
# else
              NULL, 0, NULL
# endif
              );
#endif
    if (new_count > PTRDIFF_MAX / item_size) return 1;
    new_ptr = ma->realloc(*ptr, *count * item_size, new_count * item_size, ma);
    if (new_ptr)
    {
#ifdef _DEBUG
        ma->info_set(ma, new_ptr, src, line, func, info);
#endif
        *ptr = new_ptr;
        *count = new_count;
    }
    return !new_ptr;
}

ZLX_API void * ZLX_CALL zlxnir_alloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    size_t size
);

ZLX_API void * ZLX_CALL zlxnic_alloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    size_t size
);

ZLX_API void * ZLX_CALL zlxnid_alloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    size_t size
    , char const * src
    , unsigned int line
    , char const * func
    , char const * info
);

ZLX_API void * ZLX_CALL zlxnir_realloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * old_ptr,
    size_t old_size,
    size_t new_size
);

ZLX_API void * ZLX_CALL zlxnic_realloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * old_ptr,
    size_t old_size,
    size_t new_size
);

ZLX_API void * ZLX_CALL zlxnid_realloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * old_ptr,
    size_t old_size,
    size_t new_size
    , char const * src
    , unsigned int line
    , char const * func
    , char const * info
);

ZLX_API void ZLX_CALL zlxnir_free
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ptr,
    size_t size
);

ZLX_API void ZLX_CALL zlxnic_free
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ptr,
    size_t size
);

ZLX_API void ZLX_CALL zlxnid_free
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ptr,
    size_t size
    , char const * src
    , unsigned int line
    , char const * func
);

ZLX_API int ZLX_CALL zlxnid_array_alloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * * ZLX_RESTRICT ptr,
    size_t * ZLX_RESTRICT count,
    size_t new_count,
    size_t item_size
    , char const * src
    , unsigned int line
    , char const * func
    , char const * info
);

ZLX_API int ZLX_CALL zlxnic_array_alloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * * ZLX_RESTRICT ptr,
    size_t * ZLX_RESTRICT count,
    size_t new_count,
    size_t item_size
);

ZLX_API int ZLX_CALL zlxnir_array_alloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * * ZLX_RESTRICT ptr,
    size_t * ZLX_RESTRICT count,
    size_t new_count,
    size_t item_size
);

ZLX_API int ZLX_CALL zlxnid_array_realloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * * ZLX_RESTRICT ptr,
    size_t * ZLX_RESTRICT count,
    size_t new_count,
    size_t item_size
    , char const * src
    , unsigned int line
    , char const * func
    , char const * info
);

ZLX_API int ZLX_CALL zlxnic_array_realloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * * ZLX_RESTRICT ptr,
    size_t * ZLX_RESTRICT count,
    size_t new_count,
    size_t item_size
);

ZLX_API int ZLX_CALL zlxnir_array_realloc
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * * ZLX_RESTRICT ptr,
    size_t * ZLX_RESTRICT count,
    size_t new_count,
    size_t item_size
);

#if defined(_DEBUG)
# define zlxni_alloc zlxnid_alloc
# define zlxni_realloc zlxnid_realloc
# define zlxni_free zlxnid_free
# define zlxni_array_alloc zlxnid_array_alloc
# define zlxni_array_realloc zlxnid_array_realloc
#elif defined(_CHECKED)
# define zlxni_alloc zlxnic_alloc
# define zlxni_realloc zlxnic_realloc
# define zlxni_free zlxnic_free
# define zlxni_array_alloc zlxnic_array_alloc
# define zlxni_array_realloc zlxnic_array_realloc
#else
# define zlxni_alloc zlxnir_alloc
# define zlxni_realloc zlxnir_realloc
# define zlxni_free zlxnir_free
# define zlxni_array_alloc zlxnir_array_alloc
# define zlxni_array_realloc zlxnir_array_realloc
#endif


#ifdef _DEBUG
#define zlx_alloc(_ma, _size, _info) \
    (_ZLXI_NAME(_alloc)((_ma), (_size), \
                        __FILE__, __LINE__, __FUNCTION__, (_info)))

#define zlx_realloc(_ma, _old_ptr, _old_size, _new_size) \
    (_ZLXI_NAME(_realloc)((_ma), (_old_ptr), (_old_size), (_new_size), \
                  __FILE__, __LINE__, __FUNCTION__, NULL))

#define zlx_free(_ma, _ptr, _size) \
    (_ZLXI_NAME(_free)((_ma), (_ptr), (_size), \
                       __FILE__, __LINE__, __FUNCTION__))

#define zlx_array_alloc(ma, array_var, count_var, count, info) \
    (_ZLXI_NAME(_array_alloc)(ma, (void * *) &(array_var), &(count_var), \
                              (count), sizeof((array_var)[0]), \
                              __FILE__, __LINE__, __FUNCTION__, (info)))

#define zlx_array_realloc(ma, array_var, count_var, count) \
    (_ZLXI_NAME(_array_realloc)(ma, (void * *) &(array_var), &(count_var), \
                                (count), sizeof((array_var)[0]), \
                                __FILE__, __LINE__, __FUNCTION__, NULL))

#else
#define zlx_alloc(_ma, _size, _info) \
    (_ZLXI_NAME(_alloc)((_ma), (_size)))

#define zlx_realloc(_ma, _old_ptr, _old_size, _new_size) \
    (_ZLXI_NAME(_realloc)((_ma), (_old_ptr), (_old_size), (_new_size)))

#define zlx_free(_ma, _ptr, _size) \
    (_ZLXI_NAME(_free)((_ma), (_ptr), (_size)))

#define zlx_array_alloc(ma, array_var, count_var, count, info) \
    (_ZLXI_NAME(_array_alloc)(ma, (void * *) &(array_var), &(count_var), \
                              (count), sizeof((array_var)[0])))

#define zlx_array_realloc(ma, array_var, count_var, count) \
    (_ZLXI_NAME(_array_realloc)(ma, (void * *) &(array_var), &(count_var), \
                                (count), sizeof((array_var)[0])))

#endif

/** @def zlx_array_init
 *  Initializes to (NULL, 0) the pair (pointer, size) representing an array.
 */
#define zlx_array_init(array_var, count_var) \
    ((array_var) = NULL, (count_var) = 0)

/** @def zlx_array_free
 *  Macro to free an array of items.
 */
#define zlx_array_free(_ma, _arr, _count) \
    (zlx_free((_ma), (_arr), (_count) * sizeof(*(_arr))))



ZLX_API void * ZLX_CALL zlx_ma_nop_realloc
(
    void * old_ptr,
    size_t old_size,
    size_t new_size,
    zlx_ma_t * ZLX_RESTRICT ma
);

ZLX_API void ZLX_CALL zlx_ma_nop_info_set
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ZLX_RESTRICT ptr,
    char const * ZLX_RESTRICT src,
    unsigned int line,
    char const * ZLX_RESTRICT func,
    char const * ZLX_RESTRICT info
);

ZLX_API void ZLX_CALL zlx_ma_nop_check
(
    zlx_ma_t * ZLX_RESTRICT ma,
    void * ZLX_RESTRICT ptr,
    size_t size,
    char const * ZLX_RESTRICT src,
    unsigned int line,
    char const * ZLX_RESTRICT func
);


#endif /* _ZLX_MEM_ALLOC_H */

