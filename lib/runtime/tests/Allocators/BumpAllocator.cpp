#include <RuntimeLib.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE ("AllocatePages", "[BumpAllocator]") {
    BUMP_ALLOCATOR allocator { };

    SECTION("Make sure allocator cannot be NULL") {
        STATUS status = RtlBumpAllocatorInitialize (NULL);
        REQUIRE (status == STATUS_INVALID_PARAMETER);
    }

    RtlBumpAllocatorInitialize (&allocator);

    //
    // Setup code
    //
    allocator.bumpers[0].heap_start = (UINTN) new UINT8[PAGE_SIZE_4K];
    allocator.bumpers[0].heap_end   = allocator.bumpers[0].heap_start + PAGE_SIZE_4K;
    allocator.bumpers[0].next       = allocator.bumpers[0].heap_start;

    allocator.bumpers[1].heap_start = (UINTN) new UINT8[PAGE_SIZE_4K * 2];
    allocator.bumpers[1].heap_end   = allocator.bumpers[1].heap_start + PAGE_SIZE_4K * 2;
    allocator.bumpers[1].next       = allocator.bumpers[1].heap_start;

    SECTION("Make sure we cannot allocate using NULL as Self") {
        VOID*  address = NULL;
        STATUS status  = allocator.AllocatePages(NULL, &address, 1, PAGE_SIZE_4K);

        assert(address == NULL);
        assert(status == STATUS_NOT_INITIALIZED);
    }

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

    //
    // Cleanup code
    //
    delete[] (UINT8*)allocator.bumpers[0].heap_start;
    delete[] (UINT8*)allocator.bumpers[1].heap_start;
}

// Just in case
TEST_CASE ("FreePages", "[BumpAllocator]") {
    BUMP_ALLOCATOR allocator { };

    SECTION("Make sure allocator cannot be NULL") {
        STATUS status = RtlBumpAllocatorInitialize (NULL);
        REQUIRE (status == STATUS_INVALID_PARAMETER);
    }

    RtlBumpAllocatorInitialize (&allocator);

    // Setup code
    allocator.bumpers[0].heap_start = (UINTN) new UINT8[PAGE_SIZE_4K];
    allocator.bumpers[0].heap_end   = allocator.bumpers[0].heap_start + PAGE_SIZE_4K;
    allocator.bumpers[0].next       = allocator.bumpers[0].heap_start;

    allocator.bumpers[1].heap_start = (UINTN) new UINT8[PAGE_SIZE_4K * 2];
    allocator.bumpers[1].heap_end   = allocator.bumpers[1].heap_start + PAGE_SIZE_4K * 2;
    allocator.bumpers[1].next       = allocator.bumpers[1].heap_start;

    SECTION("Make sure we cannot free while using NULL as Self") {
        STATUS status = allocator.FreePages(NULL, NULL, 1, PAGE_SIZE_4K);
        assert(status == STATUS_NOT_INITIALIZED);
    }

    SECTION("Free should always succeed") {
        VOID*  address = NULL;
        STATUS status = PA_ALLOCATE(&allocator, &address, 1, PAGE_SIZE_4K);
        REQUIRE(status == STATUS_SUCCESS);

        status = PA_FREE(&allocator, &address, 1, PAGE_SIZE_4K);
        REQUIRE(status == STATUS_SUCCESS);
        REQUIRE (address == NULL);
    }

    SECTION("Free cannot be NULL") {
        STATUS status = PA_FREE(&allocator, NULL, 1, PAGE_SIZE_4K);
        REQUIRE(status == STATUS_INVALID_PARAMETER);
    }

    SECTION("Free cannot be zero") {
        VOID*  address = NULL;
        STATUS status = PA_ALLOCATE(&allocator, &address, 1, PAGE_SIZE_4K);
        REQUIRE(status == STATUS_SUCCESS);

        status = PA_FREE(&allocator, &address, 0, PAGE_SIZE_4K);
        REQUIRE(status == STATUS_INVALID_PARAMETER);
    }

    delete[] (UINT8*)allocator.bumpers[0].heap_start;
    delete[] (UINT8*)allocator.bumpers[1].heap_start;
}
