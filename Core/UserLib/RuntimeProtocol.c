#include <RuntimeProtocol.h>

BOOLEAN
SYSAPI
IterateProtocolStack (
    IN CONST PROTOCOL_STACK* ProtocolStack,
    IN OUT   UINTN*          Index,
    IN OUT   UINTN*          Offset,
    OUT      PROTOCOL**      Protocol
    )
{
    if (*Index >= ProtocolStack->Count) {
        return FALSE;
    }

    if (*Index == 0) {
        *Offset = sizeof (PROTOCOL_STACK);
    }

    *Protocol = (PROTOCOL*)((UINT8*)ProtocolStack + *Offset);
    *Offset  += sizeof (PROTOCOL) + (*Protocol)->DataLength;
    ++*Index;

    return TRUE;
}

STATUS
SYSAPI
GetProtocol (
    IN  CONST PROTOCOL_STACK* ProtocolStack,
    IN  CONST GUID            Guid,
    OUT       PROTOCOL**      Protocol
    )
{
    UINTN Index  = 0;
    UINTN Offset = 0;

    while (IterateProtocolStack (ProtocolStack, &Index, &Offset, Protocol)) {
        if (GUID_EQ ((*Protocol)->Guid, Guid)) {
            return STATUS_SUCCESS;
        }
    }

    *Protocol = NULL;
    return STATUS_NOT_FOUND;
}

STATUS
SYSAPI
GetProtocolData (
    IN  CONST PROTOCOL_STACK* ProtocolStack,
    IN  CONST GUID            Guid,
    OUT       VOID**          Data
    )
{
    PROTOCOL* Protocol;
    STATUS    Status;

    Status = GetProtocol (ProtocolStack, Guid, &Protocol);
    if (FAILED (Status)) {
        return Status;
    }

    *Data = Protocol->Data;
    return STATUS_SUCCESS;
}
