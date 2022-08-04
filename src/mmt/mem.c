#include "mem.h"

#include "allocators/mem_arena.h"

static mem_arena global_arena;

u32 mem_init(u32 mem_size)
{
    return mem_arena_init(&global_arena, mem_size);
}


void mem_shrink()
{
    mem_arena_shrink(&global_arena);
}


void mem_destroy()
{
    mem_arena_destroy(&global_arena);
}


u8* mem_alloc(u32 alloc_size)
{
    return mem_arena_alloc(&global_arena, alloc_size);
}


u8* mem_alloc_aligned(u32 alloc_size, u32 alignment)
{
    return mem_arena_alloc_aligned(&global_arena, alloc_size, alignment);
}


void mem_free(u8* mem)
{
    (void)mem;
}


void mem_free_aligned(u8* aligned_mem)
{
    (void)aligned_mem;
}


void mem_free_all()
{
    mem_arena_reset(&global_arena);
}


u32 mem_available_mem()
{
    return mem_arena_available_mem(&global_arena);
}
