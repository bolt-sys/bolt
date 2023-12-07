#pragma once

#ifndef __RUNTIME_LIB_H__
    #error "This file should be included from RuntimeLib.h"
#endif

#define SLAB_CACHE_FLAGS_NONE BIT(0)
#define SLAB_CACHE_FLAGS_ZERO BIT(1)

typedef struct _SLAB_CACHE SLAB_CACHE;

typedef
STATUS
(* SLAB_CTOR)(
    IN          VOID*       Object,
    IN          SLAB_CACHE* Cache,
    IN OPTIONAL UINTN       Flags
    );

typedef
STATUS
(* SLAB_DTOR)(
    IN          VOID*       Object,
    IN          SLAB_CACHE* Cache,
    IN OPTIONAL UINTN       Flags
    );

typedef struct _SLAB {
    struct _SLAB*       Next;
    struct _SLAB*       Prev;
    struct _SLAB_CACHE* Cache;

    CHAR8*              FreeMap;
    CHAR8*              FreeMapEnd;
    UINTN               FreeCount;

    VOID*               Data;
    VOID*               DataEnd;
} SLAB;

typedef struct _SLAB_CACHE {
    struct _SLAB*   Full;
    struct _SLAB*   Partial;
    struct _SLAB*   Empty;

    SLAB_CTOR       Ctor;
    SLAB_DTOR       Dtor;

    UINTN           Flags;

    UINTN           ObjectSize;  // Size of the objects to be allocated
    UINTN           ObjectCount; // Number of objects per slab

    PAGE_ALLOCATOR* BaseAllocator;
} SLAB_CACHE;

STATUS
SYSAPI
RtlDefaultSlabCtor (
    IN          VOID*       Object,
    IN          SLAB_CACHE* Cache,
    IN OPTIONAL UINTN       Flags
    );

STATUS
SYSAPI
RtlDefaultSlabDtor (
    IN          VOID*       Object,
    IN          SLAB_CACHE* Cache,
    IN OPTIONAL UINTN       Flags
    );

/**
 * @brief Create a slab cache
 *
 * @param[out]         Cache         The cache to initialize
 * @param[in]          BaseAllocator The base allocator to use
 * @param[in]          ObjectSize    The size of the objects to be allocated
 * @param[in,optional] Flags         The flags for the cache
 * @param[in,optional] Ctor          The constructor for the objects
 * @param[in,optional] Dtor          The destructor for the objects
 *
 * @return STATUS_SUCCESS              The initialization was successful
 * @return STATUS_INVALID_PARAMETER    Cache is NULL or ObjectSize is 0
 * @return STATUS_NOT_INITIALIZED      The Page Allocator is not initialized
 **/
STATUS
SYSAPI
RtlCreateSlabCache (
    OUT         SLAB_CACHE** Cache,
    IN PAGE_ALLOCATOR*       BaseAllocator,
    IN          UINTN        ObjectSize,
    IN OPTIONAL UINTN        Flags,
    IN OPTIONAL SLAB_CTOR    Ctor,
    IN OPTIONAL SLAB_DTOR    Dtor
    );

/**
 * @brief Destroy a slab cache
 *
 * @param[in,out] Cache The cache to destroy
 *
 * @return STATUS_SUCCESS              The destruction was successful
 * @return STATUS_INVALID_PARAMETER    Cache is NULL
 * @return STATUS_NOT_INITIALIZED      The Page Allocator is not initialized
 **/
STATUS
SYSAPI
RtlDestroySlabCache (
    IN OUT SLAB_CACHE** Cache
    );

/**
 * @brief Allocate an object from a slab cache
 *
 * @param[out] Object The object to allocate
 * @param[in]  Cache The cache to allocate from
 * @param[in]  Flags The flags for the allocation
 *
 * @return STATUS_SUCCESS              The allocation was successful
 * @return STATUS_INVALID_PARAMETER    Object or Cache is NULL
 *
 **/
STATUS
SYSAPI
RtlSlabAllocate (
    IN  SLAB_CACHE* Cache,
    IN  UINTN       Flags,
    OUT VOID**      Object
    );

/**
 * @brief Free an object from a slab cache
 *
 * @param[in]     Cache The cache to free from
 * @param[in]     Flags The flags for the free
 * @param[in,out] Object The object to free
 *
 * @return STATUS_SUCCESS              The free was successful
 * @return STATUS_INVALID_PARAMETER    Object or Cache is NULL
 * @return STATUS_NOT_FOUND            The object was not found in the cache
 **/
STATUS
SYSAPI
RtlSlabFree (
    IN     SLAB_CACHE* Cache,
    IN     UINTN       Flags,
    IN OUT VOID**      Object
    );
