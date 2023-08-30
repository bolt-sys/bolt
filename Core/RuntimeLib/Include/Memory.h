#pragma once

#include "Base.h"

// --------------------------------------------------- Common Memory Operations

/**
 * @brief Copies a source buffer to a destination buffer.
 *
 * @param[in] Destination           The destination buffer of the memory copy.
 * @param[in] DestinationSize       The size of the destination buffer.
 *                                  to prevent accidental overflow.
 * @param[in] Source                The source buffer of the memory copy.
 * @param[in] Length                The number of bytes to copy from
 *                                  Source to Destination.
 *
 * @return STATUS_SUCCESS           The copy was successful.
 * @return STATUS_INVALID_PARAMETER Destination or Source are NULL.
 * @return STATUS_INVALID_PARAMETER Destination overlaps with Source.
 * @return STATUS_BUFFER_TOO_SMALL  DestinationSize is too small for the copy.
 **/
STATUS
SYSAPI
CopyMemory (
    IN        VOID* Destination,
    IN        UINTN DestinationSize,
    IN CONST  VOID* Source,
    IN        UINTN Length
    );

/**
 * @brief Fills a buffer with a specified byte value.
 *
 * @param[in] Destination           The destination buffer to fill.
 * @param[in] Length                The number of bytes in Destination to fill.
 * @param[in] Value                 The byte value with which to
 *                                  fill Length bytes of Destination.
 *
 * @return STATUS_SUCCESS           The fill was successful.
 * @return STATUS_INVALID_PARAMETER Destination is NULL.
 **/
STATUS
SYSAPI
FillMemory (
    IN VOID* Destination,
    IN UINTN Length,
    IN UINT8 Value
    );

/**
 * @brief Zeroes a buffer. This is essentially equivalent to calling
 *        FillMemory with a Value of 0.
 *
 * @param[in] Destination           The destination buffer to zero.
 * @param[in] Length                The number of bytes in Destination to zero.
 *
 * @return STATUS_SUCCESS           The zero was successful.
 * @return STATUS_INVALID_PARAMETER Destination is NULL.
 **/
STATUS
SYSAPI
ZeroMemory (
    IN VOID* Destination,
    IN UINTN Length
    );

// -------------------------------------------------------- Page SlabAllocation

#define PAGE_SIZE_4K 0x1000     // Base page size
#define PAGE_SIZE_2M 0x200000   // Large page size
#define PAGE_SIZE_1G 0x40000000 // Huge page size

/**
 * @brief Allocate a range of pages
 *
 * @param[out] Address The address of the allocated memory
 * @param[in]  Pages   The amount of pages to allocate
 * @param[in]  Align   The alignment of the allocation
 *                     must be (PAGE_SIZE_4K, PAGE_SIZE_2M, PAGE_SIZE_1G)
 *
 * @return STATUS_SUCCESS             The allocation was successful
 * @return STATUS_INVALID_PARAMETER   Address is NULL or Pages is 0
 * @return STATUS_UNALIGNED           Align is not a valid page size
 * @return STATUS_OUT_OF_MEMORY       The allocation failed
 * @return STATUS_NOT_INITIALIZED     The Allocator is not initialized
 *
 * @note This function must be implemented by the calling api.
 *       It is not implemented in the RuntimeLib.
 *
 *       it is being used by the RTL Memory Management functions.
 **/
STATUS
SYSAPI
AllocatePages (
    OUT VOID** Address,
    IN  UINTN  Pages,
    IN  UINTN  Align
    );

/**
 * @brief Free a range of pages
 *
 * @param[in,out] Address The address of the allocated memory
 * @param[in]     Pages   The amount of pages to free
 *
 * @return STATUS_SUCCESS              The free was successful
 * @return STATUS_INVALID_PARAMETER    Address is NULL or Pages is 0
 * @return STATUS_UNALIGNED            The address is not page aligned
 * @return STATUS_NOT_IMPLEMENTED      The function is not implemented
 * @return STATUS_NOT_INITIALIZED      The Allocator is not initialized
 *
 * @note This function will not check for the validity of the address
 *       or the amount of pages to free. It is up to the caller to
 *       ensure that the address and the amount of pages are correct.
 *
 * @note This function must be implemented by the calling api.
 *       It is not implemented in the RuntimeLib.
 *
 *       it is being used by the RTL Memory Management functions.
 **/
STATUS
SYSAPI
FreePages (
    IN OUT VOID** Address,
    IN     UINTN  Pages
    );

// ------------------------------------------------------------ Slab Allocation

#define SLAB_CACHE_FLAGS_NONE BIT(0)
#define SLAB_CACHE_FLAGS_ZERO BIT(1)

typedef struct _SLAB_CACHE SLAB_CACHE;

typedef STATUS SYSAPI      (* SLAB_CTOR)(
    IN          VOID*       Object,
    IN          SLAB_CACHE* Cache,
    IN OPTIONAL UINTN       Flags
    );

typedef STATUS SYSAPI (* SLAB_DTOR)(
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
    struct _SLAB* Full;
    struct _SLAB* Partial;
    struct _SLAB* Empty;

    SLAB_CTOR     Ctor;
    SLAB_DTOR     Dtor;

    UINTN         Flags;

    UINTN         ObjectSize;  // Size of the objects to be allocated
    UINTN         ObjectCount; // Number of objects per slab
} SLAB_CACHE;

STATUS
SYSAPI
DefaultSlabCtor (
    IN          VOID*       Object,
    IN          SLAB_CACHE* Cache,
    IN OPTIONAL UINTN       Flags
    );

STATUS
SYSAPI
DefaultSlabDtor (
    IN          VOID*       Object,
    IN          SLAB_CACHE* Cache,
    IN OPTIONAL UINTN       Flags
    );

/**
 * @brief Create a slab cache
 *
 * @param[out]         Cache      The cache to initialize
 * @param[in]          ObjectSize The size of the objects to be allocated
 * @param[in,optional] Flags      The flags for the cache
 * @param[in,optional] Ctor       The constructor for the objects
 * @param[in,optional] Dtor       The destructor for the objects
 *
 * @return STATUS_SUCCESS              The initialization was successful
 * @return STATUS_INVALID_PARAMETER    Cache is NULL or ObjectSize is 0
 * @return STATUS_NOT_INITIALIZED      The Page Allocator is not initialized
 **/
STATUS
SYSAPI
CreateSlabCache (
    OUT         SLAB_CACHE** Cache,
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
DestroySlabCache (
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
SlabAllocate (
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
SlabFree (
    IN     SLAB_CACHE* Cache,
    IN     UINTN       Flags,
    IN OUT VOID**      Object
    );

// --------------------------------------------- General Purpose SlabAllocation

/**
 * @brief Initialize the Heap
 *
 * @return STATUS_SUCCESS             The initialization was successful
 * @return STATUS_ALREADY_INITIALIZED The Heap is already initialized
 * @return STATUS_OUT_OF_MEMORY       The Heap could not be initialized
 *                                    due to lack of memory
 **/
STATUS
SYSAPI
HeapInit (
    VOID
    );

/**
 * @brief Allocate Memory
 *
 * @param[out] Address The address of the allocated memory
 * @param[in]  Flags   The flags for the allocation
 * @param[in]  Size    The size of the allocation
 *
 * @return STATUS_SUCCESS             The allocation was successful
 * @return STATUS_INVALID_PARAMETER   Address is NULL
 * @return STATUS_OUT_OF_MEMORY       The allocation failed
 * @return STATUS_NOT_INITIALIZED     The Allocator is not initialized
 **/
STATUS
SYSAPI
Allocate (
    OUT VOID** Address,
    IN  UINTN  Flags,
    IN  UINTN  Size
    );

/**
 * @brief Free Memory
 *
 * @param[in,out] Address The address of the allocated memory
 * @param[in]     Flags   The flags for the free
 *
 * @return STATUS_SUCCESS              The free was successful
 * @return STATUS_INVALID_PARAMETER    Address is NULL
 * @return STATUS_NOT_FOUND            The address was not found in the allocator
 * @return STATUS_NOT_INITIALIZED      The Allocator is not initialized
 **/
STATUS
SYSAPI
Free (
    IN OUT VOID** Address,
    IN     UINTN  Flags
    );

/**
 * @brief Reallocate Memory
 *
 * @param[in,out] Address The address of the allocated memory
 * @param[in]     Flags   The flags for the reallocation
 * @param[in]     Size    The new size of the allocation
 *
 * @note It is not guaranteed that the address will be the same after the reallocation
 *       but data will be copied over to the new address.
 *
 * @return STATUS_SUCCESS              The reallocation was successful
 * @return STATUS_INVALID_PARAMETER    Address is NULL
 * @return STATUS_NOT_FOUND            The address was not found in the allocator
 * @return STATUS_NOT_INITIALIZED      The Allocator is not initialized
 * @return STATUS_OUT_OF_MEMORY        The reallocation failed
 **/
STATUS
SYSAPI
Reallocate (
    IN OUT VOID** Address,
    IN     UINTN  Flags,
    IN     UINTN  Size
    );
