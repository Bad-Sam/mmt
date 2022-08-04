#ifndef MMT_MEM_STACK_H
#define MMT_MEM_STACK_H

#include <mmt/types.h>

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