#ifndef DEBUG_CHECKS_H
#define DEBUG_CHECKS_H

#include <stdio.h>
#if MMT_CLANG || MMT_GCC
#   define mmt_interrupt() do { __asm__("int3"); } while (0)
#elif MMT_MSVC
#   if defined(MMT_x64)
#       include <intrin.h>
#       define mmt_interrupt() do { __debugbreak(); } while (0)
#   elif defined(MMT_x86)
#       define mmt_interrupt() do { __asm { int3 }; } while (0)
#   endif
#else
#   include <stdlib.h>
#   define mmt_interrupt() do { exit(EXIT_FAILURE); } while (0)
#endif

#if MMT_ENABLE_DEBUG_CHECKS
#   define mmt_debug_assert(x)                                                        \
    do                                                                                \
    {                                                                                 \
        if ((x))                                                                      \
        {}                                                                            \
        else                                                                          \
        {                                                                             \
            fprintf(stderr, "\nAssertion failed in %s: %s returned false\n(%s:%i)\n", \
                    __func__, #x, __FILE__, __LINE__);                                \
            mmt_interrupt();                                                          \
        }                                                                             \
    }                                                                                 \
    while (0)
#else
#   define mmt_debug_assert(x)
#endif

#endif