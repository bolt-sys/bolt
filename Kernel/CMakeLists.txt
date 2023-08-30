set(C_OPTIONS
        "-Wall"
        "-Wextra"

        # Freestanding environment
        "-ffreestanding"

        # Stack protection is not possible for now.
        "-fno-stack-protector"
        "-fno-stack-check"

        # Cannot rely on LTO, it will break the code.
        "-fno-lto"

        # Position independent executable
        "-fPIE"

        # Disable features we cannot use, especially during initialisation.
        "-mno-80387"    # Disable FPU
        "-mno-mmx"      # Disable MMX
        "-mno-sse"      # Disable SSE
        "-mno-sse2"     # Disable SSE2
        "-mno-red-zone" # Disable red zone
)

set(ASM_OPTIONS
        "-Fdwarf"
        "-g"
        "-i${CMAKE_CURRENT_LIST_DIR}/source/x86_64"
)

set(LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/${BOLT_TARGET_ARCH}/Linker.ld")

set(LINKER_OPTIONS
        "-nostdlib"
        "-static"
        "-pie"
        "-ztext"
        "-zmax-page-size=0x1000"
        "-T${LINKER_SCRIPT}"
)

file(GLOB_RECURSE KERNEL_SOURCES "*.c" "*.h" "*.asm")

add_executable(Kernel ${KERNEL_SOURCES})

target_link_options(Kernel PRIVATE ${LINKER_OPTIONS})
target_compile_options(Kernel PRIVATE $<$<COMPILE_LANGUAGE:C>:${C_OPTIONS}> $<$<COMPILE_LANGUAGE:ASM_NASM>:${ASM_OPTIONS}>)
target_include_directories(Kernel PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Include ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/${BOLT_TARGET_ARCH})

set_target_properties(${TARGET_NAME} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

target_link_libraries(Kernel PRIVATE limine klibc RuntimeLib)
