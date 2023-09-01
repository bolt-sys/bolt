include(FetchContent)

FetchContent_Declare(
        limine
        GIT_REPOSITORY https://github.com/limine-bootloader/limine.git
        GIT_TAG        7563eb4dbe9c5c9eca8439cfb96206dfc0fa3bf6
)

message(STATUS "Fetching external dependencies...")

FetchContent_MakeAvailable(limine)

message(STATUS "Limine '${limine_SOURCE_DIR}' fetched.")

add_library(limine INTERFACE ${limine_SOURCE_DIR}/limine.h)
target_include_directories(limine INTERFACE ${limine_SOURCE_DIR})
