#include "mem_pool.h"

#include <mmt/mem.h>
#include <mmt/util/bits.h>
#include <mmt/util/calc.h>
#include <mmt/util/debug_checks.h>

#define U32_BIT_COUNT 32u

void mem_pool_init(mem_pool* pool, u32 element_count, u32 element_size, u32 element_alignment)
{
    mmt_debug_check(pool != NULL);
    mmt_debug_check(element_count != 0u);
    mmt_debug_check(element_size != 0u);

    // Same as u32_div_ceiled(element_count, U32_BIT_COUNT), simplified
    u32 bit_array_u32_count = (element_count + 31u) / U32_BIT_COUNT;
    u32 bit_array_size      = bit_array_u32_count * 4u;
    u32 max_padding_size    = element_alignment - 1u;
    u32 all_element_size    = element_count * element_size;

    pool->mem           = mem_alloc(bit_array_size + max_padding_size + all_element_size);
    pool->element_array = align_ptr(pool->mem + bit_array_size, element_alignment);
    pool->element_count = element_count;
    pool->element_size  = element_size;

    for (u32 i = 0u; i < bit_array_u32_count; i++)
    {
        pool->bit_array[i] = 0xFFFFFFFF;
    }
}


void mem_pool_destroy(mem_pool* pool)
{
    mem_free(pool->mem);
}


u8* mem_pool_allocate(mem_pool* pool)
{
    mmt_debug_check(pool != NULL);

    u32 bit_array_size = mem_pool_bit_array_size(pool);
    u32 bit_array_idx  = 0u;
    u32 bit_idx        = 0u;

    while (bit_array_idx < bit_array_size)
    {
        bit_idx = u32_lowest_1_bit_idx(pool->bit_array[bit_array_idx]);

        if (bit_idx)
        {
            break;
        }

        bit_array_idx++;
    }

    // Check that we are not looking at a bit representing an object beyond range
    u32 bit_number = bit_array_idx * U32_BIT_COUNT + bit_idx;

    if (bit_idx && (bit_number <= pool->element_count))
    {
        bit_idx--; // The index returned by u32_lowest_1_bit_idx() is 1-based
        pool->bit_array[bit_array_idx] &= ~(1 << bit_idx);

        return pool->element_array + pool->element_size * bit_idx;
    }
    
    return NULL;
}


void mem_pool_free(mem_pool* pool, u8* elem)
{
    mmt_debug_check(pool != NULL);
    mmt_debug_check(elem != NULL);
    mmt_debug_check(pool->element_array <= elem);
    mmt_debug_check(elem < pool->element_array + pool->element_count * pool->element_size);

    u32 offset        = elem - pool->element_array;
    u32 elem_idx      = offset / pool->element_size;
    u32 bit_array_idx = elem_idx / U32_BIT_COUNT;
    u32 bit_idx       = elem_idx & (U32_BIT_COUNT - 1u); // same as elem_idx % U32_BIT_COUNT

    pool->bit_array[bit_array_idx] |= (1 << bit_idx);
}


void mem_pool_reset(mem_pool* pool)
{
    mmt_debug_check(pool != NULL);

    u32 bit_array_size = mem_pool_bit_array_size(pool);
    for (u32 i = 0u; i < bit_array_size; i++)
    {
        pool->bit_array[i] = 0xFFFFFFFF;
    }
}


u32 mem_pool_bit_array_size(mem_pool* pool)
{
    mmt_debug_check(pool != NULL);
    
    return (pool->element_count + 31u) / U32_BIT_COUNT;
}