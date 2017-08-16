#ifndef _ZLX_ARCH_H
#define _ZLX_ARCH_H

#if !defined(ZLX_IA32) && (defined(_M_IX86) || defined(_M_I86) || defined(__X86__) || defined(_X86_) || defined(__i386__) || defined(__i386))
# define ZLX_IA32 1
#endif

#if !defined(ZLX_AMD64) && (defined(_M_X64) || defined(_M_AMD64) || defined(__amd64__) || defined(__x86_64))
# define ZLX_AMD64 1
#endif


#ifndef ZLX_IA32
# define ZLX_IA32 0
#endif

#ifndef ZLX_AMD64
# define ZLX_AMD64 0
#endif


#if !defined(ZLX_LITTLE_ENDIAN) && (ZLX_IA32 || ZLX_AMD64)
# define ZLX_LITTLE_ENDIAN 1
#endif

#ifndef ZLX_LITTLE_ENDIAN
# define ZLX_LITTLE_ENDIAN 0
#endif

#ifndef ZLX_BIG_ENDIAN
# define ZLX_BIG_ENDIAN 0
#endif



#ifndef ZLX_UNALIGNED_ACCESS
/* autodetect if unaligned accesses are allowed */
# if ZLX_IA32 || ZLX_AMD64
#  define ZLX_UNALIGNED_ACCESS 1
# else
#  define ZLX_UNALIGNED_ACCESS 0
# endif
#endif

#endif /* _ZLX_ARCH_H */

