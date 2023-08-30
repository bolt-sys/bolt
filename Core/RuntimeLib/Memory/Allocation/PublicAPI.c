#include "Memory.h"
#include "Utilities.h"

// ----------------------------------------------------------------- Slab Cache

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
    )
{
    STATUS      Status;
    SLAB_CACHE* CachePtr;

    Status = STATUS_SUCCESS;

    if ((Cache == NULL) || (ObjectSize == 0)) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    if (Ctor == NULL) {
        Ctor = DefaultSlabCtor;
    }

    if (Dtor == NULL) {
        Dtor = DefaultSlabDtor;
    }

    Status = AllocatePages ((VOID**)Cache, 1, PAGE_SIZE_4K);
    if (FAILED (Status)) {
        goto Exit;
    }

    CachePtr = *Cache;

    ZeroMemory (CachePtr, sizeof (SLAB_CACHE));

    CachePtr->ObjectSize  = ObjectSize;
    CachePtr->ObjectCount = ALIGN_UP (PAGE_SIZE_4K, ObjectSize) / ObjectSize;
    CachePtr->Flags       = Flags;
    CachePtr->Empty       = NULL;
    CachePtr->Ctor        = Ctor;
    CachePtr->Dtor        = Dtor;

    //
    // Create an initial empty slab
    //
    Status = AllocateEmptySlab (CachePtr);
    if (FAILED (Status)) {
        goto Exit;
    }

Exit:
    return Status;
}

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
    )
{
    STATUS      Status;
    SLAB_CACHE* CachePtr;

    Status = STATUS_SUCCESS;

    if (Cache == NULL) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    CachePtr = *Cache;

    if (CachePtr == NULL) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    if (CachePtr->ObjectCount != 0) {
        // TODO: Destroy all objects, then free the cache
        Status = STATUS_NOT_IMPLEMENTED;
        goto Exit;
    }

    Status = FreePages ((VOID**)Cache, 1);
    if (FAILED (Status)) {
        goto Exit;
    }

    *Cache = NULL;

Exit:
    return Status;
}

// ------------------------------------------------------------ Slab Allocation

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
    )
{
    STATUS Status;

    //
    // First we want to check if there are any partially filled slabs
    // If there are, we want to allocate from them.
    //
    // If there are not, we want to check if we have an empty slab,
    // and if we do, we want to allocate from that.
    //
    // otherwise, we want to allocate a new slab and allocate from that.
    //

    Status = STATUS_NOT_IMPLEMENTED;

    return Status;
}

// --------------------------------------------------- Constructor / Destructor

STATUS
SYSAPI
DefaultSlabCtor (
    IN          VOID*       Object,
    IN          SLAB_CACHE* Cache,
    IN OPTIONAL UINTN       Flags
    )
{
    STATUS Status;

    Status = STATUS_SUCCESS;

    if ((Object == NULL) || (Cache == NULL)) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    if (Flags & SLAB_CACHE_FLAGS_ZERO) {
        ZeroMemory (Object, Cache->ObjectSize);
    }

Exit:
    return Status;
}

STATUS
SYSAPI
DefaultSlabDtor (
    IN          VOID*       Object,
    IN          SLAB_CACHE* Cache,
    IN OPTIONAL UINTN       Flags
    )
{
    STATUS Status;

    Status = STATUS_SUCCESS;

    if ((Object == NULL) || (Cache == NULL)) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    if (Flags & SLAB_CACHE_FLAGS_ZERO) {
        ZeroMemory (Object, Cache->ObjectSize);
    }

Exit:
    return Status;
}
