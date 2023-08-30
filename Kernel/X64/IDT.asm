extern ISRHandler

ISRCommonHandler:
    ; Save all the registers, we can pass them to the C ISR Handler.
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rdi
    push rsi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    ; Save the ds/es registers.
    xor rax, rax

    mov ax, ds
    push rax

    mov ax, es
    push rax

    ; Call into the C ISR Handler.
    mov rdi, rsp
    call ISRHandler

    ; Restore es/ds incase we swapped cpl
    pop rax
    mov es, ax

    pop rax
    mov ds, ax

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax

    add rsp, 16 ; Adjust the stack (we pushed error code and vector number)
    sti
    iretq

%macro isr 1
    isr%1:
        cli ; Disable interrupts, we don't want to be interrupted while handling an interrupt.

        %if %1 != 8 || %1 != 10 || %1 != 11 || %1 != 12 || %1 != 13 || %1 != 14 || %1 != 17 || %1 != 21
            push 0 ; we don't have an error code and require one.
        %endif

        push %1 ; Push the vector number.

        jmp ISRCommonHandler ; Keep going into the common handler.
%endmacro

; Register all the ISRs.
%assign i 0
%rep 256
    isr i
    %assign i i+1
%endrep

;
; @brief Loads an IDTR.
;
; @param[in] RDI - The IDTR to load.
;
; @return This function does not return anything.
;
global IDTLoad
IDTLoad:
    lidt [rdi]
    ret

;
; @brief Disables interrupts.
;
; @return This function does not return anything.
;
global DisableInterrupts
DisableInterrupts:
    cli
    ret

;
; @brief Enables interrupts.
;
; @return This function does not return anything.
;
global EnableInterrupts
EnableInterrupts:
    sti
    ret

global ISRHandlers

[section .rodata]
    %macro __ISRHandler 1
        dq isr%1
    %endmacro

    ; All ISRx handlers can be accessed by the following array.
    ISRHandlers:
        %assign i 0
        %rep 256
            __ISRHandler i
            %assign i i+1
        %endrep
