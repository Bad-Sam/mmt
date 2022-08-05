#ifndef DEBUG_CHECKS_H
#define DEBUG_CHECKS_H

#if MMT_ENABLE_DEBUG_CHECKS
#   include <stdio.h>

#   if defined(__has_builtin) && __has_builtin(__builtin_trap)
#       define mmt_debugger_breakpoint() do { __builtin_trap(); } while (0)
#   elif MMT_CLANG || MMT_GCC
#       define mmt_debugger_breakpoint() do { __asm__("int3"); } while (0)
#   elif MMT_MSVC
#      include <intrin.h>
#      define mmt_debugger_breakpoint() do { __debugbreak(); } while (0)
#   else
#      include <stdlib.h>
#      define mmt_debugger_breakpoint() do { abort(); } while (0)
#   endif

#if MMT_MSVC
#   define func_name __FUNCSIG__
#elif MMT_CLANG || MMT_GCC
#   define func_name __PRETTY_FUNCTION__
#else
#   define func_name __func__
#endif

#   define mmt_debug_check(x)                                       \
    do                                                              \
    {                                                               \
        if ((x))                                                    \
        {}                                                          \
        else                                                        \
        {                                                           \
            fprintf(stderr, "\n%s:%i: %s returned false in %s\n\n", \
                    __FILE__, __LINE__, #x, func_name);             \
            mmt_debugger_breakpoint();                              \
        }                                                           \
    }                                                               \
    while (0)
#else
#   define mmt_debugger_breakpoint() do { } while (0)
#   define mmt_debug_check(x)        do { } while (0)
#endif

#endif
