#include <RuntimeLib.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE ("COMBINE_STATUS", "[Status]") {
    STATUS Status = 0;

    //
    // Test all possible combinations of severity, user-defined, facility, and error code.
    //
    // Apparently i made so many errors in the implementation of this macro that i had to write a test for it.
    //

    //
    // Severity
    //
    SECTION ("STATUS_SEVERITY_SUCCESS") {
        Status = COMBINE_STATUS(STATUS_SEVERITY_SUCCESS, 0, STATUS_FACILITY_GENERIC, 0x00);

        REQUIRE (STATUS_SEVERITY(Status) == STATUS_SEVERITY_SUCCESS);
        REQUIRE (Status == 0b00000000000000000000000000000000);
    }

    SECTION ("STATUS_SEVERITY_INFORMATIONAL") {
        Status = COMBINE_STATUS(STATUS_SEVERITY_INFORMATIONAL, 0, STATUS_FACILITY_GENERIC, 0x00);

        REQUIRE (STATUS_SEVERITY(Status) == STATUS_SEVERITY_INFORMATIONAL);
        REQUIRE (Status == 0b01000000000000000000000000000000);
    }

    SECTION ("STATUS_SEVERITY_WARNING") {
        Status = COMBINE_STATUS(STATUS_SEVERITY_WARNING, 0, STATUS_FACILITY_GENERIC, 0x00);

        REQUIRE (STATUS_SEVERITY(Status) == STATUS_SEVERITY_WARNING);
        REQUIRE (Status == 0b10000000000000000000000000000000);
    }

    SECTION ("STATUS_SEVERITY_ERROR") {
        Status = COMBINE_STATUS(STATUS_SEVERITY_ERROR, 0, STATUS_FACILITY_GENERIC, 0x00);

        REQUIRE (STATUS_SEVERITY(Status) == STATUS_SEVERITY_ERROR);
        REQUIRE (Status == 0b11000000000000000000000000000000);
    }

    //
    // User-defined
    //
    SECTION ("STATUS_USERDEFINED") {
        Status = COMBINE_STATUS(STATUS_SEVERITY_SUCCESS, 1, STATUS_FACILITY_GENERIC, 0x00);

        REQUIRE (STATUS_USER_DEFINED(Status) == 1);
        REQUIRE (Status == 0b00100000000000000000000000000000);
    }

    //
    // Facility
    //
    SECTION ("STATUS_FACILITY") {
        Status = COMBINE_STATUS(STATUS_SEVERITY_SUCCESS, 0, 0x1234, 0x00);

        REQUIRE (STATUS_FACILITY(Status) == 0x1234);
    }

    //
    // Error code
    //
    SECTION ("STATUS_ERROR_CODE") {
        Status = COMBINE_STATUS(STATUS_SEVERITY_SUCCESS, 0, STATUS_FACILITY_GENERIC, 0x1234);

        REQUIRE (STATUS_ERROR_CODE(Status) == 0x1234);
    }

    //
    // ALL
    //
    SECTION ("ALL") {
        Status = COMBINE_STATUS(STATUS_SEVERITY_ERROR, 1, 0x1234, 0x5678);

        REQUIRE (STATUS_SEVERITY(Status) == STATUS_SEVERITY_ERROR);
        REQUIRE (STATUS_USER_DEFINED(Status) == 1);
        REQUIRE (STATUS_FACILITY(Status) == 0x1234);
        REQUIRE (STATUS_ERROR_CODE(Status) == 0x5678);
    }
}