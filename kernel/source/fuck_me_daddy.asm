[BITS 64]
global gdt_flush

gdt_flush:
    ; Reload CS register:
    push 0x08                 ; Push code segment to stack, 0x08 is a stand-in for your code segment
    lea RAX, [rel .reload_CS] ; Load address of .reload_CS into RAX
    push RAX                  ; Push this value to the stack
    retfq                     ; Perform a far return, retfq or LretQ depending on syntax
.reload_CS:
    ; Reload data segment registers
    mov   AX, 0x10 ; 0x10 is a stand-in for your data segment
    mov   DS, AX
    mov   ES, AX
    mov   FS, AX
    mov   GS, AX
    mov   SS, AX
    ret