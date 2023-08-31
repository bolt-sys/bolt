#pragma once

#include <Base.h>

typedef UINTN  PHYSICAL_ADDRESS;
typedef UINTN  VIRTUAL_ADDRESS;

typedef HANDLE VIRTUAL_ADDRESS_SPACE;

#define MEMORY_TYPE_NONE      0
#define MEMORY_TYPE_FREE      1
#define MEMORY_TYPE_RESERVED  2

#define MEMORY_ATTRIBUTE_NONE  0
#define MEMORY_ATTRIBUTE_READ  BIT(0)
#define MEMORY_ATTRIBUTE_WRITE BIT(1)
#define MEMORY_ATTRIBUTE_EXEC  BIT(2)

STATUS
SYSAPI
CreateVirtualAddressSpace (
    OUT VIRTUAL_ADDRESS_SPACE* MemoryMap
    );

STATUS
SYSAPI
DestroyVirtualAddressSpace (
    IN OUT VIRTUAL_ADDRESS_SPACE* MemoryMap
    );

STATUS
SYSAPI
MapMemory (
    IN VIRTUAL_ADDRESS_SPACE MemoryMap,
    IN PHYSICAL_ADDRESS      SourceAddress,
    IN VIRTUAL_ADDRESS       DestinationAddress,
    IN UINTN                 NumberOfPages,
    IN UINTN                 Flags
    );
