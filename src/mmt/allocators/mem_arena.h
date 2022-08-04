#ifndef MMT_MEM_ARENA_H
#define MMT_MEM_ARENA_H

#include <mmt/types.h>

// A chunk of pre-allocated memory, from which memory can only be requested, not given back
// Users can manage themselves the requested memory to repurpose it
// An index to the end of the last allocated region (which is always the highest) is maintained
typedef struct
{
    u8* mem;
    u32 size;
    u32 top_idx;
} mem_arena;

u32  mem_arena_init         (mem_arena* arena, u32 mem_size);
void mem_arena_shrink       (mem_arena* arena);
void mem_arena_destroy      (mem_arena* arena);

u8*  mem_arena_alloc        (mem_arena* arena, u32 alloc_size);
u8*  mem_arena_alloc_aligned(mem_arena* arena, u32 alloc_size, u32 alignment);
void mem_arena_reset        (mem_arena* arena);

u32  mem_arena_available_mem(mem_arena* arena);

#endif
