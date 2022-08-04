#include "mem_arena.h"

#include <mmt/util/debug_checks.h>
#include <mmt/util/calc.h>

#if MMT_WINDOWS
#   include <windows.h>
#elif MMT_LINUX
#   include <sys/mman.h>
#else
#   include <stdlib.h>
#endif

u32 mem_arena_init(mem_arena* arena, u32 mem_size)
{
    mmt_debug_assert(arena != NULL);
    mmt_debug_assert(mem_size != 0u);

#if MMT_WINDOWS || MMT_LINUX
    mmt_debug_assert(u32_is_multiple(mem_size, MMT_MEM_PAGE_SIZE)
                     && "Use ceil_to_page_size() or make passed size a multiple of MMT_MEM_PAGE_SIZE");
#endif

#if MMT_WINDOWS
    arena->mem = VirtualAlloc(NULL, mem_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#elif MMT_LINUX
    arena->mem = mmap(NULL,
                      mem_size,
                      PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS,
                      -1,
                      0);
#else
    arena->mem = malloc(mem_size);
#endif

    arena->size    = mem_size;
    arena->top_idx = 0u;

    return (arena->mem != NULL);
}


void mem_arena_shrink(mem_arena* arena)
{
    mmt_debug_assert(arena != NULL);
    
    u32 new_size = ceil_to_page_size(arena->top_idx);

#if MMT_WINDOWS
    VirtualFree(arena->mem + new_size, 0, MEM_RELEASE);
#elif MMT_LINUX
    munmap(arena->mem + new_size, arena->size - new_size);
#else
    free(arena->mem + new_size);
#endif

    arena->size = new_size;
}


void mem_arena_destroy(mem_arena* arena)
{
    mmt_debug_assert(arena != NULL);
    
#if MMT_WINDOWS
    VirtualFree(arena->mem, 0, MEM_RELEASE);
#elif MMT_LINUX
    munmap(arena->mem, arena->size);
#else
    free(arena->mem);
#endif
}


u8* mem_arena_alloc(mem_arena* arena, u32 alloc_size)
{
    mmt_debug_assert(arena != NULL);
    mmt_debug_assert(alloc_size != 0u);

    u32 alloc_start_idx = arena->top_idx;
    
    arena->top_idx += alloc_size;

    mmt_debug_assert(arena->top_idx <= arena->size);

    return arena->mem + alloc_start_idx;
}


u8* mem_arena_alloc_aligned(mem_arena* arena, u32 alloc_size, u32 alignment)
{
    mmt_debug_assert(arena != NULL);
    mmt_debug_assert(alloc_size != 0u);
    mmt_debug_assert(alignment > 1u);

    u32 alloc_start_idx = u32_align(arena->top_idx, alignment);

    arena->top_idx = alloc_start_idx + alloc_size;

    mmt_debug_assert(arena->top_idx <= arena->size);

    return arena->mem + alloc_start_idx;
}


void mem_arena_reset(mem_arena* arena)
{
    mmt_debug_assert(arena != NULL);

    arena->top_idx = 0u;
}


u32 mem_arena_available_mem(mem_arena* arena)
{
    return arena->size - arena->top_idx;
}