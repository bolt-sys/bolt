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
)

target_link_options(kernel PRIVATE ${LINKER_OPTIONS})
target_compile_options(kernel PRIVATE $<$<COMPILE_LANGUAGE:C>:${C_OPTIONS}>)

set_target_properties(${TARGET_NAME} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

target_link_libraries(kernel PRIVATE limine)
