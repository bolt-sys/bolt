#include <RuntimeLib.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE ("RtlFillMemory", "[Memory]") {
    SECTION ("Fills buffer with specified byte value") {
        UINT8 buffer[10];

        RtlFillMemory (buffer, sizeof (buffer), 0xFF);
        for (UINTN i = 0; i < sizeof (buffer); i++) {
            REQUIRE (buffer[i] == 0xFF);
        }
    }

    SECTION ("Returns STATUS_SUCCESS on success") {
        UINT8  buffer[10];
        STATUS status = RtlFillMemory (buffer, sizeof (buffer), 0xFF);

        REQUIRE (status == STATUS_SUCCESS);
    }

    SECTION ("Returns STATUS_INVALID_PARAMETER if Destination is NULL") {
        STATUS status = RtlFillMemory (NULL, 10, 0xFF);

        REQUIRE (status == STATUS_INVALID_PARAMETER);
    }

    SECTION ("Returns STATUS_SUCCESS if Length is 0") {
        UINT8  buffer[10];
        STATUS status = RtlFillMemory (buffer, 0, 0xFF);

        REQUIRE (status == STATUS_SUCCESS);
    }
}

TEST_CASE ("RtlZeroMemory", "[Memory]") {
    SECTION ("Zeroes buffer") {
        UINT8 buffer[10];

        RtlFillMemory (buffer, sizeof (buffer), 0xFF);
        RtlZeroMemory (buffer, sizeof (buffer));
        for (UINTN i = 0; i < sizeof (buffer); i++) {
            REQUIRE (buffer[i] == 0x00);
        }
    }

    SECTION ("Returns STATUS_SUCCESS on success") {
        UINT8  buffer[10];
        STATUS status = RtlZeroMemory (buffer, sizeof (buffer));

        REQUIRE (status == STATUS_SUCCESS);
    }

    SECTION ("Returns STATUS_INVALID_PARAMETER if Destination is NULL") {
        STATUS status = RtlZeroMemory (NULL, 10);

        REQUIRE (status == STATUS_INVALID_PARAMETER);
    }

    SECTION ("Returns STATUS_SUCCESS if Length is 0") {
        UINT8  buffer[10];
        STATUS status = RtlZeroMemory (buffer, 0);

        REQUIRE (status == STATUS_SUCCESS);
    }
}

TEST_CASE ("RtlCopyMemory", "[Memory]") {
    SECTION ("Copies source buffer to destination buffer") {
        UINT8 src[10] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99 };
        UINT8 dst[10] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

        RtlCopyMemory (dst, sizeof (dst), src, sizeof (src));
        for (UINTN i = 0; i < sizeof (dst); i++) {
            REQUIRE (dst[i] == src[i]);
        }
    }

    SECTION ("Returns STATUS_SUCCESS on success") {
        UINT8  src[10] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99 };
        UINT8  dst[10] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
        STATUS status  = RtlCopyMemory (dst, sizeof (dst), src, sizeof (src));

        REQUIRE (status == STATUS_SUCCESS);
    }

    SECTION ("Returns STATUS_INVALID_PARAMETER if Destination is NULL") {
        UINT8  src[10] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99 };
        STATUS status  = RtlCopyMemory (NULL, 10, src, sizeof (src));

        REQUIRE (status == STATUS_INVALID_PARAMETER);
    }

    SECTION ("Returns STATUS_INVALID_PARAMETER if Source is NULL") {
        UINT8  dst[10] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
        STATUS status  = RtlCopyMemory (dst, sizeof (dst), NULL, 10);

        REQUIRE (status == STATUS_INVALID_PARAMETER);
    }

    SECTION ("Returns STATUS_INVALID_PARAMETER if Destination overlaps with Source") {
        UINT8  buffer[10] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99 };
        STATUS status     = RtlCopyMemory (buffer + 2, 6, buffer, 6);

        REQUIRE (status == STATUS_INVALID_PARAMETER);
    }

    SECTION ("Returns STATUS_BUFFER_TOO_SMALL if DestinationSize is too small for the copy") {
        UINT8  src[10] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99 };
        UINT8  dst[5];
        STATUS status = RtlCopyMemory (dst, sizeof (dst), src, sizeof (src));

        REQUIRE (status == STATUS_BUFFER_TOO_SMALL);
    }

    SECTION ("Returns STATUS_SUCCESS if Length is 0") {
        UINT8  src[10] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99 };
        UINT8  dst[10] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
        STATUS status  = RtlCopyMemory (dst, sizeof (dst), src, 0);

        REQUIRE (status == STATUS_SUCCESS);
    }
}

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
