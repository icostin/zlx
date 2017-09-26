#include "base.h"
#include "memalloc.h"

/** @defgroup array Array generator
 *  Defines and implements arrays of arbitrary types based on other macros
 *  to do operations on elements.
 *
 *  The typical use of this component:
 *  - in a header file, define macros that describe the item and help 
 *    declare generated functions then include "zlx/array.h" to generate 
 *    the header declarations
 *  - in a C source file define the same macros as above, define ZLX_BODY
 *    and include again "zlx/array.h" to generate function bodies.
 *
 *  Macros that affect the generated source:
 *  - T: item type
 *  - F(n): function name decorator
 *  - CMP: macro to compare two items, returning 0 for equal, -1 or +1 for
 *    less or more
 *  - EQU: macro that compares two items returning non-zero if items are equal
 *    and 0 if not
 *  - COPY: macro that copies one item
 *  - FDP: function decorator prefix
 *  - FDS: function decorator suffix
 *  - ZERO: macro representing a "zero" item
 *
 *  @{ 
 *
 *  @fn void FOO_set (TYPE * ZLX_RESTRICT arr, size_t n, TYPE val)
 *  Sets the elements of the array.
 *
 *  @fn void FOO_copy (TYPE * ZLX_RESTRICT dest, TYPE_ const ZLX_RESTRICT src, size_t n)
 *  Copies elements of an array to another. The two arrays must be disjunct.
 **/

#ifdef ZLX_DOXYGEN

/** @def T
 *  Array item type name.
 *  This macro must be defined before including array.h to determine the type
 *  of array items for which we generate code.
 */
#define T foo_t

/** @def F(fname)
 *  Function name generator.
 *  Before including array.h defining this macro will tell <zlx/array.h> to 
 *  declare function names according to the expansion of this macro.
 *  For instance, including <zlx/array.h> will cause F(set), F(cmp), etc to be 
 *  defined.
 *  @note
 *      If the macro is not defined it will use <b>T</b>_array_<b>fname</b> for
 *      generated function names.
 */
#define F(fname) foo_##fname

#endif

#ifndef T
#error T must be defined to represent the item type
#endif

#ifndef F
#define F(_n) (ZLX_TP2D(ZLX_TP2B(T, _array_), _n))
#endif

#ifndef CMP
/** @def CMP(a, b)
 *  Item comparator.
 *  Define this macro before including <zlx/array.h> to control how items
 *  are compared.
 *  This must expand to an expression that evaluates to an @b int where
 *  0 means equal, negative means @a a less than @a b, and positive means
 *  @a greater than @a b.
 *  @note
 *      The code that calls this macro is guaranteed to not use expressions
 *      with side-effects as macro arguments so the user can safely
 *      define the macro to expand the arguments multiple times.
 *  @note
 *      If this macro is not defined it will become 
 *      <tt>a == b ? 0 : (a < b ? -1 : +1)</tt>
 */
#define CMP(a, b) a == b ? 0 : (a < b ? -1 : +1)
#endif

#ifndef EQU
/** @def EQU(a, b)
 *  Tests for equality.
 *  Define this macro before including <zlx/array.h> to control how items
 *  are tested for equality.
 *  @note
 *      The code that calls this macro is guaranteed to not use expressions
 *      with side-effects as macro arguments so the user can safely
 *      define the macro to expand the arguments multiple times.
 *  @note
 *      If this macro is not defined it will become 
 *      <tt>a == b</tt>
 */
#define EQU(a, b) a == b
#endif

#ifndef COPY
/** @def COPY(dest, src)
 *  Copies one item.
 *  Define this macro before including <zlx/array.h> to control how items
 *  are copied.
 *  @note
 *      If this macro is not defined it will become 
 *      <tt>(dest) = (src)</tt>
 */
#define COPY(dest, src) dest = src
#endif

#ifndef FDP
/** @def FDP
 *  Function declaration prefix.
 *  Define this macro before including <zlx/array.h> to control the prefix
 *  of generated functions.
 *  This can be used for instance to declare all functions static or to export
 *  them from a DLL.
 */
#define FDP
#endif

#ifndef FDS
/** @def FDS
 *  Function declaration suffix.
 *  Define this macro before including <zlx/array.h> to control the suffix
 *  of generated functions.
 *  This can be useful for specifying compiler-specific attributes to generated
 *  functions.
 */
#define FDS
#endif

#ifndef ZERO
/** @def ZERO
 *  Zero item.
 *  Define this macro before including <zlx/array.h> to specify the item 
 *  representing value zero.
 *  This value affects functions like foo_zcmp(), foo_zcopy().
 */
#define ZERO ((T) 0)
#endif

/**
 *  Sets all array items to a specific value
 *  @note The function does not return a value.
 */
FDP void F(set) (T * ZLX_RESTRICT arr, size_t n, T val) FDS;

/**
 *  Copies array items to a non-overlapping location.
 *  @note The function does not return a value.
 */
FDP void F(copy) (T * ZLX_RESTRICT a, T const * ZLX_RESTRICT b, size_t n) FDS;

/**
 *  Copies a zero-terminated array of items.
 *  @returns the location of the zero terminator in the destination array.
 */
FDP T * F(zcopy) (T * ZLX_RESTRICT a, T const * ZLX_RESTRICT b) FDS;

/**
 *  Compares items of two arrays.
 *  @returns 0 for equal, negative for @a a less than @a b, positive for @a a
 *  greater than @a b
 */
FDP int F(cmp) (T const * a, T const * b, size_t n) FDS;

/**
 *  Scans an array until it finds the given value.
 *  @returns a pointer to where the value was found.
 *  @warning
 *      This function will keep on scanning until it finds the given value or
 *      it will generate a page fault when reaches unmapped memory.
 */
FDP T * F(scan) (T const * arr, T value) FDS;

/**
 *  Searches for a value in an array.
 *  @returns 
 *      the address where the search has stopped because it found the value
 *      or it reached the end
 */
FDP T * F(search) (T const * a, T const * end, T value) FDS;

/**
 *  Inserts a value in a given array.
 *  This function attempts to shift values after the insertion point, 
 *  reallocating the buffer if necessary.
 */
FDP T * F(insert)
(
    T * ZLX_RESTRICT * ZLX_RESTRICT ap,
    size_t * ZLX_RESTRICT np,
    size_t * ZLX_RESTRICT mp,
    size_t p,
    size_t q,
    zlx_ma_t * ma
) FDS;

/**
 *  Computes the length of the zero-terminated array.
 */
FDP size_t F(zlen) (T const * ZLX_RESTRICT a) FDS;
/**
 *  Compares to zero-terminated arrays.
 */
FDP int F(zcmp) (T const * a, T const * b) FDS;


#ifdef ZLX_BODY

FDP void F(copy) (T * ZLX_RESTRICT a, T const * ZLX_RESTRICT b, size_t n) FDS
{
    size_t i;
    for (i = 0; i < n; ++i)
    {
        COPY(a[i], b[i]);
    }
}

FDP T * F(zcopy) (T * ZLX_RESTRICT a, T const * ZLX_RESTRICT b) FDS
{
    size_t i;
    for (i = 0;; ++i)
    {
        COPY(a[i], b[i]);
        if (EQU(b[i], ZERO)) break;
    }
    return a + i;
}

FDP void F(set) (T * arr, size_t n, T val) FDS
{
    size_t i;
    for (i = 0; i < n; ++i) arr[i] = val;
}

FDP int F(cmp) (T const * a, T const * b, size_t n) FDS
{
    size_t i;
    for (i = 0; i < n; ++i)
    {
        int c = CMP(a[i], b[i]);
        if (c) return c;
    }
    return 0;
}

FDP T * F(scan) (T const * arr, T value) FDS
{
    while (!EQU(*arr, value)) ++arr;
    return (T *) arr;
}

FDP size_t F(zlen) (T const * ZLX_RESTRICT a) FDS
{
    return (size_t) (F(scan)(a, ZERO) - a);
}

FDP T * F(search) (T const * a, T const * end, T value) FDS
{
    while (a != end && !EQU(*a, value)) ++a;
    return (T *) a;
}

FDP T * F(insert)
(
    T * ZLX_RESTRICT * ZLX_RESTRICT ap,
    size_t * ZLX_RESTRICT np,
    size_t * ZLX_RESTRICT mp,
    size_t p,
    size_t q,
    zlx_ma_t * ZLX_RESTRICT ma
) FDS
{
    T * a;
    size_t n, m, i;
    n = *np + q;
    if (n < q) return NULL;
    if (n > (SIZE_MAX / 2 + 1) / sizeof(T)) return NULL;
    if (n > *mp)
    {
        m = ((size_t) 1 << zlx_size_log2_ceil(n * sizeof(T))) / sizeof(T);
        a = zlx_realloc(ma, *ap, *mp * sizeof(T), m * sizeof(T));
        if (!a) return NULL;
        *ap = a;
        *mp = m;
    }
    else a = *ap;
    for (i = *np; i > p; )
    {
        --i;
        COPY(a[i + q], a[i]);
    }
    *np = n;
    return a + p;
}

FDP int F(zcmp) (T const * a, T const * b) FDS
{
    for (;; ++a, ++b)
    {
        int c;
        c = CMP(*a, *b);
        if (c) return c;
        if (EQU(*a, ZERO)) return 0;
    }
}

#endif /* ZLX_BODY */

#undef FDP
#undef FDS
#undef CMP
#undef EQU
#undef COPY
#undef ZERO
#undef F
#undef T

/** @} */

