file(GLOB_RECURSE SOURCES "*.h" "*.c")

add_executable(ImageBuilder ${SOURCES})
target_include_directories(ImageBuilder
        PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}/Include
            ${CMAKE_CURRENT_LIST_DIR}/Include/${BOLT_TARGET_ARCH}
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(ImageBuilder PRIVATE RuntimeLib HostRuntimeLib)
