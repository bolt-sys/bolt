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

TEST_CASE("RtlCreateSlabCache", "[SlabAllocator]") {
    SLAB_CACHE* Cache = NULL;
    STATUS Status = STATUS_SUCCESS;

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

    //
    // Teardown
    //
    Status = RtlDestroySlabCache(&Cache);
    REQUIRE(Status == STATUS_SUCCESS);
}
