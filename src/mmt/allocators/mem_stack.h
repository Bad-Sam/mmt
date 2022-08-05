#ifndef MMT_MEM_STACK_H
#define MMT_MEM_STACK_H

#include <mmt/types.h>

// A block of memory from which allocations of any size can be performed.
// Memory can be released down to a location indicated by an index, in the same way a stack would.
// Said index can be copied from mem_stack.top_idx after an allocation for instance.
typedef struct
{
    u8* mem;
    u32 size;
    u32 top_idx;
} mem_stack;

void mem_stack_init         (mem_stack* stack, u32 stack_size);
void mem_stack_destroy      (mem_stack* stack);
u8*  mem_stack_alloc        (mem_stack* stack, u32 alloc_size);
u8*  mem_stack_alloc_aligned(mem_stack* stack, u32 alloc_size, u32 alignment);
void mem_stack_free_to      (mem_stack* stack, u32 idx);
void mem_stack_clear        (mem_stack* stack);

#endif