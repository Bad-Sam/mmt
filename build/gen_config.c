// ========== Macros =========
// Compiler
#if defined(__clang__)
#   define CONFIG_CLANG    1
#   define CONFIG_COMPILER "CLANG"
#elif defined(__GNUC__)
#   define CONFIG_GCC      1
#   define CONFIG_COMPILER "GCC"
#elif defined(_MSC_VER)
#   define CONFIG_MSVC     1
#   define CONFIG_COMPILER "MSVC"
#else
#   define CONFIG_UNSUPPORTED_COMPILER 1
#   define CONFIG_COMPILER             "UNSUPPORTED"
#endif

// OS
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__NT__) || defined(__CYGWIN__)
#   define CONFIG_WINDOWS 1
#   define CONFIG_OS      "WINDOWS"
#   define _CRT_SECURE_NO_WARNINGS
#   define NOMINMAX
#elif defined(__linux__)
#   define CONFIG_LINUX 1
#   define CONFIG_OS    "LINUX"
#else
#   define CONFIG_UNSUPPORTED_OS 1
#   define CONFIG_OS             "UNSUPPORTED"
#endif

// ISA
#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64) || defined(_M_ARM64) || defined(__aarch64__)
#   define CONFIG_x64 1
#   define CONFIG_ISA "x64"
#elif defined(_M_IX86) || defined(_X86_) || defined(__i386__) || defined(__i386) || defined(i386) || defined(_M_ARM) || defined(_M_ARMT) ||  defined(__arm__)
#   define CONFIG_x86 1
#   define CONFIG_ISA "x86"
#else
#   define CONFIG_UNSUPPORTED_ISA 1
#   define CONFIG_ISA             "UNSUPPORTED"
#endif


// ========== Includes ==========
#include <stdio.h>
#include <stdint.h>

#if CONFIG_WINDOWS
#   include <windows.h>
#elif CONFIG_LINUX
#   include <unistd.h>
#else
#   error Unsupported OS. Please update gen_config.c
#endif


// ========== Structs & typedefs ==========
typedef uint32_t u32;

// We're only interested in L1 et L2 specs
struct cache_info
{
    u32 data_cache_line_size[2];
    u32 data_cache_size[2];
};

struct alloc_info
{
    u32 page_size;
    u32 reserve_alignment;
    u32 commit_alignment;
};


// ========== Platform info functions ==========
void config_get_alloc_info(struct alloc_info* info)
{
#if CONFIG_WINDOWS
    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);

    info->page_size         = system_info.dwPageSize;
    info->reserve_alignment = system_info.dwAllocationGranularity;
    info->commit_alignment  = info->page_size;
#elif CONFIG_LINUX
    info->page_size         = sysconf(_SC_PAGE_SIZE);
    info->reserve_alignment = info->page_size;
    info->commit_alignment  = info->page_size;
#endif
}


void config_get_data_cache_line_size(struct cache_info* cache_info)
{
#if CONFIG_WINDOWS
    DWORD length = 0u;
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION* proc_info;

    GetLogicalProcessorInformation(NULL, &length);
    proc_info = HeapAlloc(GetProcessHeap(), 0u, length);
    GetLogicalProcessorInformation(proc_info, &length);

    // 0b01 ==> L1 data cache size & line size found
    // 0b10 ==> L2 data cache size & line size found
    u32 found = 0b00;

    DWORD end_i = length / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
    for (DWORD i = 0u; i < end_i; i++)
    {
        const u32 cacheLvl = proc_info[i].Cache.Level;
        const u32 cacheIdx = cacheLvl - 1;

        if (proc_info[i].Relationship == RelationCache &&
            cacheIdx < 2 &&
            (proc_info[i].Cache.Type == CacheData || proc_info[i].Cache.Type == CacheUnified))
        {
            cache_info->data_cache_line_size[cacheIdx] = (u32)proc_info[i].Cache.LineSize;
            cache_info->data_cache_size[cacheIdx]      = (u32)proc_info[i].Cache.Size;

            found |= cacheLvl;

            // Exit when both L1 and L2 data cache size and line size have been found
            if (found == 0b11)
            {
                break;
            }
        }
    }

    HeapFree(GetProcessHeap(), 0u, proc_info);
#elif CONFIG_LINUX
    cache_info->data_cache_line_size[0] = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
    cache_info->data_cache_line_size[1] = sysconf(_SC_LEVEL2_DCACHE_LINESIZE);
    cache_info->data_cache_size[0]      = sysconf(_SC_LEVEL1_DCACHE_SIZE);
    cache_info->data_cache_size[1]      = sysconf(_SC_LEVEL2_DCACHE_SIZE);
#endif
}


// ========== Config generation ==========
// TODO: explore the __cpuid() option
#define CONFIG_MACRO_PREFIX "MMT_"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        fprintf(stderr,
                "\nIncorrect parameters passed to gen_config.exe."
                "\nFormat: gen_config.exe <config file name>");
    }

    // Get additional relevant platform info
    struct alloc_info alloc_info;
    struct cache_info cache_info;

    config_get_alloc_info(&alloc_info);
    config_get_data_cache_line_size(&cache_info);

    // Write macro config file
    fprintf(stderr, "\nWriting macros to %s... ", argv[1]);
    FILE* config_file = fopen(argv[1], "w");

    if (config_file == NULL)
    {
        fprintf(stderr, "Failed\n");
    }

    // Conditionnally compile-time defined macros
    fprintf(config_file, CONFIG_MACRO_PREFIX CONFIG_COMPILER "=1\n");
    fprintf(config_file, CONFIG_MACRO_PREFIX CONFIG_OS "=1\n");
    fprintf(config_file, CONFIG_MACRO_PREFIX CONFIG_ISA "=1\n");

    // Always defined runtime-time macros
    fprintf(config_file, CONFIG_MACRO_PREFIX "MEM_PAGE_SIZE=%uu\n",           alloc_info.page_size);
    fprintf(config_file, CONFIG_MACRO_PREFIX "MEM_RESERVE_GRANULARITY=%uu\n", alloc_info.reserve_alignment);
    fprintf(config_file, CONFIG_MACRO_PREFIX "MEM_COMMIT_GRANULARITY=%uu\n",  alloc_info.commit_alignment);
    fprintf(config_file, CONFIG_MACRO_PREFIX "L1_DCACHE_LINE_SIZE=%uu\n",     cache_info.data_cache_line_size[0]);
    fprintf(config_file, CONFIG_MACRO_PREFIX "L2_DCACHE_LINE_SIZE=%uu\n",     cache_info.data_cache_line_size[1]);
    fprintf(config_file, CONFIG_MACRO_PREFIX "L1_DCACHE_SIZE=%uu\n",          cache_info.data_cache_size[0]);
    fprintf(config_file, CONFIG_MACRO_PREFIX "L2_DCACHE_SIZE=%uu\n",          cache_info.data_cache_size[1]);
    
    fclose(config_file);
    fprintf(stderr, "Success\n");

    return 0;
}