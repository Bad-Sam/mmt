#ifndef MMT_BITS_H
#define MMT_BITS_H

#include <mmt/types.h>

#if defined(__has_builtin)
#   define mmt_has_builtin(builtin) __has_builtin(builtin)
#else
#   define mmt_has_builtin(builtin) 0
#endif

#if mmt_has_builtin(__builtin_ffs) == 0
#   include <immintrin.h>
#endif

inline s32 u32_as_s32(u32 a)
{
    return *(s32*)&a;
}


inline u32 u32_lowest_1_bit_idx(u32 bits)
{
#if mmt_has_builtin(__builtin_ffs)
    s32 bits_s32 = u32_as_s32(bits);

    return __builtin_ffs(bits_s32);
#else
    u32 index;
    _BitScanForward(&index, bits);

    return index;
#endif
}

#endif