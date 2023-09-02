include(FetchContent)

FetchContent_Declare(
        limine
        URL      https://github.com/limine-bootloader/limine/archive/7563eb4dbe9c5c9eca8439cfb96206dfc0fa3bf6.tar.gz
        URL_HASH SHA1=d74aa6b82874141e9eb18d732375359571052833
)

message(STATUS "Fetching external dependencies...")

FetchContent_MakeAvailable(limine)

message(STATUS "Limine '${limine_SOURCE_DIR}' fetched.")

add_library(limine INTERFACE ${limine_SOURCE_DIR}/limine.h)
target_include_directories(limine INTERFACE ${limine_SOURCE_DIR})
