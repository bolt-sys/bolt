#include <RuntimeLib.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE ("AllocatePages", "[BumpAllocator]") {
    BUMP_ALLOCATOR allocator { };

    RtlBumpAllocatorInitialize (&allocator);

    // Setup code
    allocator.bumpers[0].heap_start = (UINTN) new UINT8[PAGE_SIZE_4K];
    allocator.bumpers[0].heap_end   = allocator.bumpers[0].heap_start + PAGE_SIZE_4K;
    allocator.bumpers[0].next       = allocator.bumpers[0].heap_start;

    allocator.bumpers[1].heap_start = (UINTN) new UINT8[PAGE_SIZE_4K * 2];
    allocator.bumpers[1].heap_end   = allocator.bumpers[1].heap_start + PAGE_SIZE_4K * 2;
    allocator.bumpers[1].next       = allocator.bumpers[1].heap_start;

    SECTION ("Allocates 4K page") {
        VOID*  address = NULL;
        STATUS status  = PA_ALLOCATE (&allocator, &address, 1, PAGE_SIZE_4K);

        REQUIRE (status == STATUS_SUCCESS);
        REQUIRE (allocator.bumpers[0].next == allocator.bumpers[0].heap_end);
        REQUIRE (address != NULL);
        REQUIRE (address == (VOID*)allocator.bumpers[0].heap_start);
    }

    SECTION ("Allocate multiple pages") {
        VOID*  address = NULL;
        STATUS status  = PA_ALLOCATE (&allocator, &address, 2, PAGE_SIZE_4K);

        REQUIRE (status == STATUS_SUCCESS);
        REQUIRE (allocator.bumpers[1].next == allocator.bumpers[1].heap_end);
        REQUIRE (address != NULL);
        REQUIRE (address == (VOID*)allocator.bumpers[1].heap_start);
    }

    SECTION ("Making sure we cannot over-allocate") {
        VOID*  address = NULL;
        STATUS status  = PA_ALLOCATE (&allocator, &address, PAGE_SIZE_4K, PAGE_SIZE_4K);

        REQUIRE (status == STATUS_OUT_OF_MEMORY);
        REQUIRE (address == NULL);
    }

    SECTION ("Try unaligned pages") {
        VOID*  address = NULL;
        STATUS status  = PA_ALLOCATE (&allocator, &address, 1, 1025);

        REQUIRE (status == STATUS_INVALID_ALIGNMENT);
        REQUIRE (address == NULL);
    }

    SECTION ("Make sure zero pages are not allocated") {
        VOID*  address = NULL;
        STATUS status  = PA_ALLOCATE (&allocator, &address, 0, PAGE_SIZE_4K);

        REQUIRE (status == STATUS_INVALID_PARAMETER);
        REQUIRE (address == NULL);
    }

    SECTION ("Make sure NULL Address is not allocated") {
        STATUS status = PA_ALLOCATE (&allocator, NULL, 1, PAGE_SIZE_4K);

        REQUIRE (status == STATUS_INVALID_PARAMETER);
    }

    SECTION ("Allocate multiple pages, multiple times") {
        VOID*  address = NULL;
        STATUS status;

        // First allocation, this is guaranteed to succeed
        status = PA_ALLOCATE (&allocator, &address, 1, PAGE_SIZE_4K);

        REQUIRE (status == STATUS_SUCCESS);
        REQUIRE (address != NULL);
        REQUIRE (address == (VOID*)allocator.bumpers[0].heap_start);
        REQUIRE (allocator.bumpers[0].next == allocator.bumpers[0].heap_end);

        // Second allocation, this is also guaranteed, but we must be on the second bumper
        status = PA_ALLOCATE (&allocator, &address, 1, PAGE_SIZE_4K);

        REQUIRE (status == STATUS_SUCCESS);
        REQUIRE (address != NULL);
        REQUIRE (address == (VOID*)allocator.bumpers[1].heap_start);

        // Same as above, but we should reach the end of the second bumper
        status = PA_ALLOCATE (&allocator, &address, 1, PAGE_SIZE_4K);

        REQUIRE (status == STATUS_SUCCESS);
        REQUIRE (address != NULL);
        REQUIRE (address == (VOID*)(allocator.bumpers[1].heap_start + PAGE_SIZE_4K));
        REQUIRE (allocator.bumpers[1].next == allocator.bumpers[1].heap_end);

        // Third allocation, this should fail as we have no more bumpers available.
        status = PA_ALLOCATE (&allocator, &address, 1, PAGE_SIZE_4K);

        REQUIRE (status == STATUS_OUT_OF_MEMORY);
        REQUIRE (address == NULL);
    }
}
