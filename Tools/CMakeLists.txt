add_library(HostRuntimeLib STATIC Runtime/HostRuntime.c)
target_link_libraries(HostRuntimeLib PUBLIC RuntimeLib UserLib)

add_subdirectory(ImageBuilder)
