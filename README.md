# mmt (managed memory tools)
A small collection of functions and allocators to help manage allocated memory and reduce memory allocations.

## A list of available allocators
- [`mem_arena`](mmt/src/allocators/mem_arena.h): a chunk of pre-allocated memory, from which memory can be requested in arbitrary sizes, but only freed all at once
- [`mem_stack`](mmt/src/allocators/mem_stack.h): manages a memory block populated with data type(s) of the same size, which can be allocated and released
- [`mem_pool`](mmt/src/allocators/mem_pool.): manages a memory block from which memory can be allocated and released in arbitrary size, in the same way a stack would