#include "mem_stack.h"

#include <mmt/mem.h>
#include <mmt/util/debug_checks.h>
#include <mmt/util/calc.h>

void mem_stack_init(mem_stack* stack, u32 stack_size)
{
    mmt_debug_assert(stack != NULL);

    stack->mem     = mem_alloc(stack_size);
    stack->size    = stack_size;
    stack->top_idx = 0u;
}


void mem_stack_destroy(mem_stack* stack)
{
    mmt_debug_assert(stack != NULL);

    mem_free(stack->mem);
}


u8* mem_stack_alloc(mem_stack* stack, u32 alloc_size)
{
    mmt_debug_assert(stack != NULL);
    mmt_debug_assert(alloc_size != 0u);
    mmt_debug_assert(stack->top_idx + alloc_size < stack->size);

    u8* alloc_start = stack->mem + stack->top_idx;
    stack->top_idx += alloc_size;

    return alloc_start;
}


u8* mem_stack_alloc_aligned(mem_stack* stack, u32 alloc_size, u32 alignment)
{
    mmt_debug_assert(stack != NULL);
    mmt_debug_assert(alloc_size != 0u);

    u32 alloc_start_idx = u32_align(stack->top_idx, alignment);
    
    stack->top_idx = alloc_start_idx + alloc_size;

    mmt_debug_assert(stack->top_idx < stack->size);

    return stack->mem + alloc_start_idx;
}


void mem_stack_free_to(mem_stack* stack, u32 idx)
{
    mmt_debug_assert(stack != NULL);
    mmt_debug_assert(idx < stack->top_idx);

    stack->top_idx = idx;
}


void mem_stack_clear(mem_stack* stack)
{
    mmt_debug_assert(stack != NULL);
    
    stack->top_idx = 0u;
}