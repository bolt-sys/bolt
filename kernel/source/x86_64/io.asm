bits 64

%include "misc.inc"

; Put a byte into an I/O port.
;
; inputs:
;   di  - I/O port
;   si  - byte to put
;
; outputs:
;   none
;
; C equivalent: void outb(uint16_t port, uint8_t value);
;
beg_func outb
    mov ax, si ; Arg0 (Byte)
    mov dx, di ; Arg1 (Port)

    out dx, al
end_func

; Put a word into an I/O port.
;
; inputs:
;   di  - I/O port
;   si  - word to put
;
; outputs:
;   none
;
; C equivalent: void outw(uint16_t port, uint16_t value);
;
beg_func outw
    mov ax, si ; Arg0 (Word)
    mov dx, di ; Arg1 (Port)

    out dx, ax
end_func

; Put a dword into an I/O port.
;
; inputs:
;   di   - I/O port
;   esi  - dword to put
;
; outputs:
;   none
;
; C equivalent: void outl(uint16_t port, uint32_t value);
;
beg_func outd
    mov eax, esi ; Arg0 (Dword)
    mov dx, di   ; Arg1 (Port)

    out dx, eax
end_func

; ---------------------------------------------------------------------------

; Read a byte from an I/O port.
;
; inputs:
;   di - I/O port
;
; outputs:
;   al - byte read
;
; C equivalent: uint8_t inb(uint16_t port)
;
beg_func inb
    mov dx, di
    in al, dx
end_func

; Read a word from an I/O port.
;
; inputs:
;   di - I/O port
;
; outputs:
;   ax - word read
;
; C equivalent: uint16_t inw(uint16_t port)
;
beg_func inw
    mov dx, di
    in ax, dx
end_func

; Read a dword from an I/O port.
;
; inputs:
;   di - I/O port
;
; outputs:
;   eax - dword read
;
; C equivalent: uint32_t ind(uint16_t port)
;
beg_func ind
    mov dx, di
    in eax, dx
end_func

; ---------------------------------------------------------------------------

; Put a sequence of bytes into an I/O port.
;
; inputs:
;   di   - I/O port
;   rsi  - pointer to bytes to put
;   rdx  - number of bytes to put
;
; outputs:
;   none
;
; C equivalent: void outsb(uint16_t port, const void *addr, uint64_t count);
;
beg_func outsb
    mov rcx, rdx ; Arg2 (Count)
    mov dx,   di ; Arg0 (Port)
    mov rsi, rsi ; Arg1 (Pointer)

    rep outsb
end_func

; Put a sequence of words into an I/O port.
;
; inputs:
;   di   - I/O port
;   rsi  - pointer to words to put
;   rdx  - number of words to put
;
; outputs:
;   none
;
; C equivalent: void outsw(uint16_t port, const void *addr, uint64_t count);
;
beg_func outsw
    mov rcx, rdx ; Arg2 (Count)
    mov dx,   di ; Arg0 (Port)
    mov rsi, rsi ; Arg1 (Pointer)

    rep outsw
end_func

; Put a sequence of dwords into an I/O port.
;
; inputs:
;   di   - I/O port
;   rsi  - pointer to dwords to put
;   rdx  - number of dwords to put
;
; outputs:
;   none
;
; C equivalent: void outsd(uint16_t port, const void *addr, uint64_t count);
;
beg_func outsd
    mov rcx, rdx ; Arg2 (Count)
    mov dx,   di ; Arg0 (Port)
    mov rsi, rsi ; Arg1 (Pointer)

    rep outsd
end_func

; ---------------------------------------------------------------------------

; Read a sequence of bytes from an I/O port.
;
; inputs:
;   di   - I/O port
;   rsi  - pointer to where to put bytes read
;   rdx  - number of bytes to read
;
; outputs:
;   none
;
; C equivalent: void insb(uint16_t port, void *addr, uint64_t count);
;
beg_func insb
    mov rcx, rdx ; Arg2 (Count)
    mov dx,   di ; Arg0 (Port)
    mov rsi, rsi ; Arg1 (Pointer)

    rep insb
end_func

; Read a sequence of words from an I/O port.
;
; inputs:
;   di   - I/O port
;   rsi  - pointer to where to put words read
;   rdx  - number of words to read
;
; outputs:
;   none
;
; C equivalent: void insw(uint16_t port, void *addr, uint64_t count);
;
beg_func insw
    mov rcx, rdx ; Arg2 (Count)
    mov dx,   di ; Arg0 (Port)
    mov rsi, rsi ; Arg1 (Pointer)

    rep insw
end_func

; Read a sequence of dwords from an I/O port.
;
; inputs:
;   di   - I/O port
;   rsi  - pointer to where to put dwords read
;   rdx  - number of dwords to read
;
; outputs:
;   none
;
; C equivalent: void insd(uint16_t port, void *addr, uint64_t count);
;
beg_func insd
    mov rcx, rdx ; Arg2 (Count)
    mov dx,   di ; Arg0 (Port)
    mov rsi, rsi ; Arg1 (Pointer)

    rep insd
end_func

; ---------------------------------------------------------------------------

; Wait for an I/O operation to complete.
;
; inputs:
;   di  - I/O port
;
; outputs:
;   none
;
; C equivalent: void io_wait(uint16_t port);
;
beg_func io_wait
; while ((status & 0x20) == 0);
.loop:
    in al, dx
    test al, 0x20
    jz .loop
end_func
