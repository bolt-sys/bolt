file(GLOB_RECURSE KLIBC_SOURCES "*.c" "*.h")

set(C_OPTIONS "-nostdinc")

add_library(klibc STATIC ${KLIBC_SOURCES})
target_include_directories(klibc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(klibc PUBLIC $<$<COMPILE_LANGUAGE:C>:${C_OPTIONS}>)
