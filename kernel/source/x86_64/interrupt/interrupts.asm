[BITS 64]

extern isr_handler
extern idt_set_entry
%include "misc.inc"

; Push geral purpose registers
; Usage: push_gpr
%macro push_gpr 0
    ; Pushes all general purpose registers
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

; Pop geral purpose registers
; Usage: pop_gpr
%macro pop_gpr 0
    ; Pops all general purpose registers
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro


isr_handler_wrapper:
    push_gpr

    ; Calls the c handler
    mov rdi, rsp
    call isr_handler

    ; Clean up the stack
    add rsp, 8 * 22

    pop_gpr
    add rsp, 8
    sti
    iretq

%macro idt_set 1
    push rdi
    push rsi
    mov rdi, %1
    mov rsi, isr%1
    call idt_set_entry
    pop rsi
    pop rdi
%endmacro

; ISR Macro
; Usage: isr <number>
%macro isr 1
    isr%1:
        cli
        push 0
        push %1
        jmp isr_handler_wrapper
%endmacro

; ISR Handlers

%assign i 0
%rep 256
    isr i
    %assign i i+1
%endrep

beg_func idt_init
    %assign i 0
    %rep 256
        idt_set i
        %assign i i+1
    %endrep
end_func