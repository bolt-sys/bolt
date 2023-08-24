set(C_OPTIONS
        "-Wall"
        "-Wextra"
        "-std=gnu11"
        "-ffreestanding"
        "-fno-stack-protector"
        "-fno-stack-check"
        "-fno-lto"
        "-fPIE"
        "-mno-80387"
        "-mno-mmx"
        "-mno-sse"
        "-mno-sse2"
        "-mno-red-zone"
)

set(ASM_OPTIONS
    "-Fdwarf"
    "-g"
)

set(LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/linker.ld")

set(LINKER_OPTIONS
        "-nostdlib"
        "-static"
        "-pie"
        "-ztext"
        "-zmax-page-size=0x1000"
        "-T${LINKER_SCRIPT}"
)

add_executable(kernel
        source/kernel.c
        source/GDT.c
        source/ITR.c
        source/fuck_me_daddy.asm
)

target_link_options(kernel PRIVATE ${LINKER_OPTIONS})
target_compile_options(kernel PRIVATE $<$<COMPILE_LANGUAGE:C>:${C_OPTIONS}> $<$<COMPILE_LANGUAGE:ASM_NASM>:${ASM_OPTIONS}>)
target_include_directories(kernel PRIVATE include)

set_target_properties(${TARGET_NAME} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

target_link_libraries(kernel PRIVATE limine)
