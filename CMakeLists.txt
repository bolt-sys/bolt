cmake_minimum_required(VERSION 3.26.4)
project(bolt
    LANGUAGES C CXX ASM_NASM
    VERSION 0.1.0
    )

set(BOLT_TARGET_ARCH "X64" CACHE STRING "Target architecture" FORCE) # TODO: do not force.

add_subdirectory(External)
add_subdirectory(Kernel)
add_subdirectory(klibc)
add_subdirectory(Core/RuntimeLib)
add_subdirectory(Tools)
