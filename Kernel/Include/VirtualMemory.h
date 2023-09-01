#pragma once

#include <Base.h>

typedef UINTN  PHYSICAL_ADDRESS;
typedef UINTN  VIRTUAL_ADDRESS;

typedef HANDLE VIRTUAL_ADDRESS_SPACE;

#define MEMORY_TYPE_4KB_PAGE            0
#define MEMORY_TYPE_2MB_PAGE            1
#define MEMORY_TYPE_1GB_PAGE            2

#define MEMORY_ATTRIBUTE_NONE           0
#define MEMORY_ATTRIBUTE_READ           BIT(0)
#define MEMORY_ATTRIBUTE_WRITE          BIT(1)
#define MEMORY_ATTRIBUTE_EXEC           BIT(2)
#define MEMORY_ATTRIBUTE_CACHEABLE      BIT(3)
#define MEMORY_ATTRIBUTE_WRITE_COMBINED BIT(4)
#define MEMORY_ATTRIBUTE_WRITE_THROUGH  BIT(5)
#define MEMORY_ATTRIBUTE_WRITE_BACK     BIT(6)

STATUS
SYSAPI
CreateVirtualAddressSpace (
    OUT VIRTUAL_ADDRESS_SPACE* VirtualAddressSpace
    );

STATUS
SYSAPI
DestroyVirtualAddressSpace (
    IN OUT VIRTUAL_ADDRESS_SPACE* VirtualAddressSpace
    );

STATUS
SYSAPI
MapVirtualAddress(
    IN VIRTUAL_ADDRESS_SPACE VirtualAddressSpace,
    IN PHYSICAL_ADDRESS PhysicalAddress,
    IN UINTN                 NumberOfPages,
    IN UINTN Attributes,
    IN UINTN Type,
    IN VIRTUAL_ADDRESS VirtualAddress
);

STATUS
SYSAPI
ActivateVirtualAddressSpace(
    IN VIRTUAL_ADDRESS_SPACE VirtualAddressSpace
);

STATUS
SYSAPI
GetPhysicalAddress(
    IN OPTIONAL VIRTUAL_ADDRESS_SPACE VirtualAddressSpace,
    IN          VIRTUAL_ADDRESS VirtualAddress,
    OUT         PHYSICAL_ADDRESS* PhysicalAddress
    );
