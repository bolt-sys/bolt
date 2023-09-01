// Initialization of the Kernel, limine specific.

#include <Base.h>
#include <Kernel.h>

#include <limine.h>

// TODO: make this architecture independent
#include <X64/GDT.h>
#include <X64/IDT.h>

STATIC KERNEL_PARAMETERS                             g_Parameters;

STATIC volatile struct limine_kernel_file_request    g_KernelFileRequest    = { .id = LIMINE_KERNEL_FILE_REQUEST, .revision = 0 };
STATIC volatile struct limine_memmap_request         g_MemoryRequest        = { .id = LIMINE_MEMMAP_REQUEST, .revision = 0 };
STATIC volatile struct limine_hhdm_request           g_HHDMRequest          = { .id = LIMINE_HHDM_REQUEST, .revision = 0 };
STATIC volatile struct limine_kernel_address_request g_KernelAddressRequest = { .id = LIMINE_KERNEL_ADDRESS_REQUEST, .revision = 0 };

#define QUICK_CHECK(x) if (FAILED(x)) { goto Exit; }

VOID
StartupRoutine (
    VOID
    )
{
    STATUS Status;

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
    GDTInit ();
    IDTInit ();

    QUICK_CHECK (MemoryInit (&g_Parameters));
    QUICK_CHECK (HeapInit ());

    //
    // Enter the Kernel
    //
    KernelMain (&g_Parameters);

Exit:
    //
    // We fucked up.
    // TODO: Print to serial that we fucked up and why..
    //
    DisableInterrupts ();

    while (TRUE) {
        __asm__ volatile ("hlt");
    }
}
