#include <RuntimeLib.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE ("GUID equality", "[GUID]") {
    GUID guid1 = { 0x12345678, 0x9ABC, 0xDEFA, { 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFA }
    };
    GUID guid2 = { 0x12345678, 0x9ABC, 0xDEFA, { 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFA }
    };
    GUID guid3 = { 0x87654321, 0xCDEF, 0xABCD, { 0x87, 0x65, 0x43, 0x21, 0xFE, 0xDC, 0xBA, 0x98 }
    };

    REQUIRE (GUID_EQ (guid1, guid2));
    REQUIRE_FALSE (GUID_EQ (guid1, guid3));
}

TEST_CASE ("GUID inequality", "[GUID]") {
    GUID guid1 = { 0x12345678, 0x9ABC, 0xDEFA, { 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFA }
    };
    GUID guid2 = { 0x12345678, 0x9ABC, 0xDEFA, { 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFA }
    };
    GUID guid3 = { 0x87654321, 0xCDEF, 0xABCD, { 0x87, 0x65, 0x43, 0x21, 0xFE, 0xDC, 0xBA, 0x98 }
    };

    REQUIRE_FALSE (GUID_NE (guid1, guid2));
    REQUIRE (GUID_NE (guid1, guid3));
}

TEST_CASE ("GUID_NULL", "[GUID]") {
    GUID guid = GUID_NULL;

    REQUIRE (guid.Data1 == 0x00000000);
    REQUIRE (guid.Data2 == 0x0000);
    REQUIRE (guid.Data3 == 0x0000);
    REQUIRE (guid.Data4[0] == 0x00);
    REQUIRE (guid.Data4[1] == 0x00);
    REQUIRE (guid.Data4[2] == 0x00);
    REQUIRE (guid.Data4[3] == 0x00);
    REQUIRE (guid.Data4[4] == 0x00);
    REQUIRE (guid.Data4[5] == 0x00);
    REQUIRE (guid.Data4[6] == 0x00);
    REQUIRE (guid.Data4[7] == 0x00);
}
