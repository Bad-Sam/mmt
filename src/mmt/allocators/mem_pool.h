#ifndef MMT_MEM_POOL_H
#define MMT_MEM_POOL_H

#include <mmt/types.h>

// Manages a memory block populated with data type(s) of the same size.
// A bit array is maintained at the beginning of mem to keep track of
// freed and allocated memory parts
typedef struct
{
    union
    {
        u8*  mem;
        u32* bit_array;
    };
    
    u8*  element_array;
    u32  element_count;
    u32  element_size;
} mem_pool;

void mem_pool_init   (mem_pool* pool, u32 element_count, u32 element_size, u32 element_alignment);
void mem_pool_destroy(mem_pool* pool);

u8*  mem_pool_allocate      (mem_pool* pool);
void mem_pool_free          (mem_pool* pool, u8* elem);
void mem_pool_reset         (mem_pool* pool);
u32  mem_pool_bit_array_size(mem_pool* pool);

#endif