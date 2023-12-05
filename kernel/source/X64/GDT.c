#include <Kernel.h>

STATIC GDT  g_GDT = {
    .Null       = GDT_NULL64,
    .KernelCode = GDT_SEGMENT64 (0x0000000000000000,0xFFFFF,  GDT_ACCESS_PRESENT | GDT_ACCESS_SYSTEM | GDT_ACCESS_PRIVILEGE0 | GDT_ACCESS_EXECUTABLE | GDT_ACCESS_READWRITE, GDT_GRANULARITY_4KB | GDT_GRANULARITY_64BIT),
    .KernelData = GDT_SEGMENT64 (0x0000000000000000,0xFFFFF,  GDT_ACCESS_PRESENT | GDT_ACCESS_SYSTEM | GDT_ACCESS_PRIVILEGE0 | GDT_ACCESS_READWRITE | GDT_ACCESS_DIRECTION,  GDT_GRANULARITY_4KB | GDT_GRANULARITY_64BIT),
    .UserCode   = GDT_SEGMENT64 (0x0000000000000000, 0xFFFFF, GDT_ACCESS_PRESENT | GDT_ACCESS_SYSTEM | GDT_ACCESS_PRIVILEGE3 | GDT_ACCESS_EXECUTABLE | GDT_ACCESS_READWRITE, GDT_GRANULARITY_4KB | GDT_GRANULARITY_64BIT),
    .UserData   = GDT_SEGMENT64 (0x0000000000000000, 0xFFFFF, GDT_ACCESS_PRESENT | GDT_ACCESS_SYSTEM | GDT_ACCESS_PRIVILEGE3 | GDT_ACCESS_READWRITE | GDT_ACCESS_DIRECTION,  GDT_GRANULARITY_4KB | GDT_GRANULARITY_64BIT),
    .TSS        = GDT_NULL64,
};

STATIC GDTR g_GDTPtr = {
    .Limit = sizeof (GDT) - 1,
    .Base  = (UINTN)&g_GDT,
};

/**
 * Initialize the GDT
 **/
VOID
SYSAPI
GDTInit (
    VOID
    )
{
    GDTLoad (&g_GDTPtr);

    GDTReloadSegments (GDT_KERNEL_CS, GDT_KERNEL_DS);
}

/**
 * @brief Loads a GDTR
 *
 * @param gdtr A pointer and length to the effective `GDT`
 */
VOID
SYSAPI
GDTLoad (
    GDTR* gdtr
    )
{
    __asm__ volatile (
        "lgdt %0"
        :
        : "m" (*gdtr)
        : "memory"
    );
}

/**
 * @brief Reloads the segments
 *
 * @param CS Code Segment
 * @param DS Data Segment
 **/
VOID
SYSAPI
GDTReloadSegments (
    UINT16 CS,
    UINT16 DS
    )
{
    __asm__ volatile (
        // Data segments
        "movw %0, %%ds\n"
        "movw %0, %%es\n"
        "movw %0, %%fs\n"
        "movw %0, %%gs\n"
        "movw %0, %%ss\n"

                                  // Code segment
        "pushq %2\n"              // Push the code segment
        "leaq 1f(%%rip), %%rax\n" // Get the address of the next instruction
        "pushq %%rax\n"           // Push the address of the next instruction
        "lretq\n"                 // Load the code segment and jump to the next instruction
        "1:\n"
        :
        : "r" (DS), "r" (CS), "i" (GDT_KERNEL_CS)
        : "memory"
    );
}
