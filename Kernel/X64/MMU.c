#include "VirtualMemory.h"

#include <Base.h>

typedef UINTN PML4E;
typedef UINTN PDPTE;
typedef UINTN PDE;
typedef UINTN PTE;

typedef PTE   PT[512];
typedef PDE   PD[512];
typedef PDPTE PDPT[512];
typedef PML4E PML4[512];

STATIC_ASSERT (sizeof (PT)   == PAGE_SIZE_4K, "Invalid size for PT");
STATIC_ASSERT (sizeof (PD)   == PAGE_SIZE_4K, "Invalid size for PD");
STATIC_ASSERT (sizeof (PDPT) == PAGE_SIZE_4K, "Invalid size for PDPT");
STATIC_ASSERT (sizeof (PML4) == PAGE_SIZE_4K, "Invalid size for PML4");

#define MAXIMUM_PHYSICAL_ADDRESS 0x000FFFFFFFFFF000ULL // 52 bits

//  CR3   - Control Register for Paging.
// ----------------------------------------
//  0-2   - Ignored
//    3   - Page-level write-through
//    4   - Page-level cache disable
//  5-11  - Ignored
// 12-51  - Physical address of PML4 table
// 52-63  - Ignored
#define CR3_PWT (1 << 3)
#define CR3_PCD (1 << 4)
#define CR3_PML4_ADDRESS(_X) ((_X) & MAXIMUM_PHYSICAL_ADDRESS)

//  Virtual Address Format (for 4-level paging, 4KB pages)
//  See Intel SDM Vol 3A, Section 4.5.4, Figure 4-8
// --------------------------------------------------------
//  0-11  - Offset within page
// 12-20  - Offset within page table
// 21-29  - Offset within page directory
// 30-38  - Offset within page directory pointer table
// 39-47  - Offset within PML4 table
// 48-63  - User-defined
#define CREATE_VIRTUAL_ADDRESS_L4_4K(_USER, _PML4, _PDPT, _PD, _PT, _OFFSET) \
    (((_USER) << 48) | ((_PML4) << 39) | ((_PDPT) << 30) | ((_PD) << 21) | ((_PT) << 12) | (_OFFSET))

//  Virtual Address Format (for 4-level paging, 2MB pages)
//  See Intel SDM Vol 3A, Section 4.5.4, Figure 4-9
// --------------------------------------------------------
//  0-20  - Offset within page
// 21-29  - Offset within page directory
// 30-38  - Offset within page directory pointer table
// 39-47  - Offset within PML4 table
// 48-63  - User-defined
#define CREATE_VIRTUAL_ADDRESS_L4_2M(_USER, _PML4, _PDPT, _PD, _OFFSET) \
    CREATE_VIRTUAL_ADDRESS_L4_4K (_USER, _PML4, _PDPT, _PD, (_OFFSET) >> 12, (_OFFSET) & 0xFFF)

//  Virtual Address Format (for 4-level paging, 1GB pages)
//  See Intel SDM Vol 3A, Section 4.5.4, Figure 4-10
// --------------------------------------------------------
//  0-29  - Offset within page
// 30-38  - Offset within page directory pointer table
// 39-47  - Offset within PML4 table
// 48-63  - User-defined
#define CREATE_VIRTUAL_ADDRESS_L4_1G(_USER, _PML4, _PDPT, _OFFSET) \
    CREATE_VIRTUAL_ADDRESS_L4_2M (_USER, _PML4, _PDPT, (_OFFSET) >> 21, (_OFFSET) & 0x1FFFFF)

// Utilities for getting information from a virtual address
#define USER_DEFINED(_X) (((_X) >> 48) & 0xFFFF)
#define PML4_INDEX(_X)   (((_X) >> 39) & 0x1FF)
#define PDPT_INDEX(_X)   (((_X) >> 30) & 0x1FF)
#define PD_INDEX(_X)     (((_X) >> 21) & 0x1FF)
#define PT_INDEX(_X)     (((_X) >> 12) & 0x1FF)
#define OFFSET(_X)       ((_X) & 0xFFF)

//  Page Map Level 4 Entry (PML4E)
//  See Intel SDM Vol 3A, Section 4.5.4, Table 4-15.
// ----------------------------------------
//     0 - Present
//     1 - Read/Write
//     2 - User/Supervisor
//     3 - Page-level write-through
//     4 - Page-level cache disable
//     5 - Accessed (set by CPU)
//     6 - Ignored
//     7 - Reserved (must be 0)
//  8-10 - Ignored
//    11 - Ignored (only used by HLAT)
// 12-50 - Physical address of 512GB page directory pointer table
//    51 - Reserved (must be 0)
// 52-62 - Ignored
//    63 - Execute Disable (if 1, instruction fetches are not allowed from the 512GB region controlled by this entry)

#define PML4E_PRESENT         BIT(0)
#define PML4E_READ_WRITE      BIT(1)
#define PML4E_USER_SUPERVISOR BIT(2)
#define PML4E_PWT             BIT(3)
#define PML4E_PCD             BIT(4)
#define PML4E_ACCESSED        BIT(5)
#define PML4E_RESERVED        BIT(7)
#define PML4E_RESTART         BIT(11)
#define PML4E_PDPT_ADDRESS(_X) ((_X) & MAXIMUM_PHYSICAL_ADDRESS)
#define PML4E_EXECUTE_DISABLE BIT(63)

#define PML4E_USER_DEFINED(_DATA) \
    (((_DATA) & 0x3FF) << 52)

//  Page Directory Pointer Table Entry (PDPTE)
//  See Intel SDM Vol 3A, Section 4.5.4, Table 4-16 & 4-17.
// ----------------------------------------
//     0 - Present
//     1 - Read/Write
//     2 - User/Supervisor
//     3 - Page-level write-through
//     4 - Page-level cache disable
//     5 - Accessed (set by CPU)
//     6 - Dirty                                                                ; Ignored if Page Size = 0
//     7 - Page size (1; 1GB page, 0; Reference to Page Directory)
//     8 - Global if CR4.PGE = 1, ignored otherwise                             ; Ignored if Page Size = 0
//     9 - Ignored
//    10 - Ignored
//    11 - Ignored (only used by HLAT)
//    12 - Memory Typ                                                           ; Part of the physical address if Page Size = 0
// 13-29 - Reserved (must be 0)                                                 ; Part of the physical address if Page Size = 0
// 30-50 - Physical address of 1GB page or 4KB page directory
// 51-52 - Reserved (must be 0)
// 53-58 - Ignored
// 59-62 - Protection Key, if CR4.PKE = 1, ignored otherwise                    ; Ignored if Page Size = 0
//    63 - Execute Disable (if 1, instruction fetches are not allowed from the 1GB region controlled by this entry)

#define PDPTE_PRESENT         BIT(0)
#define PDPTE_READ_WRITE      BIT(1)
#define PDPTE_USER_SUPERVISOR BIT(2)
#define PDPTE_PWT             BIT(3)
#define PDPTE_PCD             BIT(4)
#define PDPTE_ACCESSED        BIT(5)
#define PDPTE_DIRTY           BIT(6)
#define PDPTE_PAGE_SIZE       BIT(7)
#define PDPTE_GLOBAL          BIT(8)
#define PDPTE_MEMORY_TYPE     BIT(12)                       // Only used if Page Size = 0
#define PDPTE_PD_ADDRESS(_X)      ((_X) & MAXIMUM_PHYSICAL_ADDRESS)
#define PDPTE_PROTECTION_KEY(_X)  (((_X) & 0x1F) << 59)     // Only used if Page Size = 0
#define PDPTE_EXECUTE_DISABLE BIT(63)
#define PDPTE_USER_DEFINED(_DATA) (((_DATA) & 0x3FF) << 52) // Only used if Page Size = 0

// Page Directory Entry (PDE)
// See Intel SDM Vol 3A, Section 4.5.4, Table 4-18 & 4-19.
// ----------------------------------------
//     0 - Present
//     1 - Read/Write
//     2 - User/Supervisor
//     3 - Page-level write-through
//     4 - Page-level cache disable
//     5 - Accessed (set by CPU)
//     6 - Dirty                                                                ; Ignored if Page Size = 0
//     7 - Page size (1; 2MB page, 0; Reference to Page Table)
//     8 - Global if CR4.PGE = 1, ignored otherwise                             ; Ignored if Page Size = 0
//     9 - Ignored
//    10 - Ignored
//    11 - Ignored (only used by HLAT)
//    12 - Memory Typ                                                           ; Part of the physical address if Page Size = 0
// 13-29 - Reserved (must be 0)                                                 ; Part of the physical address if Page Size = 0
// 30-50 - Physical address of 2MB page or 4KB page table
// 51-52 - Reserved (must be 0)
// 53-58 - Ignored
// 59-62 - Protection Key, if CR4.PKE = 1, ignored otherwise                    ; Ignored if Page Size = 0
//    63 - Execute Disable (if 1, instruction fetches are not allowed from the 2MB region controlled by this entry)

#define PDE_PRESENT         BIT(0)
#define PDE_READ_WRITE      BIT(1)
#define PDE_USER_SUPERVISOR BIT(2)
#define PDE_PWT             BIT(3)
#define PDE_PCD             BIT(4)
#define PDE_ACCESSED        BIT(5)
#define PDE_DIRTY           BIT(6)
#define PDE_PAGE_SIZE       BIT(7)
#define PDE_GLOBAL          BIT(8)
#define PDE_MEMORY_TYPE     BIT(12)                       // Only used if Page Size = 0
#define PDE_PT_ADDRESS(_X)      ((_X) & MAXIMUM_PHYSICAL_ADDRESS)
#define PDE_PROTECTION_KEY(_X)  (((_X) & 0x1F) << 59)     // Only used if Page Size = 0
#define PDE_EXECUTE_DISABLE BIT(63)
#define PDE_USER_DEFINED(_DATA) (((_DATA) & 0x3FF) << 52) // Only used if Page Size = 0

// Page Table Entry (PTE)
// See Intel SDM Vol 3A, Section 4.5.4, Table 4-20.
// ----------------------------------------
//     0 - Present
//     1 - Read/Write
//     2 - User/Supervisor
//     3 - Page-level write-through
//     4 - Page-level cache disable
//     5 - Accessed (set by CPU)
//     6 - Dirty
//     7 - Memory Typ
//     8 - Global if CR4.PGE = 1, ignored otherwise
//     9 - Ignored
//    10 - Ignored
//    11 - Ignored (only used by HLAT)
// 12-51 - Physical address of 4KB page
// 52-58 - Ignored
// 59-62 - Protection Key, if CR4.PKE = 1, ignored otherwise
//    63 - Execute Disable (if 1, instruction fetches are not allowed from the 4KB region controlled by this entry)

#define PTE_PRESENT         BIT(0)
#define PTE_READ_WRITE      BIT(1)
#define PTE_USER_SUPERVISOR BIT(2)
#define PTE_PWT             BIT(3)
#define PTE_PCD             BIT(4)
#define PTE_ACCESSED        BIT(5)
#define PTE_DIRTY           BIT(6)
#define PTE_MEMORY_TYPE     BIT(7)
#define PTE_GLOBAL          BIT(8)
#define PTE_P_ADDRESS(_X)       ((_X) & MAXIMUM_PHYSICAL_ADDRESS)
#define PTE_PROTECTION_KEY(_X)  (((_X) & 0x1F) << 59)
#define PTE_EXECUTE_DISABLE BIT(63)
#define PTE_USER_DEFINED(_DATA) (((_DATA) & 0x3FF) << 52)

STATUS
SYSAPI
CreateVirtualAddressSpace (
    OUT VIRTUAL_ADDRESS_SPACE* MemoryMap
    )
{
    STATUS Status;
    PML4*  PML4;

    Status = STATUS_SUCCESS;

    if (MemoryMap == NULL) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    Status = AllocatePages ((VOID*)&PML4, 1, PAGE_SIZE_4K);
    if (FAILED (Status)) {
        goto Exit;
    }

Exit:
    return Status;
}
