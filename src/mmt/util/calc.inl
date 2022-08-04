inline u32 u32_div_ceiled(u32 a, u32 b)
{
    return (a + b - 1u) / b;
}

inline u32 u32_is_multiple(u32 n, u32 multiple)
{
    return n % multiple == 0u;
}


inline u32 u32_align(u32 n, u32 alignment_pow_of_2)
{
    mmt_debug_assert(alignment_pow_of_2 > 1u);
    mmt_debug_assert(u32_is_pow_of_2(alignment_pow_of_2));

    u32 mask = alignment_pow_of_2 - 1u;

    return (n + mask) & ~mask;
}


inline u32 u32_align_up(u32 n, u32 alignment_pow_of_2)
{
    mmt_debug_assert(alignment_pow_of_2 > 1u);
    mmt_debug_assert(u32_is_pow_of_2(alignment_pow_of_2));

    u32 modulo = n & (alignment_pow_of_2 - 1u);

    return n + alignment_pow_of_2 - modulo;
}


inline u32 u32_is_pow_of_2(u32 n)
{
    return (n & (n - 1u)) == 0u;
}


inline size_t align_address(size_t address, size_t alignment_pow_of_2)
{
    mmt_debug_assert(alignment_pow_of_2 > 1u);
    mmt_debug_assert(address_is_pow_of_2(alignment_pow_of_2));

    size_t mask = alignment_pow_of_2 - 1u;

    return (address + mask) & ~mask;
}


inline size_t align_address_up(size_t address, size_t alignment_pow_of_2)
{
    mmt_debug_assert(alignment_pow_of_2 > 1u);
    mmt_debug_assert(address_is_pow_of_2(alignment_pow_of_2));

    size_t modulo = address & (alignment_pow_of_2 - 1u);

    return address + alignment_pow_of_2 - modulo;
}

inline u8* align_ptr(u8* ptr, size_t alignment_pow_of_2)
{
    return (u8*)align_address((size_t)ptr, alignment_pow_of_2);
}


inline u8* align_ptr_up(u8* ptr, size_t alignment_pow_of_2)
{
    return (u8*)align_address_up((size_t)ptr, alignment_pow_of_2);
}


inline u32 address_is_pow_of_2(size_t address)
{
    return (address & (address - 1u)) == 0u;
}

/*
// See "Find the log base 2 of an N-bit integer"
// at https://graphics.stanford.edu/~seander/bithacks.html
u32 u32_log2(u32 a)
{
    static const u32 de_bruijn[32] = 
    {
        0u, 9u, 1u, 10u, 13u, 21u, 2u, 29u, 11u, 14u, 16u, 18u, 22u, 25u, 3u, 30u,
        8u, 12u, 20u, 28u, 15u, 17u, 24u, 7u, 19u, 27u, 23u, 6u, 26u, 5u, 4u, 31u
    };

    a |= a >> 1;
    a |= a >> 2;
    a |= a >> 4;
    a |= a >> 8;
    a |= a >> 16;

    return de_bruijn[(uint32_t)(a * 0x07C4ACDDU) >> 27];
}*/