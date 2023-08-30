#pragma once

#include <Base.h>

#define MEMMAP_USABLE                 0
#define MEMMAP_RESERVED               1
#define MEMMAP_ACPI_RECLAIMABLE       2
#define MEMMAP_ACPI_NVS               3
#define MEMMAP_BAD_MEMORY             4
#define MEMMAP_BOOTLOADER_RECLAIMABLE 5
#define MEMMAP_KERNEL_AND_MODULES     6
#define MEMMAP_FRAMEBUFFER            7

typedef struct {
    UINT64 Base;
    UINT64 Length;
    UINT64 Type;
} MEMORY_DESCRIPTOR;

typedef struct {
    // Program Parameters
    UINTN               CommandLineSize;
    CHAR8**             CommandLine;

    // Memory Map
    UINTN               MemoryMapSize;
    MEMORY_DESCRIPTOR** MemoryMap;
} KERNEL_PARAMETERS;

/**
 * @brief The entry point of the Kernel.This function is the entry point for the Kernel.
 *
 * @param[in] Parameters A pointer to a KERNEL_PARAMETERS structure that contains the
 *                       parameters passed to the Kernel by the setup routine.
 *
 * @return This function never returns.
 **/
VOID
SYSAPI
NORETURN
KernelMain (
    KERNEL_PARAMETERS* Parameters
    );

/**
 * @brief Initialize the Bump Allocator
 *
 * @return STATUS_SUCCESS             The initialization was successful
 * @return STATUS_ALREADY_INITIALIZED The Bump Allocator is already initialized
 * @return STATUS_OUT_OF_RESOURCES    The Bump Allocator could not be initialized
 *                                                                    due to lack of memory
 */
STATUS
SYSAPI
MemoryInit (
    KERNEL_PARAMETERS* Parameters
    );
