#ifndef MMT_MEM_H
#define MMT_MEM_H

#include "types.h"

// Wrapper around a global mem_arena
u32  mem_init   (u32 mem_size);
void mem_shrink ();
void mem_destroy();

u8*  mem_alloc        (u32 alloc_size);
u8*  mem_alloc_aligned(u32 alloc_size, u32 alignment);
void mem_free         (u8* mem);
void mem_free_aligned (u8* aligned_mem);
void mem_free_all     ();

u32 mem_available_mem();

#endif
