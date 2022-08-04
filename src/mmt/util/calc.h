#ifndef MMT_CALC_H
#define MMT_CALC_H

#include "debug_checks.h"

#include <mmt/types.h>

#define ceil_to_page_size(n) ((n + MMT_MEM_PAGE_SIZE - 1u) & -MMT_MEM_PAGE_SIZE)

inline u32    u32_div_ceiled     (u32 a, u32 b);
inline u32    u32_is_multiple    (u32 n, u32 multiple);
inline u32    u32_align          (u32 n, u32 alignment_pow_of_2);
inline u32    u32_align_up       (u32 n, u32 alignment_pow_of_2);
inline u32    u32_is_pow_of_2    (u32 n);
inline size_t align_address      (size_t address, size_t alignment_pow_of_2);
inline size_t align_address_up   (size_t address, size_t alignment_pow_of_2);
inline u8*    align_ptr          (u8* ptr, size_t alignment_pow_of_2);
inline u8*    align_ptr_up       (u8* ptr, size_t alignment_pow_of_2);
inline u32    address_is_pow_of_2(size_t address);

#include "calc.inl"

#endif