#ifndef MMT_TYPES_H
#define MMT_TYPES_H

#if MMT_MSVC
    typedef          __int8  s8;
    typedef          __int16 s16;
    typedef          __int32 s32;
    typedef          __int64 s64;
    typedef unsigned __int8  u8;
    typedef unsigned __int16 u16;
    typedef unsigned __int32 u32;
    typedef unsigned __int64 u64;
#else
#   include <stdint.h>
    typedef int8_t   s8;
    typedef int16_t  s16;
    typedef int32_t  s32;
    typedef int64_t  s64;
    typedef uint8_t  u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;
#endif

#if MMT_x64
    typedef u64 size_t;
#elif MMT_x86
    typedef u32 size_t;
#endif

typedef float f32;

#endif