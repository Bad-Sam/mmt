
#include "mem.h"
#include "allocators/mem_pool.h"
#include "allocators/mem_arena.h"
#include "allocators/mem_stack.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} vec4;


s64 get_perf_counter()
{
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    return counter.QuadPart;
}

void print_mem_pool(mem_pool* pool)
{
    static const char* status[2] = {"used", "free"};

    for (u32 i = 0u; i < pool->element_count; i++)
    {
        u32 bit_array_idx = i / 32u;
        u32 status_idx    = (pool->bit_array[bit_array_idx] & (1 << i)) > 0u;

        u8* pool_mem_ptr  = pool->element_array + (i * pool->element_size);

        fprintf(stderr, "\nSlot #%d (0x%X): %s", i, *(u32*)&pool_mem_ptr, status[status_idx]);
    }
}


void print_mem_arena(mem_arena* arena)
{
    fprintf(stderr, "\narena.mem = 0x%zX", (size_t)arena->mem);
    fprintf(stderr, "\narena.size = %u", arena->size);
    fprintf(stderr, "\narena.top_idx = %u\n", arena->top_idx);
}


void test_mem_arena()
{
#define ALLOC_SIZE (MMT_MEM_PAGE_SIZE * 100u)

    mem_arena arena;
    u32 init_success = mem_arena_init(&arena, ALLOC_SIZE);
    fprintf(stderr, "\ninit_success = mem_arena_init(&arena, %u)", ALLOC_SIZE);
    fprintf(stderr, "\ninit_success = %u", init_success);
    print_mem_arena(&arena);

    void* alloc0 = mem_arena_alloc(&arena, MMT_MEM_PAGE_SIZE >> 1);
    fprintf(stderr, "\nmem_arena_alloc(&arena, %u)", MMT_MEM_PAGE_SIZE >> 1);
    fprintf(stderr, "\nalloc0 is 0x%zX", (size_t)alloc0);
    fprintf(stderr, "\narena.top_idx = %u\n", arena.top_idx);

    void* alloc1 = mem_arena_alloc(&arena, ALLOC_SIZE - MMT_MEM_PAGE_SIZE);
    fprintf(stderr, "\nmem_arena_alloc(&arena, %u)", ALLOC_SIZE - MMT_MEM_PAGE_SIZE);
    fprintf(stderr, "\nalloc1 is 0x%zX", (size_t)alloc1);
    fprintf(stderr, "\narena.top_idx = %u\n", arena.top_idx);

    void* alloc2 = mem_arena_alloc(&arena, MMT_MEM_PAGE_SIZE);
    fprintf(stderr, "\nmem_arena_alloc(&arena, %u)", MMT_MEM_PAGE_SIZE);
    fprintf(stderr, "\nalloc2 is 0x%zX", (size_t)alloc2);
    fprintf(stderr, "\narena.top_idx = %u\n", arena.top_idx);

    void* alloc3 = mem_arena_alloc(&arena, MMT_MEM_PAGE_SIZE >> 1);
    fprintf(stderr, "\nmem_arena_alloc(&arena, %u)", MMT_MEM_PAGE_SIZE >> 1);
    fprintf(stderr, "\nalloc3 is 0x%zX", (size_t)alloc3);
    fprintf(stderr, "\narena.top_idx = %u\n", arena.top_idx);

    void* alloc4 = mem_arena_alloc(&arena, MMT_MEM_PAGE_SIZE);
    fprintf(stderr, "\nmem_arena_alloc(&arena, %u)", MMT_MEM_PAGE_SIZE);
    fprintf(stderr, "\nalloc4 is 0x%zX", (size_t)alloc4);
    fprintf(stderr, "\narena.top_idx = %u\n", arena.top_idx);

    print_mem_arena(&arena);

    void* alloc5 = mem_arena_alloc(&arena, MMT_MEM_PAGE_SIZE);
    fprintf(stderr, "\nmem_arena_alloc(&arena, %u)", MMT_MEM_PAGE_SIZE);
    fprintf(stderr, "\nalloc5 is 0x%zX", (size_t)alloc5);
    fprintf(stderr, "\narena.top_idx = %u\n", arena.top_idx);
    mem_arena_reset(&arena);
    fprintf(stderr, "\nmem_arena_reset(&arena)");
    fprintf(stderr, "\narena.top_idx = %u", arena.top_idx);

    mem_arena_destroy(&arena);
#undef ALLOC_SIZE
}

void test_mem_stack()
{
    mem_stack stack;

    mem_stack_init(&stack, MMT_MEM_PAGE_SIZE);
    
    fprintf(stderr, "\nmem_stack_alloc(&stack, %u) x10", MMT_MEM_PAGE_SIZE / 10u);
    for (u32 i = 0u; i < 10u ; i++)
    {
        mem_stack_alloc(&stack, MMT_MEM_PAGE_SIZE / 10u);
    }
    fprintf(stderr, "\nstack.top_idx = %u", stack.top_idx);

    mem_stack_free_to(&stack, MMT_MEM_PAGE_SIZE / 10u);
    fprintf(stderr, "\nmem_stack_free_to(&stack, %u)", MMT_MEM_PAGE_SIZE / 10u);
    fprintf(stderr, "\nstack.top_idx = %u", stack.top_idx);

    mem_stack_destroy(&stack);
}


void test_mem_pool()
{
    const u32 element_count     = 16u;
    const u32 element_size      = sizeof(vec4);
    const u32 element_alignment = 16u;

    mem_pool pool;

    mem_pool_init(&pool, element_count, element_size, element_alignment);

    const u32 alloc_size = element_count - 1u;
    fprintf(stderr, "\nAllocating %u vec4 (alignement %u):", alloc_size, element_alignment);
    for (u32 i = 0u; i < alloc_size; i++)
    {
        mem_pool_allocate(&pool);
    }
    print_mem_pool(&pool);

    fprintf(stderr, "\n\nFreeing %uth element:", alloc_size >> 1u);
    mem_pool_free(&pool, pool.element_array + sizeof(vec4) * (alloc_size >> 1u) - 1u);
    print_mem_pool(&pool);

    fprintf(stderr, "\n\nAllocating 1 element:");
    mem_pool_allocate(&pool);
    print_mem_pool(&pool);

    fprintf(stderr, "\n\nAllocating 1 more element:");
    mem_pool_allocate(&pool);
    print_mem_pool(&pool);

    fprintf(stderr, "\n\nAllocating 1 more element:");
    u8* alloc = mem_pool_allocate(&pool);
    fprintf(stderr, "\nReturned pointer is 0x%zX", (size_t)alloc);

    mem_pool_destroy(&pool);
}

int main()
{
    mem_init(MMT_MEM_PAGE_SIZE * 100u);
    
    mem_destroy();
    
    return 0;
}