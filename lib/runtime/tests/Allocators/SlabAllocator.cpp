#include <RuntimeLib.h>

#include <catch2/catch_test_macros.hpp>

//
// Emulation code for page allocator.
//
PAGE_ALLOCATOR g_MallocAllocator = {
    [](PAGE_ALLOCATOR* Self, VOID** Address, UINTN Pages, UINTN Align) -> STATUS {
        if (Align == 0) {
            Align = PAGE_SIZE_4K;
        }

        if (Address == NULL || (Align != PAGE_SIZE_4K && Align != PAGE_SIZE_2M && Align != PAGE_SIZE_1G)) {
            return STATUS_INVALID_PARAMETER;
        }

        if (Pages < 1) {
            return STATUS_INVALID_PARAMETER;
        }

        *Address = malloc(Pages * Align);
        if (*Address == NULL) {
            return STATUS_OUT_OF_MEMORY;
        }

        return STATUS_SUCCESS;
    },
    [](PAGE_ALLOCATOR* Self, VOID** Address, UINTN Pages, UINTN Align) -> STATUS {
        if (Address == NULL || Pages == 0 || (Align != PAGE_SIZE_4K && Align != PAGE_SIZE_2M && Align != PAGE_SIZE_1G)) {
            return STATUS_INVALID_PARAMETER;
        }

        if (Pages < 1) {
            return STATUS_INVALID_PARAMETER;
        }

        free(*Address);

        return STATUS_SUCCESS;
    }
};

//
// Emulation code for a full page allocator.
//
PAGE_ALLOCATOR g_NullAllocator = {
    [](PAGE_ALLOCATOR* Self, VOID** Address, UINTN Pages, UINTN Align) -> STATUS {
        return STATUS_OUT_OF_MEMORY;
    },
    [](PAGE_ALLOCATOR* Self, VOID** Address, UINTN Pages, UINTN Align) -> STATUS {
        return STATUS_SUCCESS;
    }
};

TEST_CASE("RtlCreateSlabCache", "[SlabAllocator]") {
    SLAB_CACHE* Cache = NULL;
    STATUS Status = STATUS_SUCCESS;

    SECTION("Check for invalid parameter") {
        Status = RtlCreateSlabCache(NULL, &g_MallocAllocator, sizeof(UINTN), SLAB_CACHE_FLAGS_NONE, NULL, NULL);
        REQUIRE(Status == STATUS_INVALID_PARAMETER);

        Status = RtlCreateSlabCache(&Cache, NULL, sizeof(UINTN), SLAB_CACHE_FLAGS_NONE, NULL, NULL);
        REQUIRE(Status == STATUS_INVALID_PARAMETER);

        Status = RtlCreateSlabCache(&Cache, &g_MallocAllocator, 0, SLAB_CACHE_FLAGS_NONE, NULL, NULL);
        REQUIRE(Status == STATUS_INVALID_PARAMETER);

        Status = RtlCreateSlabCache(&Cache, &g_MallocAllocator, sizeof(UINTN), SLAB_CACHE_FLAGS_NONE, NULL, NULL);
        REQUIRE(Status == STATUS_SUCCESS);

        Status = RtlDestroySlabCache(&Cache);
        REQUIRE(Status == STATUS_SUCCESS);
    }

    SECTION("Create and destroy a slab cache") {
        Status = RtlCreateSlabCache(&Cache, &g_MallocAllocator, sizeof(UINTN), SLAB_CACHE_FLAGS_NONE, NULL, NULL);

        REQUIRE(Status == STATUS_SUCCESS);

        REQUIRE(Cache != NULL);
        REQUIRE(Cache->BaseAllocator == &g_MallocAllocator);
        REQUIRE(Cache->ObjectSize == sizeof(UINTN));
        REQUIRE(Cache->ObjectCount == ALIGN_UP (PAGE_SIZE_4K, sizeof(UINTN)) / sizeof(UINTN));
        REQUIRE(Cache->Flags == SLAB_CACHE_FLAGS_NONE);
        REQUIRE(Cache->Ctor == RtlDefaultSlabCtor);
        REQUIRE(Cache->Dtor == RtlDefaultSlabDtor);
        REQUIRE(Cache->Empty == NULL);
        REQUIRE(Cache->Partial == NULL);
        REQUIRE(Cache->Full == NULL);

        Status = RtlDestroySlabCache(&Cache);
        REQUIRE(Status == STATUS_SUCCESS);
    }

    SECTION("Make sure we cannot allocate a cache if the base allocator is full") {
        Status = RtlCreateSlabCache(&Cache, &g_NullAllocator, sizeof(UINTN), SLAB_CACHE_FLAGS_NONE, NULL, NULL);
        REQUIRE(Status == STATUS_OUT_OF_MEMORY);
    }
}

TEST_CASE("RtlDestroySlabCache", "[SlabAllocator]") {
    SLAB_CACHE* Cache = NULL;
    STATUS Status = STATUS_SUCCESS;

    SECTION("Invalid arguments") {
        Status = RtlDestroySlabCache(NULL);
        REQUIRE(Status == STATUS_INVALID_PARAMETER);

        Status = RtlDestroySlabCache(&Cache);
        REQUIRE(Status == STATUS_INVALID_PARAMETER);
    }
}

TEST_CASE("RtlSlabAllocate", "[SlabAllocator]") {
    SLAB_CACHE* Cache = NULL;
    STATUS Status = STATUS_SUCCESS;

    Status = RtlCreateSlabCache(&Cache, &g_MallocAllocator, sizeof(UINTN), SLAB_CACHE_FLAGS_NONE, NULL, NULL);
    REQUIRE(Status == STATUS_SUCCESS);

    SECTION("Allocate a single object") {
        UINTN* Object = NULL;

        Status = RtlSlabAllocate(Cache, SLAB_CACHE_FLAGS_NONE, (VOID**)&Object);
        REQUIRE(Status == STATUS_SUCCESS);

        // We need an object
        REQUIRE(Object != NULL);

        // Make sure we correctly moved the object from the empty list to the partial list
        REQUIRE(Cache->Empty == NULL);
        REQUIRE(Cache->Partial != NULL);
        REQUIRE(Cache->Partial->FreeCount == Cache->ObjectCount - 1);
        REQUIRE(Cache->Partial->Next == NULL);

        // And it shouldn't be in the full list
        REQUIRE(Cache->Full == NULL);
    }

    SECTION("Allocate all Objects") {
        UINTN* Object = NULL;

        for (UINTN i = 0; i < Cache->ObjectCount; i++) {
            Status = RtlSlabAllocate(Cache, SLAB_CACHE_FLAGS_NONE, (VOID**)&Object);
            REQUIRE(Status == STATUS_SUCCESS);

            // We need an object
            REQUIRE(Object != NULL);
            REQUIRE(Cache->Empty == NULL);

            if (i <= Cache->ObjectCount - 2) {
                REQUIRE(Cache->Partial != NULL);
                REQUIRE(Cache->Partial->FreeCount == Cache->ObjectCount - i - 1);
                REQUIRE(Cache->Partial->Next == NULL);
            }
        }

        // We should have allocated all objects
        REQUIRE(Cache->Full != NULL);
    }

    SECTION("Don't allocate at all") {
        // Just to test the teardown code
    }

    SECTION("Allocate invalid arguments") {
        UINTN* Object = NULL;

        Status = RtlSlabAllocate(NULL, SLAB_CACHE_FLAGS_NONE, (VOID**)&Object);
        REQUIRE(Status == STATUS_INVALID_PARAMETER);

        Status = RtlSlabAllocate(Cache, SLAB_CACHE_FLAGS_NONE, NULL);
        REQUIRE(Status == STATUS_INVALID_PARAMETER);

        Status = RtlSlabAllocate(Cache, SLAB_CACHE_FLAGS_NONE, (VOID**)&Object);
        REQUIRE(Status == STATUS_SUCCESS);

        Status = RtlSlabAllocate(Cache, SLAB_CACHE_FLAGS_NONE, (VOID**)&Object);
        REQUIRE(Status == STATUS_SUCCESS);
    }

    SECTION("Over-allocate") {
        UINTN* Object = NULL;

        for (UINTN i = 0; i < Cache->ObjectCount; i++) {
            Status = RtlSlabAllocate(Cache, SLAB_CACHE_FLAGS_NONE, (VOID**)&Object);
            REQUIRE(Status == STATUS_SUCCESS);
        }

        Status = RtlSlabAllocate(Cache, SLAB_CACHE_FLAGS_NONE, (VOID**)&Object);
        REQUIRE(Status == STATUS_SUCCESS); // This should be a new slab
        REQUIRE(Cache->Full != NULL);
        REQUIRE(Cache->Full->Next == NULL);
        REQUIRE(Cache->Full->FreeCount == 0);

        REQUIRE(Cache->Partial != NULL);
        REQUIRE(Cache->Partial->Next == NULL);
        REQUIRE(Cache->Partial->FreeCount == Cache->ObjectCount - 1);
    }

    SECTION("Allocate and free") {
        UINTN* Object = NULL;

        // Allocate
        Status = RtlSlabAllocate(Cache, SLAB_CACHE_FLAGS_NONE, (VOID**)&Object);
        REQUIRE(Status == STATUS_SUCCESS); // This should be a new slab
        REQUIRE(Cache->Partial != NULL);
        REQUIRE(Cache->Partial->Next == NULL);
        REQUIRE(Cache->Partial->FreeCount == Cache->ObjectCount - 1);

        // Free
        Status = RtlSlabFree(Cache, SLAB_CACHE_FLAGS_NONE, (VOID **)&Object);
        REQUIRE(Status == STATUS_SUCCESS);
        REQUIRE(Object == NULL);
        REQUIRE(Cache->Empty != NULL);
        REQUIRE(Cache->Empty->Next == NULL);
        REQUIRE(Cache->Empty->FreeCount == Cache->ObjectCount);
        REQUIRE(Cache->Partial == NULL);
    }

    //
    // Teardown
    //
    Status = RtlDestroySlabCache(&Cache);
    REQUIRE(Status == STATUS_SUCCESS);
}
