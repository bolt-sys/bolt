[BITS 64]

extern isr_handler
extern idt_set_entry
%include "misc.inc"

; Push geral purpose registers
; Usage: push_gpr
%macro push_gpr 0
    ; Pushes all general purpose registers
    push rax      ;save current rax
    push rbx      ;save current rbx
    push rcx      ;save current rcx
    push rdx      ;save current rdx
    push rbp      ;save current rbp
    push rdi      ;save current rdi
    push rsi      ;save current rsi
    push r8         ;save current r8
    push r9         ;save current r9
    push r10      ;save current r10
    push r11      ;save current r11
    push r12      ;save current r12
    push r13      ;save current r13
    push r14      ;save current r14
    push r15      ;save current r15
%endmacro

; Pop geral purpose registers
; Usage: pop_gpr
%macro pop_gpr 0
    ; Pops all general purpose registers
    pop r15         ;restore current r15
    pop r14         ;restore current r14
    pop r13         ;restore current r13
    pop r12         ;restore current r12
    pop r11         ;restore current r11
    pop r10         ;restore current r10
    pop r9          ;restore current r9
    pop r8          ;restore current r8
    pop rsi         ;restore current rsi
    pop rdi         ;restore current rdi
    pop rbp         ;restore current rbp
    pop rdx         ;restore current rdx
    pop rcx         ;restore current rcx
    pop rbx         ;restore current rbx
    pop rax         ;restore current rax
%endmacro

isr_handler_wrapper:
    ; push ds
    xor rax, rax
    mov ax, ds
    push rax
    ; Push the stack
    push_gpr

    ; Calls the c handler
    mov rdi, rsp
    push rsp
    call isr_handler
    add rsp, 4

    ; Clean up the stack
    pop rax
    pop_gpr
    add rsp, 8
    sti
    iret

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