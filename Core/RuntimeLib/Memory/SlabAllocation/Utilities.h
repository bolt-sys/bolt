#pragma once

#include <Memory.h>

STATUS
SYSAPI
AllocateEmptySlab (
    IN OUT SLAB_CACHE* Cache
    );

/**
 * @brief Moves a slab from one list to another.
 *
 * @param[in,out] Slab The slab to move.
 * @param[in,out] List The list to move the slab to.
 *
 * @return STATUS_SUCCESS on success.
 **/
STATUS
SYSAPI
MoveSlabToList (
    IN OUT SLAB*  Slab,
    IN OUT SLAB** List
    );

/**
 * @brief Tries to find a slab where an object can be allocated from.
 *
 * @param[in] Cache The cache to allocate from.
 *
 * @return STATUS_SUCCESS   on success.
 * @return STATUS_NOT_FOUND if no slab was found, a new one must be allocated
 *                          using `AllocateEmptySlab`.
 **/
STATUS
SYSAPI
FindSlabForAllocation (
    IN SLAB_CACHE* Cache,
    OUT SLAB**     Slab
    );

/**
 * @brief Find an empty object in a slab
 *
 * @param[in]  Slab   The slab to search
 * @param[out] Bit    The bit of the object within the freemap
 *
 * @return STATUS_SUCCESS              The search was successful
 * @return STATUS_INVALID_PARAMETER    Slab or Object is NULL
 * @return STATUS_NOT_FOUND            No empty object was found
 **/
STATUS
SYSAPI
FindEmptyObjectInSlab (
    IN  SLAB*  Slab,
    OUT UINTN* Bit
    );

/**
 * @brief Allocate an object from a slab
 *
 * @param[in]  Slab   The slab to allocate from
 * @param[in]  Flags  The flags for the allocation
 * @param[out] Object The object to allocate
 *
 * @return STATUS_SUCCESS              The allocation was successful
 * @return STATUS_INVALID_PARAMETER    Object or Slab is NULL
 * @return STATUS_OUT_OF_MEMORY        The slab is full
 *
 **/
STATUS
SYSAPI
AllocateObjectFromSlab (
    IN  SLAB*  Slab,
    IN  UINTN  Flags,
    OUT VOID** Object
    );

STATUS
SYSAPI
FindSlabForObject (
    IN SLAB_CACHE* Cache,
    IN VOID*       Object,
    OUT SLAB**     Slab,
    OUT UINTN*     Bit
    );

STATUS
SYSAPI
FreeObjectInSlab (
    IN OUT SLAB* Slab,
    IN     UINTN Flags,
    IN     UINTN Bit
    );
