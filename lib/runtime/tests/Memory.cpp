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

    SECTION ("Simple copy") {
        UINT8  src[10] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99 };
        UINT8  dst[10] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
        STATUS status  = RtlCopyMemory (dst, sizeof (dst), src, sizeof (src));

        REQUIRE (status == STATUS_SUCCESS);
    }

    SECTION ("Make sure to fail if Destination is NULL (STATUS_INVALID_PARAMETER)") {
        UINT8  src[10] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99 };
        STATUS status  = RtlCopyMemory (NULL, 10, src, sizeof (src));

        REQUIRE (status == STATUS_INVALID_PARAMETER);
    }

    SECTION ("Make sure to fail if Source is NULL (STATUS_INVALID_PARAMETER)") {
        UINT8  dst[10] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
        STATUS status  = RtlCopyMemory (dst, sizeof (dst), NULL, 10);

        REQUIRE (status == STATUS_INVALID_PARAMETER);
    }

    SECTION ("Make sure to fail if Destination overlaps with Source (STATUS_INVALID_PARAMETER)") {
        UINT8  buffer[10] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99 };
        STATUS status     = RtlCopyMemory (buffer + 2, 6, buffer, 6);

        REQUIRE (status == STATUS_INVALID_PARAMETER);
    }

    SECTION("Make sure to fail if Source overlaps with Destination (STATUS_INVALID_PARAMETER)") {
        UINT8 buffer[10] = { 0 };
        STATUS status = RtlCopyMemory (buffer, sizeof (buffer), buffer + 1, sizeof (buffer) - 1);

        REQUIRE (status == STATUS_INVALID_PARAMETER);
    }

    SECTION ("Make sure to fail if DestinationSize is too small for the copy (STATUS_BUFFER_TOO_SMALL)") {
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
