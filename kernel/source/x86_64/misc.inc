; Utility macros

%ifndef BOLT_KERNEL_SOURCE_X86_64_MISC_ASM_
%define BOLT_KERNEL_SOURCE_X86_64_MISC_ASM_ 1

; Define a function
; Usage: beg_func <function name>
;        end_func
%macro beg_func 1
    global %1
    %1:
        push rbp
        mov rbp, rsp
%endmacro

; End a function
; Usage: end_func
%macro end_func 0
    leave
    ret
%endmacro

%endif ; BOLT_KERNEL_SOURCE_X86_64_MISC_ASM_
