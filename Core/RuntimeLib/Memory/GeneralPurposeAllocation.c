#include <Memory.h>

#define SLAB_ALLOCATORS 48 // 256 TiB in theory, although overkill.

SLAB_CACHE*    g_Allocators[SLAB_ALLOCATORS];
static BOOLEAN g_Heap_initialized = FALSE;

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
    )
{
    STATUS Status;

    Status = STATUS_SUCCESS;

    if (g_Heap_initialized) {
        Status = STATUS_ALREADY_INITIALIZED;
        goto Exit;
    }

    // The first index is basically unused because it is not possible to allocate
    // 0 bytes.
    for (UINTN i = 1; i < SLAB_ALLOCATORS; i++) {
        UINTN Size = BIT (i);

        Status = CreateSlabCache (&g_Allocators[i], Size, 0, NULL, NULL);
        if (FAILED (Status)) {
            goto Exit;
        }
    }

    g_Heap_initialized = TRUE;

Exit:
    return Status;
}

#define MEMORY_COOKIE 0x12345678

typedef struct {
    UINT32 Cookie;
    UINT8  Index;

    UINTN  Size;
    UINTN  Capacity;
} MEMORY_BLOCK;

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
    )
{
    STATUS        Status;
    UINTN         Index;
    MEMORY_BLOCK* Block;

    Status = STATUS_SUCCESS;

    if (!g_Heap_initialized) {
        Status = STATUS_NOT_INITIALIZED;
        goto Exit;
    }

    if (Address == NULL) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    Index = 0;
    while (BIT (Index) < (Size + sizeof (MEMORY_BLOCK))) {
        Index++;
    }

    if (Index >= SLAB_ALLOCATORS) {
        Status = STATUS_OUT_OF_MEMORY;
        goto Exit;
    }

    Status = SlabAllocate (g_Allocators[Index], Flags, Address);
    if (FAILED (Status)) {
        goto Exit;
    }

    Block = *Address;

    Block->Cookie   = MEMORY_COOKIE;
    Block->Index    = Index;
    Block->Size     = Size;
    Block->Capacity = BIT (Index);

    *Address = (VOID*)(Block + 1);

Exit:
    return Status;
}

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
    )
{
    STATUS        Status;
    UINTN         Index;
    MEMORY_BLOCK* Block;

    Status = STATUS_SUCCESS;

    if (!g_Heap_initialized) {
        Status = STATUS_NOT_INITIALIZED;
        goto Exit;
    }

    if (Address == NULL) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    Block = (MEMORY_BLOCK*)*Address - 1;

    if (Block->Cookie != MEMORY_COOKIE) {
        Status = STATUS_NOT_FOUND;
        goto Exit;
    }

    Index  = Block->Index;
    Status = SlabFree (g_Allocators[Index], Flags, (VOID**)&Block);
    if (FAILED (Status)) {
        goto Exit;
    }

    *Address = NULL;

Exit:
    return Status;
}

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
    )
{
    STATUS        Status;
    UINTN         Index;
    VOID*         NewAddress;
    MEMORY_BLOCK* Block;

    Status = STATUS_SUCCESS;

    if (!g_Heap_initialized) {
        Status = STATUS_NOT_INITIALIZED;
        goto Exit;
    }

    if (Address == NULL) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    Block = (MEMORY_BLOCK*)*Address - 1;

    if (Block->Cookie != MEMORY_COOKIE) {
        Status = STATUS_NOT_FOUND;
        goto Exit;
    }

    Index = 0;
    while (BIT (Index) < (Size + sizeof (MEMORY_BLOCK))) {
        Index++;
    }

    if (Index >= SLAB_ALLOCATORS) {
        Status = STATUS_OUT_OF_MEMORY;
        goto Exit;
    }

    if (Index == Block->Index) {
        Block->Size = Size;
        goto Exit;
    }

    NewAddress = NULL;

    Status = Allocate (&NewAddress, Flags, Size);
    if (FAILED (Status)) {
        goto Exit;
    }

    Status = CopyMemory (NewAddress, Block->Size, *Address, Block->Size);
    if (FAILED (Status)) {
        goto Exit;
    }

    Status = Free (Address, Flags);
    if (FAILED (Status)) {
        goto Exit;
    }

    *Address = NewAddress;

Exit:
    return Status;
}
