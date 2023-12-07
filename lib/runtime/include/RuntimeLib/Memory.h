/**
 * @file Memory.h
 * @brief This file contains common memory operations such as copying, filling, and zeroing memory buffers.
 *
 * This file should be included from RuntimeLib.h
 */
#pragma once

#ifndef __RUNTIME_LIB_H__
    #error "This file should be included from RuntimeLib.h"
#endif

EXTERN_C_START

// -------------------------------------------------- Structures and Enums

typedef struct _PAGE_ALLOCATOR PAGE_ALLOCATOR;

/**
 * @brief Allocate a range of pages
 *
 * @param[in]  Self    The allocator to use
 * @param[out] Address The address of the allocated memory
 * @param[in]  Pages   The amount of pages to allocate
 * @param[in]  Align   The alignment of the allocation
 *                     must be (PAGE_SIZE_4K, PAGE_SIZE_2M, PAGE_SIZE_1G)
 *
 * @return STATUS_SUCCESS             The allocation was successful
 * @return STATUS_INVALID_PARAMETER   Address is NULL or Pages is 0
 * @return STATUS_INVALID_ALIGNMENT   Align is not a valid page size
 * @return STATUS_OUT_OF_MEMORY       The allocation failed
 * @return STATUS_NOT_INITIALIZED     The Allocator is not initialized
 **/
typedef
STATUS
(* PA_ALLOCATE_PAGES)(
    IN PAGE_ALLOCATOR* Self,
    OUT VOID**         Address,
    IN  UINTN          Pages,
    IN  UINTN          Align
    );

/**
 * @brief Free a range of pages
 *
 * after the page has been freed, *Address will be set to NULL.
 *
 * @param[in]     Self    The allocator to use
 * @param[in,out] Address The address of the allocated memory
 * @param[in]     Pages   The amount of pages to free
 *
 * @return STATUS_SUCCESS              The free was successful
 * @return STATUS_INVALID_PARAMETER    Address is NULL or Pages is 0
 * @return STATUS_INVALID_ALIGNMENT    The address is not page aligned
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
typedef
STATUS
(* PA_FREE_PAGES)(
    IN     PAGE_ALLOCATOR* Self,
    IN OUT VOID**          Address,
    IN     UINTN           Pages
    );

// A simple allocators struct must look like this:
//
// typedef struct _MY_PAGE_ALLOCATOR {
//      PA_ALLOCATE_PAGES AllocatePages;
//      PA_FREE_PAGES FreePages;
//
//      ... implementation specific ...
// } MY_ALLOCATOR;
//
typedef struct _PAGE_ALLOCATOR {
    PA_ALLOCATE_PAGES AllocatePages;
    PA_FREE_PAGES     FreePages;
} PAGE_ALLOCATOR;

// Helper macros to make life easier.

/**
 * @brief Allocates pages using the given page allocator.
 *
 * This macro calls the AllocatePages function of the given page allocator
 * to allocate the specified number of pages with the given alignment.
 *
 * @param[in]  _Allocator  The page allocator to use for allocation.
 * @param[out] _Address    Pointer to the allocated memory address.
 * @param[in]  _Pages      Number of pages to allocate.
 * @param[in]  _Align      Alignment of the allocated memory.
 *
 * @return Status code from the allocator's AllocatePages function.
 */
#define PA_ALLOCATE(_Allocator, _Address, _Pages, _Align) \
    ((PAGE_ALLOCATOR*)_Allocator)->AllocatePages (((PAGE_ALLOCATOR *)(_Allocator)), (_Address), (_Pages), (_Align))

/**
 * @brief Frees pages using the given page allocator.
 *
 * This macro calls the FreePages function of the given page allocator
 * to free the specified number of pages starting at the given address.
 *
 * @param[in] _Allocator  The page allocator to use for freeing.
 * @param[in] _Address    Starting address of memory range to free.
 * @param[in] _Pages      Number of pages to free.
 *
 * @return Status code from the allocator's FreePages function.
 */
#define PA_FREE(_Allocator, _Address, _Pages) \
    ((PAGE_ALLOCATOR*)_Allocator)->FreePages (((PAGE_ALLOCATOR *)(_Allocator)), (_Address), (_Pages))

// ------------------------------------------------------------------ Constants

#define PAGE_SIZE_4K 0x1000
#define PAGE_SIZE_2M 0x200000
#define PAGE_SIZE_1G 0x40000000

// --------------------------------------------------- Common Memory Operations

/**
 * @brief Copies a source buffer to a destination buffer.
 *
 * This function copies Length bytes from the Source buffer to the Destination buffer.
 * The caller must ensure that the Destination buffer is large enough to hold the
 * copied data, or else the function will return STATUS_BUFFER_TOO_SMALL.
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
RtlCopyMemory (
    IN        VOID* Destination,
    IN        UINTN DestinationSize,
    IN CONST VOID*  Source,
    IN        UINTN Length
    );

/**
 * @brief Fills a buffer with a specified byte value.
 *
 * This function fills the first Length bytes of the Destination buffer with the
 * specified Value.
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
RtlFillMemory (
    IN VOID* Destination,
    IN UINTN Length,
    IN UINT8 Value
    );

/**
 * @brief Zeroes a buffer.
 *
 * This function fills the first Length bytes of the Destination buffer with zeroes.
 * This is equivalent to calling FillMemory with a Value of 0.
 *
 * @param[in] Destination           The destination buffer to zero.
 * @param[in] Length                The number of bytes in Destination to zero.
 *
 * @return @see RtlFillMemory
 **/
STATUS
SYSAPI
RtlZeroMemory (
    IN VOID* Destination,
    IN UINTN Length
    );

// --------------------------------------------------------------- Sub-includes

#include "Memory/BumpAllocator.h"
#include "Memory/SlabAllocator.h"

EXTERN_C_END
