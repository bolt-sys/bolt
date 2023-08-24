cmake_minimum_required(VERSION 3.26.,4)
project(bolt
    LANGUAGES C CXX ASM_NASM
    VERSION 0.1.0
    )

add_subdirectory(external)
add_subdirectory(kernel)
