#include <RuntimeLib.h>
#include <Kernel.h>

VOID
StartupRoutine (
    VOID
    )
{
    // TODO: pass the correct arguments
    // TODO: init architecture
    GDTInit ();

    KernelMain ();
}
