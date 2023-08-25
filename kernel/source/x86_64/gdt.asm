bits 64

global load_gdt

; Reload the CS register with a new code segment selector.
;
; This is used to switch to a new code
; segment with a different privilege level.
;
; Inputs:
;  rdi = Code Segment Selector
;
; Outputs:
;  None
gdt_reload_cs:
    ; Load the code segment register
    ; in 64-bit mode, CS is loaded with a far jump
    ; to the same address
    push rdi                     ; Arg1 = Code Segment Selector
    lea rsi, [rel .trampoline]   ; Get the address of the trampoline
    push rsi                     ; Arg2 = Trampoline address
    retfq                        ; Far jump to the trampoline and our new code segment
.trampoline:
    ret

; Reload the DS, ES, FS, GS, and SS registers with a new data segment selector.
;
; This is used to switch to a new data
; segment with a different privilege level.
;
; Inputs:
;  rdi = Data Segment Selector
;
; Outputs:
;  None
gdt_reload_ds:
    ; Load the data segment registers
    mov ds, rdi
    mov es, rdi
    mov fs, rdi
    mov gs, rdi
    mov ss, rdi
    ret

; Load the GDT
;
; Inputs:
;  None
;
; Outputs:
;  None
load_gdt:
    push rbp
    mov rbp, rsp

    ; GDTR
    lea rax, [rel gdt.begin]
    push rax               ; Base address of the GDT
    push word gdt.size - 1 ; Size of the GDT

    ; Load the GDT
    lgdt [rsp]

    ; Reload the segment registers
    mov rdi, gdt.k_ds - gdt.begin
    call gdt_reload_ds

    mov rdi, gdt.k_cs - gdt.begin
    call gdt_reload_cs

    leave
    ret

; -----------------------------------------------------------------------

; The GDT cannot reside in rodata, because it is modified at runtime by the CPU.
; or we get a segfault.

section .data

; helper macro to create a GDT entry
%macro gdt_entry64 4 ; base, limit, access, flags
    ; 32 Bit GDT
    dw %2 & 0xFFFF                              ; Limit lo       0:15
    dw %1 & 0xFFFF                              ; Base lo        16:31
    db (%1 >> 16) & 0xFF                        ; Base mi        32:39
    db %3 & 0xFF                                ; Access Byte    40:47
    db ((%2 >> 16) & 0x0F) | ((%4 & 0x0F) << 4) ; Flags/Limit hi 48:55
    db (%1 >> 24) & 0xFF                        ; Base hi        56:63

    ; 64 Bit GDT
    dd (%1 >> 32) & 0xFFFFFFFF                  ; Base           64:95
    dd 0x00000000                               ; Reserved       96:127
%endmacro

gdt:
  .begin:
    ; Segments
    ;-------------------------------------------------------------------------------------------
    ;                base                limit              access               flags
    ;                                                  P DPL S E DC RW A    G D/B L AVL
    ;-------------------------------------------------------------------------------------------
    dq 0x0000000000000000                                                                      ; Null Segment
    dq 0x0000000000000000                                                                      ;
    ;-------------------------------------------------------------------------------------------
  .k_cs: gdt_entry64 0x0000000000000000, 0xFFFFF,    0b1_00__1_1_0__1__0, 0b1_1___1_0          ; Kernel Code Segment
  .k_ds: gdt_entry64 0x0000000000000000, 0xFFFFF,    0b1_00__1_0_1__1__0, 0b1_1___1_0          ; Kernel Data Segment
    ;-------------------------------------------------------------------------------------------
  .u_cs: gdt_entry64 0x0000000000000000, 0xFFFFF,    0b1_00__1_1_0__1__0, 0b1_1___1_0          ; User Code Segment
  .u_ds: gdt_entry64 0x0000000000000000, 0xFFFFF,    0b1_00__1_0_1__1__0, 0b1_1___1_0          ; User Data Segment
    ;-------------------------------------------------------------------------------------------
    dq 0x0000000000000000                                                                      ; TSS Segment (unused)
    dq 0x0000000000000000                                                                      ;
    ;-------------------------------------------------------------------------------------------
  .end:
  .size equ .end - .begin
