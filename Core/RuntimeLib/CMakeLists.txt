set(C_OPTIONS
        "-Wall"
        "-Wextra"

        # Freestanding environment
        "-ffreestanding"
        "-nostdlib"
        "-nostdinc"

        # Stack protection is not possible for now.
        "-fno-stack-protector"
        "-fno-stack-check"

        # Disable features we cannot use, although we could enable them later.
        "-mno-80387"    # Disable FPU
        "-mno-mmx"      # Disable MMX
        "-mno-sse"      # Disable SSE
        "-mno-sse2"     # Disable SSE2
        "-mno-red-zone" # Disable red zone
)

file(GLOB_RECURSE RTL_SOURCES "*.h" "*.c")

add_library(RuntimeLib STATIC ${RTL_SOURCES})
target_include_directories(RuntimeLib
        PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}/Include
            ${CMAKE_CURRENT_LIST_DIR}/Include/${BOLT_TARGET_ARCH}
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}
    )

target_compile_options(RuntimeLib PRIVATE $<$<COMPILE_LANGUAGE:C>:${C_OPTIONS}>)
