#include "Base.h"
#include "Kernel.h"
#include "InternalMemory.h"

/**
 * @brief The entry point of the Kernel.This function is the entry point for the Kernel.
 *
 * @param[in] Parameters A pointer to a KERNEL_PARAMETERS structure that contains the
 *                       parameters passed to the Kernel by the setup routine.
 *
 * @return This function never returns.
 **/
NORETURN
SYSAPI
VOID
KernelMain (
    KERNEL_PARAMETERS* Parameters
    )
{
    UNUSED (Parameters);
    
    KernelBuddyAllocatorInit(Parameters);
    void* test;
    void* test2;
    STATUS s = KernelBuddyAllocate(0x2001, &test);
    STATUS s2 = KernelBuddyAllocate(0x1000, &test2);
    // :)
    KernelBuddyFree(test2);
    KernelBuddyFree(test);
    while (TRUE) {
        __asm__ volatile ("hlt");
    }
}
