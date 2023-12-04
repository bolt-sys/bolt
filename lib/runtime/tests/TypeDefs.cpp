#include <RuntimeLib.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE ("STRINGIFY", "[TypeDefsTests]") {
    REQUIRE (std::string_view (STRINGIFY (HelloWorld)) == "HelloWorld");
}

TEST_CASE ("ARRAY_SIZE", "[TypeDefsTests]") {
    int arr[10];

    REQUIRE (ARRAY_SIZE (arr) == 10);
}

TEST_CASE ("ALIGN_DOWN", "[TypeDefsTests]") {
    SECTION ("Test for Directly aligned value") {
        REQUIRE (ALIGN_DOWN (0x1000, 0x1000) == 0x1000);
    }

    SECTION ("Test for Non-Directly aligned value") {
        REQUIRE (ALIGN_DOWN (0x1001, 0x1000) == 0x1000);
    }

    SECTION ("Test for Non-Directly aligned value but under 0x1000") {
        REQUIRE (ALIGN_DOWN (0x100, 0x1000) == 0x0);
    }
}

TEST_CASE ("ALIGN_UP", "[TypeDefsTests]") {
    SECTION ("Test for Directly aligned value") {
        REQUIRE (ALIGN_UP (0x1000, 0x1000) == 0x1000);
    }

    SECTION ("Test for Non-Directly aligned value") {
        REQUIRE (ALIGN_UP (0x1001, 0x1000) == 0x2000);
    }

    SECTION ("Test for Non-Directly aligned value but under 0x1000") {
        REQUIRE (ALIGN_UP (0x100, 0x1000) == 0x1000);
    }
}

TEST_CASE ("IS_ALIGNED", "[TypeDefsTests]") {
    SECTION ("Test for Directly aligned value") {
        REQUIRE (IS_ALIGNED (0x1000, 0x1000) == true);
    }

    SECTION ("Test for Non-Directly aligned value") {
        REQUIRE (IS_ALIGNED (0x1001, 0x1000) == false);
    }

    SECTION ("Test for Non-Directly aligned value but under 0x1000") {
        REQUIRE (IS_ALIGNED (0x100, 0x1000) == false);
    }
}

TEST_CASE ("MIN", "[TypeDefsTests]") {
    REQUIRE (MIN (10, 20) == 10);
    REQUIRE (MIN (20, 10) == 10);
}

TEST_CASE ("MAX", "[TypeDefsTests]") {
    REQUIRE (MAX (10, 20) == 20);
    REQUIRE (MAX (20, 10) == 20);
}

TEST_CASE ("CLAMP", "[TypeDefsTests]") {
    REQUIRE (CLAMP (10, 0, 20) == 10);
    REQUIRE (CLAMP (10, 20, 30) == 20);
    REQUIRE (CLAMP (10, 0, 5) == 5);
}

TEST_CASE ("Bit Manipulation", "[TypeDefsTests]") {
    SECTION ("Validate that BIT(6) is 0x40") {
        REQUIRE (BIT (6) == 0x40);
    }

    SECTION ("Check if BIT_IS_SET(0x40, 6) is true") {
        REQUIRE (BIT_IS_SET (0x40, 6) == true);
    }

    SECTION ("Check if BIT_IS_CLEAR(0x40, 6) is false") {
        REQUIRE (BIT_IS_CLEAR (0x40, 6) == false);
    }

    SECTION ("Check if BIT_IS_CLEAR(0x40, 7) is true") {
        REQUIRE (BIT_IS_CLEAR (0x40, 7) == true);
    }

    SECTION ("Check if BIT_IS_SET(0x40, 7) is false") {
        REQUIRE (BIT_IS_SET (0x40, 7) == false);
    }

    SECTION ("Set 6 bit within 0x0") {
        UINT8 val = 0x0;

        BIT_SET (val, 6);

        REQUIRE (val == 0x40);
    }

    SECTION ("Set 6 bit within 0x40") {
        UINT8 val = 0x40;

        BIT_SET (val, 6);

        REQUIRE (val == 0x40);
    }

    SECTION ("Clear 6 bit within 0x40") {
        UINT8 val = 0x40;

        BIT_CLEAR (val, 6);

        REQUIRE (val == 0x0);
    }

    SECTION ("Clear 6 bit within 0x0") {
        UINT8 val = 0x0;

        BIT_CLEAR (val, 6);

        REQUIRE (val == 0x0);
    }

    SECTION ("Flip 6 bit within 0x0") {
        UINT8 val = 0x0;

        BIT_FLIP (val, 6);

        REQUIRE (val == 0x40);
    }

    SECTION ("Flip 6 bit within 0x40") {
        UINT8 val = 0x40;

        BIT_FLIP (val, 6);

        REQUIRE (val == 0x0);
    }
}

TEST_CASE ("CONTAINER_OF", "[TypeDefsTests]") {
    struct TestStruct {
        int a;
        int b;
        int c;
    };

    TestStruct testStruct;

    testStruct.a = 10;
    testStruct.b = 20;
    testStruct.c = 30;

    int*        ptr = &testStruct.b;

    TestStruct* testStructPtr = CONTAINER_OF (ptr, TestStruct, b);

    REQUIRE (testStructPtr->a == 10);
    REQUIRE (testStructPtr->b == 20);
    REQUIRE (testStructPtr->c == 30);

    REQUIRE (&testStructPtr->a == &testStruct.a);
    REQUIRE (&testStructPtr->b == &testStruct.b);
    REQUIRE (&testStructPtr->c == &testStruct.c);

    REQUIRE (ptr == &testStruct.b);
}

TEST_CASE ("VA_LIST", "[TypeDefsTests]") {
    int  arg1 = 10;
    int  arg2 = 20;
    int  arg3 = arg2;

    auto TestVaList = [](int arg1, int arg2, ...) {
                          VA_LIST ap;

                          VA_START (ap, arg2);
                          REQUIRE (__builtin_va_arg (ap, int) == arg2);
                          VA_END (ap);
                      };

    TestVaList (arg1, arg2, arg3);
}
