set(C_OPTIONS "-nostdinc")

file(GLOB_RECURSE RTL_SOURCES "*.h" "*.c")

add_library(RuntimeLib STATIC ${RTL_SOURCES})
target_include_directories(RuntimeLib
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/Include
        ${CMAKE_CURRENT_LIST_DIR}/Include/${BOLT_TARGET_ARCH}
    )

message(${BOLT_TARGET_ARCH})

target_compile_options(RuntimeLib PUBLIC $<$<COMPILE_LANGUAGE:C>:${C_OPTIONS}>)
