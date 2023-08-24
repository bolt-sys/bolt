add_executable(kernel
        source/GDT.c
        source/ITR.c
        source/kernel.c
)

target_link_options(
    kernel PRIVATE
        -Wall
        -Wextra
        -std=gnu11
        -ffreestanding
        -fno-stack-protector
        -fno-stack-check
        -fno-lto
        -fPIE
        -mno-80387
        -mno-mmx
        -mno-sse
        -mno-sse2
        -mno-red-zone
)
