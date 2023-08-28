#include "Base.h"
#include "Kernel.h"

/**
 * @brief The entry point of the Kernel.This function is the entry point for the Kernel.
 *
 * @param[in] Parameters A pointer to a KERNEL_PARAMETERS structure that contains the
 *                                           parameters passed to the Kernel by the setup routine.
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
	UNUSED(Parameters);

	while (TRUE) {
		__asm__ volatile ("hlt");
	}
}
