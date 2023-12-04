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

EXTERN_C_END
