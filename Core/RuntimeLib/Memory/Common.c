#include <Memory.h>
#include <Error.h>

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
    )
{
    STATUS       Status;
    CONST UINT8* Src;
    UINT8*       Dst;

    Status = STATUS_SUCCESS;

    if ((Source == NULL) || (Destination == NULL)) {
        // Invalid parameter.
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    if (Destination == Source) {
        // Cannot overlap.
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    if (DestinationSize < Length) {
        // Destination buffer is too small.
        Status = STATUS_BUFFER_TOO_SMALL;
        goto Exit;
    }

    if (Length == 0) {
        // Nothing to copy.
        Status = STATUS_SUCCESS;
        goto Exit;
    }

    Src = (CONST UINT8*)Source;
    Dst = (UINT8*)Destination;

    while (Length--) {
        *Dst++ = *Src++;
    }

Exit:
    return Status;
}

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
    )
{
    STATUS Status;
    UINT8* Dst;

    Status = STATUS_SUCCESS;

    if (Destination == NULL) {
        // Invalid parameter.
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    if (Length == 0) {
        // Nothing to fill.
        Status = STATUS_SUCCESS;
        goto Exit;
    }

    Dst = (UINT8*)Destination;

    while (Length--) {
        *Dst++ = Value;
    }

Exit:
    return Status;
}

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
    )
{
    // no need to do anything special, we can just return FillMemory
    return FillMemory (Destination, Length, 0);
}
