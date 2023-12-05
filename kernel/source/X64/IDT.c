#include <Kernel.h>

IDT_ENTRY64  g_IDT[IDT_ENTRY_COUNT] = { 0 };

UINT8        g_InterruptGates[32] = {
    IDT_TYPE_TRAP_GATE,          // Divide by zero
    IDT_TYPE_TRAP_GATE,          // Debug
    IDT_TYPE_INTERRUPT_GATE,     // Non maskable interrupt
    IDT_TYPE_TRAP_GATE,          // Breakpoint
    IDT_TYPE_TRAP_GATE,          // Overflow
    IDT_TYPE_TRAP_GATE,          // Bound range exceeded
    IDT_TYPE_TRAP_GATE,          // Invalid opcode
    IDT_TYPE_TRAP_GATE,          // Device not available
    IDT_TYPE_TRAP_GATE,          // Double fault
    IDT_TYPE_TRAP_GATE,          // Coprocessor segment overrun
    IDT_TYPE_TRAP_GATE,          // Invalid TSS
    IDT_TYPE_TRAP_GATE,          // Segment not present
    IDT_TYPE_TRAP_GATE,          // Stack segment fault
    IDT_TYPE_TRAP_GATE,          // General protection fault
    IDT_TYPE_TRAP_GATE,          // Page fault
    0,                           // Reserved
    IDT_TYPE_TRAP_GATE,          // x87 floating point exception
    IDT_TYPE_TRAP_GATE,          // Alignment check
    IDT_TYPE_TRAP_GATE,          // Machine check
    IDT_TYPE_TRAP_GATE,          // SIMD floating point exception
    IDT_TYPE_TRAP_GATE,          // Virtualization exception
    0,                           // Reserved
    0,                           // Reserved
    0,                           // Reserved
    0,                           // Reserved
    0,                           // Reserved
    0,                           // Reserved
    0,                           // Reserved
    0,                           // Reserved
    0,                           // Reserved
    0,                           // Reserved
    0,                           // Reserved
};

EXTERN UINTN ISRHandlers[IDT_ENTRY_COUNT];

/**
 * @brief Initializes the IDT.
 **/
VOID
SYSAPI
IDTInit (
    VOID
    )
{
    UINT8 InterruptGate;

    // Disable interrupts while we're setting up the IDT
    // we cannot handle interrupts at this stage.
    DisableInterrupts ();

    // Before we can do anything, we need to populate the ISR Handlers itself.
    PopulateISRHandlers ();

    for (UINTN i = 0; i < IDT_ENTRY_COUNT; ++i) {
        InterruptGate = IDT_TYPE_INTERRUPT_GATE;

        if (i < 32) {
            InterruptGate = g_InterruptGates[i];
        }

        g_IDT[i].Low.BaseLow       = ISRHandlers[i] & 0xFFFF;
        g_IDT[i].Low.Selector      = GDT_KERNEL_CS;
        g_IDT[i].Low.IST           = 0;
        g_IDT[i].Low.TypeAttribute = InterruptGate;
        g_IDT[i].Low.BaseHigh      = (ISRHandlers[i] >> 16) & 0xFFFF;
        g_IDT[i].BaseHighExtended  = (ISRHandlers[i] >> 32) & 0xFFFFFFFF;
        g_IDT[i].Reserved          = 0;
    }

    IDTR IDTR = {
        .Limit = sizeof (g_IDT) - 1,
        .Base  = (UINTN)g_IDT
    };

    IDTLoad (&IDTR);

    EnableInterrupts ();

    // Trigger a breakpoint exception
    __asm__ VOLATILE ("int3");
    __asm__ VOLATILE ("int3");
    __asm__ VOLATILE ("int3");
    __asm__ VOLATILE ("int3");
    __asm__ VOLATILE ("int3");
    __asm__ VOLATILE ("int3");
    __asm__ VOLATILE ("int3");
    __asm__ VOLATILE ("int3");
}

/**
 * @brief The C part of the ISR handler.
 *
 * @param[in] Frame The interrupt frame.
 **/
VOID
SYSAPI
ISRHandler (
    IN INTERRUPT_FRAME* Frame
    )
{
    UNUSED (Frame);

    // print "hello world" to serial port
    const char* serial_message = "X";

    __asm__ VOLATILE (
              "movq $0x3F8, %%rdx\n"
              "movb %0, %%al\n"
              "outb %%al, %%dx\n"
              :
              : "m" (*serial_message)
              : "memory"
              );
}
