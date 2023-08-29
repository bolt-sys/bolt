EXTERN GDTLoad, GDTReloadSegments

;
; Modifies:
;
; @brief Loads a GDTR
;
; @param[in] RDI - Pointer to GDTR
;
; @return          This function returns nothing.
;
GDTLoad:
    lgdt [rdi]
    ret

;
; Modifies: RSI
;
; @brief Reloads the segments
;
; @param[in] DI - Code Segment
; @param[in] SI - Data Segment
;
; @return         This function returns nothing.
GDTReloadSegments:
    push rbp
    mov rbp, rsp

    ; DS is easier to debug, so we set them first.
    mov ds, si
    mov es, si
    mov fs, si
    mov gs, si
    mov ss, si

    ; Load the code segment register
    ; in 64-bit mode, CS is loaded with a far return
    ; to the same address
    push di                     ; Arg1 = Code Segment Selector
    lea rsi, [rel .trampoline]  ; Get the address of the trampoline
    push rsi                    ; Arg2 = Trampoline address
    retfq                       ; Far return to the trampoline and our new code segment

.trampoline:
    leave
    ret
