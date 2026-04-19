#pragma once

/*
BrunoNotes C helper lib

#define BN_IMPLEMENTATION
#include "bn.h"

References:
- nob.h (https://github.com/tsoding/nob.h)
*/

#define BN_ASSERT_ENABLED // comment to disable asserts

#if defined(__cplusplus)
extern "C" {
#endif

// clang-format off
#ifndef thread_local
    #define thread_local _Thread_local
#endif

#ifdef __GNUC__
    #ifndef _VA_LIST_DEFINED
        #include <stdio.h>
        typedef __gnuc_va_list va_list;
        #define _VA_LIST_DEFINED
    #endif
#else
    #include <stdarg.h>
#endif

#ifndef va_arg
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
        /* C23 uses a special builtin. */
        #define va_start(...) __builtin_c23_va_start(__VA_ARGS__)
    #else
        /* Versions before C23 do require the second parameter. */
        #define va_start(ap, param) __builtin_va_start(ap, param)
    #endif
    #define va_end(ap) __builtin_va_end(ap)
    #define va_arg(ap, type) __builtin_va_arg(ap, type)
#endif

#if _MSC_VER
    #include <intrin.h>
    #define debugBreak() __debugbreak()
#else
    #define debugBreak() __builtin_trap()
#endif
// clang-format on

#include <stdint.h>
#include <stdbool.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef i8 b8;
typedef i32 b32;
typedef i64 b64;
typedef float f32;
typedef double f64;

#define arrayLength(a) (sizeof(a) / sizeof(a[0]))

#define bnSlicePrototype(type)                                                 \
    typedef struct {                                                           \
        type* ptr;                                                             \
        u32 length;                                                            \
    } type##Slice

#define sliceInit(type, p, len) ((type##Slice){.ptr = (p), .length = (len)})

#define sliceFromArray(type, ...)                                              \
    ((type##Slice){                                                            \
        .items = (type[]){__VA_ARGS__},                                        \
        .length = sizeof((type[]){__VA_ARGS__}) / sizeof(type)                 \
    })

bnSlicePrototype(i8);
bnSlicePrototype(i16);
bnSlicePrototype(i32);
bnSlicePrototype(i64);
bnSlicePrototype(u8);
bnSlicePrototype(u16);
bnSlicePrototype(u32);
bnSlicePrototype(u64);
bnSlicePrototype(b8);
bnSlicePrototype(b32);
bnSlicePrototype(b64);
bnSlicePrototype(f32);
bnSlicePrototype(f64);

typedef struct {
    char* str;
    u32 length;
} String;

#define stringLit(s)                                                           \
    (String){(char*)(s), sizeof((s)) - 1} // makes a string from char*
#define stringFmt(s)                                                           \
    (int)(s).length, (s).str // to be used with "%.*s" in printf for example

#define Byte 1
#define KiloByte (u32)(1024 * Byte)
#define MegaByte (u32)(1024 * KiloByte)
#define GigaByte (u32)(1024 * MegaByte)
#define TeraByte (u32)(1024 * GigaByte)
#define PetaByte (u32)(1024 * TeraByte)
#define ExaByte (u32)(1024 * PetaByte)

#define Kib(n) ((u32)(n) * KiloByte)
#define Mib(n) ((u32)(n) * MegaByte)
#define Gib(n) ((u32)(n) * GigaByte)

#define bnMin(a, b) (((a) < (b)) ? (a) : (b))
#define bnMax(a, b) (((a) > (b)) ? (a) : (b))

#define bnAlignPow2(x, b) (((x) + (b) - 1) & (~((b) - 1)))

typedef enum {
    BN_LogLevel_Debug = 0,
    BN_LogLevel_Info = 1,
    BN_LogLevel_Warning = 2,
    BN_LogLevel_Error = 3,
    BN_LogLevel_Panic = 4,
} BN_LogLevel;

void bnDefaultLogHandler(BN_LogLevel level, const char* fmt, va_list args);

typedef void(BN_LogHandler)(BN_LogLevel level, const char* fmt, va_list args);

void bnLog(BN_LogLevel level, const char* fmt, ...);
BN_LogLevel bnGetMinLogLevel();
void bnSetMinLogLevel(BN_LogLevel level);
void bnSetLogHandler(BN_LogHandler* handler);

#define bnLogInfo(message) bnLog(BN_LogLevel_Info, message)
#define bnLogInfof(...) bnLog(BN_LogLevel_Info, __VA_ARGS__)
#define bnLogWarn(message) bnLog(BN_LogLevel_Warning, message)
#define bnLogWarnf(...) bnLog(BN_LogLevel_Warning, __VA_ARGS__)
#define bnLogError(message) bnLog(BN_LogLevel_Error, message)
#define bnLogErrorf(...) bnLog(BN_LogLevel_Error, __VA_ARGS__)
#define bnLogPanic(message) bnLog(BN_LogLevel_Panic, message)
#define bnLogPanicf(...) bnLog(BN_LogLevel_Panic, __VA_ARGS__)
#define bnLogDebug(message) bnLog(BN_LogLevel_Debug, message)
#define bnLogDebugf(...) bnLog(BN_LogLevel_Debug, __VA_ARGS__)
#define bnLogFatal(message) bnLog(BN_LogLevel_Fatal, message)
#define bnLogFatalf(...) bnLog(BN_LogLevel_Fatal, __VA_ARGS__)

// clang-format off
#ifdef BN_ASSERT_ENABLED
    #define bnAssert(expr)                                                         \
        do {                                                                       \
            if (expr) {                                                            \
            } else {                                                               \
                bnLogErrorf(                                                         \
                    "Assertion Failure: %s, in file: %s, line: %d", #expr,         \
                    __FILE__, __LINE__                                             \
                );                                                                 \
                debugBreak();                                                      \
            }                                                                      \
        } while (0)

    #define bnAssertMsg(expr, msg)                                                 \
        do {                                                                       \
            if (expr) {                                                            \
            } else {                                                               \
                bnLogErrorf(                                                         \
                    "Assertion Failure: %s, in file: %s, line: %d, msg: %s",       \
                    #expr, __FILE__, __LINE__, msg                                 \
                );                                                                 \
                debugBreak();                                                      \
            }                                                                      \
        } while (0)
#else
    #define bnAssert(expr)
    #define bnAssertMsg(expr, msg)
#endif
// clang-format on

u32 bnPlatformGetPageSize(void);
void* bnPlatformMemReserve(u64 size);
b32 bnPlatformMemCommit(void* ptr, u64 size);
b32 bnPlatformMemDecommit(void* ptr, u64 size);
b32 bnPlatformMemRelease(void* ptr, u64 size);

#define BN_ARENA_BASE_POS (sizeof(BN_Arena))
#define BN_ARENA_ALIGN (sizeof(void*))

typedef enum {
    BN_ArentaType_Static,
    BN_ArenaType_Growing,
} BN_ArenaType;

typedef struct {
    u64 reserve_size;
    u64 commit_size;
    u64 pos;
    u64 commit_pos;
    BN_ArenaType type;
} BN_Arena;

BN_Arena* bnInitArena(u64 reserve_size, u64 commit_size, BN_ArenaType type);
void bnDestroyArena(BN_Arena* arena);

void* bnArenaPush(BN_Arena* arena, u64 size, b32 non_zero);
void bnArenaRealloc(BN_Arena* arena, u64 new_pos);
void bnArenaPop(BN_Arena* arena, u64 size);
void bnArenaPopTo(BN_Arena* arena, u64 pos);

typedef struct {
    BN_Arena* arena;
    u64 start_pos;
} BN_TempArena;

BN_TempArena bnBeginTempArena(BN_Arena* arena);
void bnEndTempArena(BN_TempArena temp);

void* bnHeapAllocatorPush(u64 size, b32 non_zero);
void bnHeapAllocatorFree(void* data, u64 size);

typedef enum {
    BN_AllocatorType_ArenaStatic,
    BN_AllocatorType_ArenaGrowing,
    BN_AllocatorType_HeapAllocator,
    BN_AllocatorType_TempAllocator,
} BN_AllocatorType;

typedef struct {
    u64 reserve_size;
    u64 commit_size;
    BN_AllocatorType type;
} BN_AllocatorInitParams;

typedef struct {
    u64 size;
    b32 non_zero;
} BN_AllocatorParams;

typedef struct {
    BN_AllocatorType type;
    void* data;
    u64 size;
    BN_TempArena _temp;
} BN_Allocator;

BN_Allocator bnInitAllocator(BN_AllocatorInitParams params);
void bnDestroyAllocator(BN_Allocator* alloc);

void* bnAllocatorPush(BN_AllocatorParams params, BN_Allocator* alloc);
void bnAllocatorFree(BN_Allocator* alloc, void* data);
void bnAllocatorFreeAll(BN_Allocator* alloc);

#define bnAllocPushStruct(alloc, T)                                            \
    (T*)bnAllocatorPush((BN_AllocatorParams){sizeof(T), false}, (alloc))
#define bnAllocPushStructNZ(alloc, T)                                          \
    (T*)bnAllocatorPush((BN_AllocatorParams){sizeof(T), true}, (alloc))
#define bnAllocPushArray(alloc, T, len)                                        \
    (T*)bnAllocatorPush((BN_AllocatorParams){sizeof(T) * (len), false}, (alloc))
#define bnAllocPushArrayNZ(alloc, T, len)                                      \
    (T*)bnAllocatorPush((BN_AllocatorParams){sizeof(T) * (len), true}, (alloc))

#define bnArrayPrototype(type)                                                 \
    typedef struct {                                                           \
        type* items;                                                           \
        u32 count;                                                             \
        u32 capacity;                                                          \
        BN_Allocator* allocator;                                               \
    } type##BNArray

bnArrayPrototype(i8);
bnArrayPrototype(i16);
bnArrayPrototype(i32);
bnArrayPrototype(i64);
bnArrayPrototype(u8);
bnArrayPrototype(u16);
bnArrayPrototype(u32);
bnArrayPrototype(u64);
bnArrayPrototype(b8);
bnArrayPrototype(b32);
bnArrayPrototype(b64);
bnArrayPrototype(f32);
bnArrayPrototype(f64);
bnArrayPrototype(String);

#ifndef BN_DYNAMIC_ARRAY_INITIAL_CAPACITY
#define BN_DYNAMIC_ARRAY_INITIAL_CAPACITY 256
#endif

#define bnDaInit(alloc)                                                        \
    {.items = NULL, .count = 0, .capacity = 0, .allocator = alloc}

#define bnDaReserve(da, expected_capacity)                                     \
    do {                                                                       \
        if ((expected_capacity) > (da)->capacity) {                            \
            if ((da)->capacity == 0) {                                         \
                (da)->capacity = BN_DYNAMIC_ARRAY_INITIAL_CAPACITY;            \
                (da)->items = bnAllocatorPush(                                 \
                    (BN_AllocatorParams){                                      \
                        (da)->capacity * sizeof(*(da)->items), false           \
                    },                                                         \
                    (da)->allocator                                            \
                );                                                             \
            } else {                                                           \
                u32 old_capacity = (da)->capacity;                             \
                while ((expected_capacity) > (da)->capacity) {                 \
                    (da)->capacity *= 2;                                       \
                }                                                              \
                void* new_entries = bnAllocatorPush(                           \
                    (BN_AllocatorParams){                                      \
                        (da)->capacity * sizeof(*(da)->items), false           \
                    },                                                         \
                    (da)->allocator                                            \
                );                                                             \
                memcpy(                                                        \
                    new_entries, (da)->items,                                  \
                    old_capacity * sizeof(*(da)->items)                        \
                );                                                             \
                bnAllocatorFree((da)->allocator, (da)->items);                 \
                (da)->items = new_entries;                                     \
            }                                                                  \
            bnAssert((da)->items != NULL);                                     \
        }                                                                      \
    } while (0)

#define bnDaAppend(da, item)                                                   \
    do {                                                                       \
        bnDaReserve((da), (da)->count + 1);                                    \
        (da)->items[(da)->count++] = (item);                                   \
    } while (0)

#define bnDaAppendMany(da, new_items, new_items_count)                         \
    do {                                                                       \
        bnDaReserve((da), (da)->count + (new_items_count));                    \
        memcpy(                                                                \
            (da)->items + (da)->count, (new_items),                            \
            (new_items_count) * sizeof(*(da)->items)                           \
        );                                                                     \
        (da)->count += (new_items_count);                                      \
    } while (0)

#define bnDaFree(da)                                                           \
    do {                                                                       \
        bnAllocatorFree((da)->allocator, (da)->items);                         \
        (da)->count = 0;                                                       \
        (da)->capacity = 0;                                                    \
    } while (0)

#define bnDaResize(da, new_size)                                               \
    do {                                                                       \
        bnDaReserve((da), new_size);                                           \
        (da)->count = (new_size);                                              \
    } while (0)

#ifndef BN_HASH_TABLE_INITIAL_CAPACITY
#define BN_HASH_TABLE_INITIAL_CAPACITY 256
#endif

typedef struct {
    String key;
    void* value;
} BN_HashTableEntry;

typedef struct {
    BN_HashTableEntry* items;
    u32 count;
    u32 capacity;
    BN_Allocator* allocator;
} BN_HashTable;

#define bnHTInit(alloc)                                                        \
    {.items = NULL, .count = 0, .capacity = 0, .allocator = alloc}

void bnHashTableReserve(BN_HashTable* table, u32 expected_capacity);
void bnHashTableFree(BN_HashTable* table);
u32 bnHashTableLinearProbe(BN_HashTable* table, String key);
void* bnHashTableGet(BN_HashTable* table, String key);
void bnHashTableAppend(BN_HashTable* table, String key, void* value);

#define bnHTAppend(ht, key, value)                                             \
    bnHashTableAppend(ht, stringLit(key), (void*)value)

#define bnHTGet(ht, key) bnHashTableGet(ht, stringLit(key))

#ifdef BN_IMPLEMENTATION

#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
static thread_local BN_LogLevel bn_g_min_log_level = BN_LogLevel_Debug;
#else
static thread_local BN_LogLevel bn_g_min_log_level = BN_LogLevel_Info;
#endif

static thread_local BN_LogHandler* bn_g_log_handler = NULL;

void bnDefaultLogHandler(BN_LogLevel level, const char* fmt, va_list args) {
    if (level < bn_g_min_log_level) {
        return;
    }

    switch (level) {
    case BN_LogLevel_Debug:
        fprintf(stderr, "\x1b[90m[Debug]\x1b[0m ");
        break;
    case BN_LogLevel_Info:
        fprintf(stderr, "\x1b[0m[Info]\x1b[0m ");
        break;
    case BN_LogLevel_Warning:
        fprintf(stderr, "\x1b[33m[Warn]\x1b[0m ");
        break;
    case BN_LogLevel_Error:
        fprintf(stderr, "\x1b[31m[Error]\x1b[0m ");
        break;
    case BN_LogLevel_Panic:
        fprintf(stderr, "\x1b[35m[Panic]\x1b[0m ");
        break;
    default:
        return;
    }

    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    if (level == BN_LogLevel_Panic) {
        exit(1);
    }
}

void bnLog(BN_LogLevel level, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    if (bn_g_log_handler == NULL) {
        bn_g_log_handler = &bnDefaultLogHandler;
    }

    bn_g_log_handler(level, fmt, args);
    va_end(args);
}

BN_LogLevel bnGetMinLogLevel() {
    return bn_g_min_log_level;
}
void bnSetMinLogLevel(BN_LogLevel level) {
    bn_g_min_log_level = level;
}

void bnSetLogHandler(BN_LogHandler* handler) {
    bn_g_log_handler = handler;
}

#if defined(_WIN32)

#include <windows.h>

u32 bnPlatformGetPageSize(void) {
    SYSTEM_INFO sysinfo = {0};
    GetSystemInfo(&sysinfo);

    return sysinfo.dwPageSize;
}

void* bnPlatformMemReserve(u64 size) {
    return VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_READWRITE);
}

b32 bnPlatformMemCommit(void* ptr, u64 size) {
    void* ret = VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE);
    return ret != NULL;
}

b32 bnPlatformMemDecommit(void* ptr, u64 size) {
    return VirtualFree(ptr, size, MEM_DECOMMIT);
}

b32 bnPlatformMemRelease(void* ptr, u64 size) {
    return VirtualFree(ptr, size, MEM_RELEASE);
}

#elif defined(__linux__)

#include <sys/mman.h>
#include <unistd.h>

u32 bnPlatformGetPageSize(void) {
    return (u32)sysconf(_SC_PAGESIZE);
}

void* bnPlatformMemReserve(u64 size) {
    void* out = mmap(NULL, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (out == MAP_FAILED) {
        return NULL;
    }
    return out;
}

b32 bnPlatformMemCommit(void* ptr, u64 size) {
    i32 ret = mprotect(ptr, size, PROT_READ | PROT_WRITE);
    return ret == 0;
}

b32 bnPlatformMemDecommit(void* ptr, u64 size) {
    i32 ret = mprotect(ptr, size, PROT_NONE);
    if (ret != 0)
        return false;
    ret = madvise(ptr, size, MADV_DONTNEED);
    return ret == 0;
}

b32 bnPlatformMemRelease(void* ptr, u64 size) {
    i32 ret = munmap(ptr, size);
    return ret == 0;
}

#endif

BN_Arena* bnInitArena(u64 reserve_size, u64 commit_size, BN_ArenaType type) {
    bnAssert(reserve_size > 0);
    bnAssert(commit_size > 0);

    u32 page_size = bnPlatformGetPageSize();

    reserve_size = bnAlignPow2(reserve_size, page_size);
    commit_size = bnAlignPow2(commit_size, page_size);

    BN_Arena* arena = (BN_Arena*)bnPlatformMemReserve(reserve_size);

    if (!bnPlatformMemCommit(arena, commit_size)) {
        return NULL;
    }

    arena->reserve_size = reserve_size;
    arena->commit_size = commit_size;
    arena->pos = BN_ARENA_BASE_POS;
    arena->commit_pos = commit_size;
    arena->type = type;

    return arena;
}

void bnDestroyArena(BN_Arena* arena) {
    bool ok = bnPlatformMemRelease(arena, arena->reserve_size);
    bnAssert(ok == true);
}

void* bnArenaPush(BN_Arena* arena, u64 size, b32 non_zero) {
    u64 pos_aligned = bnAlignPow2(arena->pos, BN_ARENA_ALIGN);
    u64 new_pos = pos_aligned + size;

    // Grow arena if needed
    if (new_pos > arena->reserve_size && arena->type == BN_ArenaType_Growing) {
        bnArenaRealloc(arena, new_pos);
    }

    if (new_pos > arena->commit_pos) {
        u64 new_commit_pos = new_pos;
        new_commit_pos += arena->commit_size - 1;
        new_commit_pos -= new_commit_pos % arena->commit_size;
        new_commit_pos = bnMin(new_commit_pos, arena->reserve_size);

        u8* mem = (u8*)arena + arena->commit_pos;
        u64 commit_size = new_commit_pos - arena->commit_pos;

        if (!bnPlatformMemCommit(mem, commit_size)) {
            return NULL;
        }

        arena->commit_pos = new_commit_pos;
    }

    arena->pos = new_pos;

    u8* out = (u8*)arena + pos_aligned;

    if (!non_zero) {
        memset(out, 0, size);
    }

    return out;
}

void bnArenaRealloc(BN_Arena* arena, u64 new_pos) {
    u64 new_reserve_size = arena->reserve_size * 2;
    while (new_reserve_size < new_pos) {
        new_reserve_size *= 2;
    }

    u32 page_size = bnPlatformGetPageSize();
    new_reserve_size = bnAlignPow2(new_reserve_size, page_size);

    BN_Arena* new_arena = (BN_Arena*)bnPlatformMemReserve(new_reserve_size);
    bnAssert(new_arena != NULL);

    // Copy existing data and commit the current committed size
    memcpy(new_arena, arena, arena->commit_pos);
    if (!bnPlatformMemCommit(new_arena, arena->commit_pos)) {
        bnPlatformMemRelease(new_arena, new_reserve_size);
        return;
    }

    new_arena->reserve_size = new_reserve_size;
    new_arena->commit_size = arena->commit_size;
    new_arena->pos = arena->pos;
    new_arena->commit_pos = arena->commit_pos;

    // Release old arena and update pointer
    bnPlatformMemRelease(arena, arena->reserve_size);

    arena = new_arena;
}

void bnArenaPop(BN_Arena* arena, u64 size) {
    size = bnMin(size, arena->pos - BN_ARENA_BASE_POS);
    arena->pos -= size;
}

void bnArenaPopTo(BN_Arena* arena, u64 pos) {
    u64 size = pos < arena->pos ? arena->pos - pos : 0;
    bnArenaPop(arena, size);
}

BN_TempArena bnBeginTempArena(BN_Arena* arena) {
    return (BN_TempArena){
        .arena = arena,
        .start_pos = arena->pos,
    };
}

void bnEndTempArena(BN_TempArena temp) {
    bnArenaPopTo(temp.arena, temp.start_pos);
}

BN_Allocator bnInitAllocator(BN_AllocatorInitParams params) {
    BN_Allocator alloc = {0};

    switch (params.type) {
    case BN_AllocatorType_ArenaStatic:
        alloc.data = bnInitArena(
            params.reserve_size, params.commit_size, BN_ArentaType_Static
        );
        alloc.type = params.type;
        break;
    case BN_AllocatorType_ArenaGrowing:
        if (params.reserve_size <= 0) {
            params.reserve_size = Mib(32);
        }
        if (params.commit_size <= 0) {
            params.commit_size = Mib(1);
        }

        alloc.data = bnInitArena(
            params.reserve_size, params.commit_size, BN_ArenaType_Growing
        );
        alloc.type = params.type;
        break;
    case BN_AllocatorType_HeapAllocator:
        alloc.data = NULL;
        alloc.size = 0;
        alloc.type = params.type;
        break;
    case BN_AllocatorType_TempAllocator:
        alloc.data = bnInitArena(
            params.reserve_size, params.commit_size, BN_ArenaType_Growing
        );
        alloc.type = params.type;
        alloc._temp = bnBeginTempArena((BN_Arena*)alloc.data);
        break;
    default:
        bnLogPanic("Allocator not implemented");
        break;
    }

    return alloc;
}

void bnDestroyAllocator(BN_Allocator* alloc) {
    switch (alloc->type) {
    case BN_AllocatorType_ArenaStatic:
    case BN_AllocatorType_ArenaGrowing:
        bnDestroyArena((BN_Arena*)alloc->data);
        break;
    case BN_AllocatorType_HeapAllocator:
        break;
    case BN_AllocatorType_TempAllocator:
        bnEndTempArena(alloc->_temp);
        bnDestroyArena((BN_Arena*)alloc->data);
        break;
    default:
        bnLogPanic("Allocator not implemented");
        break;
    }
}

void* bnHeapAllocatorPush(u64 size, b32 non_zero) {
    // void* data = malloc(size);
    u32 page_size = bnPlatformGetPageSize();
    size = bnAlignPow2(size, page_size);
    void* data = bnPlatformMemReserve(size);

    if (!bnPlatformMemCommit(data, size)) {
        return NULL;
    }

    if (!non_zero) {
        memset(data, 0, size);
    }

    return data;
}

void bnHeapAllocatorFree(void* data, u64 size) {
    // free(data);
    // data = NULL;
    bnPlatformMemRelease(data, size);
}

void* bnAllocatorPush(BN_AllocatorParams params, BN_Allocator* alloc) {
    switch (alloc->type) {
    case BN_AllocatorType_ArenaStatic:
    case BN_AllocatorType_ArenaGrowing:
        return bnArenaPush(
            (BN_Arena*)alloc->data, params.size, params.non_zero
        );
    case BN_AllocatorType_HeapAllocator:
        alloc->size = params.size;
        return bnHeapAllocatorPush(params.size, params.non_zero);
    case BN_AllocatorType_TempAllocator:
        return bnArenaPush(
            (BN_Arena*)alloc->_temp.arena, params.size, params.non_zero
        );
    default:
        bnLogPanic("Allocator not initialized");
        return NULL;
    }
}

void bnAllocatorFree(BN_Allocator* alloc, void* data) {
    switch (alloc->type) {
    case BN_AllocatorType_ArenaStatic:
    case BN_AllocatorType_ArenaGrowing:
    case BN_AllocatorType_TempAllocator:
        break;
    case BN_AllocatorType_HeapAllocator:
        bnHeapAllocatorFree(data, alloc->size);
        break;
    default:
        bnLogPanic("Allocator not initialized");
        break;
    }
}

void bnAllocatorFreeAll(BN_Allocator* alloc) {
    switch (alloc->type) {
    case BN_AllocatorType_ArenaStatic:
    case BN_AllocatorType_ArenaGrowing:
        bnArenaPopTo((BN_Arena*)alloc->data, BN_ARENA_BASE_POS);
        break;
    case BN_AllocatorType_HeapAllocator:
        break;
    case BN_AllocatorType_TempAllocator:
        bnEndTempArena(alloc->_temp);
        alloc->_temp = bnBeginTempArena((BN_Arena*)alloc->data);
        break;
    default:
        bnLogPanic("Allocator not initialized");
        break;
    }
}

static inline u32 fvn32aHash(u8* data, u32 length) {
    u32 h = 0x811c9dc5;

    for (u32 i = 0; i < length; i++) {
        h = (h ^ (u32)data[i]) * 0x01000193;
    }

    return h;
}

void bnHashTableReserve(BN_HashTable* table, u32 expected_capacity) {
    if (expected_capacity > table->capacity) {
        if (table->capacity == 0) {
            table->capacity = BN_HASH_TABLE_INITIAL_CAPACITY;
            table->items = (BN_HashTableEntry*)bnAllocatorPush(
                (BN_AllocatorParams){
                    table->capacity * sizeof(*table->items), false
                },
                table->allocator
            );
        } else {
            while (expected_capacity > table->capacity) {
                table->capacity *= 2;
            }
            BN_HashTableEntry* new_entries =
                (BN_HashTableEntry*)bnAllocatorPush(
                    (BN_AllocatorParams){
                        table->capacity * sizeof(*table->items), false
                    },
                    table->allocator
                );

            for (u32 i = 0; i < table->capacity; i++) {
                if (table->items[i].key.str != NULL) {
                    new_entries[i] = table->items[i];
                }
            }

            bnAllocatorFree(table->allocator, table->items);
            table->items = new_entries;
        }
        bnAssert(table->items != NULL);
    }
}

void bnHashTableFree(BN_HashTable* table) {
    bnAllocatorFree(table->allocator, table->items);
    table->count = 0;
    table->capacity = 0;
}

u32 bnHashTableLinearProbe(BN_HashTable* table, String key) {
    u32 hash = fvn32aHash((u8*)key.str, key.length);
    u32 idx = (u32)(hash & (u32)(table->capacity - 1));

    while (table->items[idx].key.str != NULL) {
        if (strcmp(key.str, table->items[idx].key.str) == 0) {
            // Found key
            return idx;
        }
        // Key wasn't in this slot, move to next (linear probing).
        idx++;
        if (idx >= table->capacity) {
            // At end of entries array, wrap around.
            idx = 0;
        }
    }
    return idx;
}

void* bnHashTableGet(BN_HashTable* table, String key) {
    u32 idx = bnHashTableLinearProbe(table, key);

    return table->items[idx].value;
}

void bnHashTableAppend(BN_HashTable* table, String key, void* value) {
    bnAssert(value != NULL);

    bnHashTableReserve(table, table->count + 1);

    u32 idx = bnHashTableLinearProbe(table, key);

    if (table->items[idx].key.str == NULL) {
        table->items[idx].key = key;
    }
    table->items[idx].value = value;
    table->count++;
}

#endif

#if defined(__cplusplus)
}
#endif
