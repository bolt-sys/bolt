#pragma once

#include <Base.h>

/*
   Protocols are used to share data between the host and the application.

   The host can provide data to the application and the application can provide data to the host,
   which often can be used to configure another application.

   Every new revision of a protocol must be backwards compatible.
   or the protocol must be renamed and a new GUID must be used.

   The primary entry is the PROTOCOL_STACK, which a pointer will be stored in the RDI register,
   when jumping into the application entry point. It is a contiguous chunk of memory,
   which contains all the protocols that the host has provided to the application.
*/

#define PDF_NONE 0

/**
 * @brief Utility structure for easier access to protocols.
 */
typedef struct PROTOCOL {
    /**
     * @brief The GUID of the protocol.
     */
    GUID   Guid;

    /**
     * @brief The revision of the protocol.
     */
    UINT16 Revision;

    /**
     * @brief Flags for the data.
     * @see PROTOCOL_DATA_FLAGS_*
     */
    UINT8  DataFlags;

    /**
     * @brief Reserved for future use. Must be 0.
     */
    UINT8  Reserved;

    /**
     * @brief The length of the data.
     */
    UINT32 DataLength;

    /**
     * @brief The data of the protocol.
     */
    CHAR8  Data[0]; // Data[DataLength]
} PROTOCOL;

STATIC_ASSERT (sizeof (PROTOCOL) == 24, "Invalid size for PROTOCOL");

typedef struct {
    UINTN Count;
    CHAR8 Protocols[0]; // Protocols[Count]
} PACKED PROTOCOL_STACK;

VOID
SYSAPI
PrintProtocolStack (
    IN CONST PROTOCOL_STACK* ProtocolStack
    );

BOOLEAN
SYSAPI
IterateProtocolStack (
    IN CONST PROTOCOL_STACK* ProtocolStack,
    IN OUT   UINTN*          Index,
    IN OUT   UINTN*          Offset,
    OUT      PROTOCOL**      Protocol
    );

STATUS
SYSAPI
GetProtocol (
    IN  CONST PROTOCOL_STACK* ProtocolStack,
    IN  CONST GUID            Guid,
    OUT       PROTOCOL**      Protocol
    );

STATUS
SYSAPI
GetProtocolData (
    IN  CONST PROTOCOL_STACK* ProtocolStack,
    IN  CONST GUID            Guid,
    OUT       VOID**          Data
    );

#define PROTOCOL_DATA(_X, _PROTOCOL) ((_X*)_PROTOCOL->Data)
