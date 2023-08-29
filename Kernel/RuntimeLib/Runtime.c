// Initialization of the Kernel, limine specific.

#include <Base.h>
#include <Kernel.h>

#include <limine.h>

#include <X64/GDT.h>

STATIC KERNEL_PARAMETERS                          g_Parameters;

STATIC volatile struct limine_kernel_file_request g_KernelFileRequest = { .id = LIMINE_KERNEL_FILE_REQUEST, .revision = 0 };
STATIC volatile struct limine_memmap_request      g_MemoryRequest     = { .id = LIMINE_MEMMAP_REQUEST, .revision = 0 };

VOID
_start (
    VOID
    )
{
    // TODO: split this into multiple command line variables
    g_Parameters.CommandLineSize = 1;
    g_Parameters.CommandLine     = (CHAR8**)g_KernelFileRequest.response->kernel_file->cmdline;

    //
    // Initialize the memory map
    //
    g_Parameters.MemoryMapSize = g_MemoryRequest.response->entry_count;
    g_Parameters.MemoryMap     = (MEMORY_DESCRIPTOR**)g_MemoryRequest.response->entries;

    //
    // Initialize core Kernel functionality
    //
    MemoryInit (&g_Parameters);
    GDTInit ();

    //
    // Enter the Kernel
    //
    KernelMain (&g_Parameters);
}
