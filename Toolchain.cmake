# We enforce clang as compiler, because it's just easier to cross-compile
# with clang than with gcc.
set(CMAKE_C_COMPILER   clang)
set(CMAKE_CXX_COMPILER clang++)

set(BOLT_TARGET_TRIPLE "x86_64-bolt-elf")

set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_PROCESSOR ${CMAKE_HOST_SYSTEM_PROCESSOR})

if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
  set(BOLT_TARGET_ARCH "X64")
else()
  message(FATAL_ERROR "Unsupported target architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

set(CMAKE_C_COMPILER_TARGET ${BOLT_TARGET_TRIPLE})
set(CMAKE_CXX_COMPILER_TARGET ${BOLT_TARGET_TRIPLE})

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY") # Cannot rely on compiler tests when cross-compiling
                                                    # STATIC_LIBRARY is the only option that works.

set(CMAKE_C_FLAGS   "")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -O0 -g")
else()
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -O2 -gline-tables-only")
endif()

# Freestanding, cannot rely on the host.
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}     -ffreestanding -nostdlib -fno-builtin -fno-stack-protector")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}     -Wno-unused-parameter -Wno-unused-function")

# LLD all the way
set(CMAKE_EXE_LINKER_FLAGS_INIT    "-fuse-ld=lld")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-fuse-ld=lld")

# Don't rely on anything that the Host system provides
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE NEVER)
