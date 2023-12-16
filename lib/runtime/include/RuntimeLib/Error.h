/**
 * @file Error.h
 * @brief BoltOS SDK error codes.
 *
 * This file defines macros and functions for creating and manipulating error codes in BoltOS SDK.
 * The error codes are 32-bit values with the following format:
 *
 *  +-------------+-------------+--------------+-------------+
 *  | Severity    | UserDefined | Facility     | ErrorCode   |
 *  +-------------+-------------+--------------+-------------+
 *  | 2 bits      | 1 bit       | 13 bits      | 16 bits     | << Length
 *  +-------------+-------------+--------------+-------------+
 *  | 30          | 29          | 16           | 0           | << Position
 *  +-------------+-------------+--------------+-------------+
 *  | 0x3         | 0x1         | 0x2000       | 0xFFFF      | << Max
 *  +-------------+-------------+--------------+-------------+
 *
 * The Severity field indicates the severity level of the error, such as success, informational, warning, or error.
 * The UserDefined field is reserved for application-defined error codes.
 * The Facility field identifies the source of the error, such as generic or memory.
 * The ErrorCode field is a unique identifier for the error.
 *
 * This file also defines macros and functions for extracting and checking the severity, facility, and error code
 * of an error code value.
 *
 * A simple use case of this file is as follows:
 *
 * @code
 * #include <RuntimeLib.h>
 *
 * STATUS MyFunction()
 * {
 *    STATUS Status;
 *    ...
 *
 *    Status = STATUS_SUCCESS;
 *    ...
 *
 *    // NOTE: following code can be replaced with the QUICK_CHECK macro.
 *    Status = MyOtherFunction();
 *    if (FAILED(Status)) {
 *       goto Exit;
 *    }
 *
 *    ...
 *
 * Exit:
 *    ...CleanupCode...
 *    return Status;
 * }
 * @endcode
 **/

#pragma once

#ifndef __RUNTIME_LIB_H__
    #error "This file should be included from RuntimeLib.h"
#endif

EXTERN_C_START

#define COMBINE_STATUS(_Severity, _UserDefined, _Facility, _ErrorCode) \
    (\
          (((STATUS)_Severity)    << 30)\
        | (((STATUS)_UserDefined) << 29)\
        | (((STATUS)_Facility)    << 16)\
        | (((STATUS)_ErrorCode)   <<  0)\
    )

#define STATUS_SEVERITY_SUCCESS        0b00
#define STATUS_SEVERITY_INFORMATIONAL  0b01
#define STATUS_SEVERITY_WARNING        0b10
#define STATUS_SEVERITY_ERROR          0b11

#define STATUS_FACILITY_GENERIC        0x0

// Reserved for system use.
#define STATUS_FACILITY_RESERVED_BEGIN 0x0
#define STATUS_FACILITY_RESERVED_END   0xFF

#define STATUS_SEVERITY(_Status)     ((_Status)  >> 30)
#define STATUS_USER_DEFINED(_Status) (((_Status) >> 29) & 0x1)
#define STATUS_FACILITY(_Status)     (((_Status) >> 16) & 0x1FFF)
#define STATUS_ERROR_CODE(_Status)   ((_Status)         & 0xFFFF)

#define IS_SUCCESS(_Status)          (STATUS_SEVERITY(_Status) == STATUS_SEVERITY_SUCCESS)
#define IS_WARNING(_Status)          (STATUS_SEVERITY(_Status) == STATUS_SEVERITY_WARNING)
#define IS_INFORMATIONAL(_Status)    (STATUS_SEVERITY(_Status) == STATUS_SEVERITY_INFORMATIONAL)
#define IS_ERROR(_Status)            (STATUS_SEVERITY(_Status) == STATUS_SEVERITY_ERROR)

#define SUCCEEDED(_Status)           (IS_SUCCESS(_Status) || IS_INFORMATIONAL(_Status) || IS_WARNING(_Status))
#define FAILED(_Status)              (IS_ERROR(_Status))

#define QUICK_CHECK(_F)              Status = (_F); if (FAILED(Status)) { goto Exit; }

// -------------------------------------------------------------------- Generic

#define STATUS_SUCCESS             COMBINE_STATUS(STATUS_SEVERITY_SUCCESS, 0,   STATUS_FACILITY_GENERIC, 0x00)
#define STATUS_FAILURE             COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x00)

// 0x01 - 0x0F (INVALID_*)
#define STATUS_INVALID_PARAMETER   COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x01)
#define STATUS_INVALID_HANDLE      COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x02)
#define STATUS_INVALID_STATE       COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x03)
#define STATUS_INVALID_ADDRESS     COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x04)
#define STATUS_INVALID_OPERATION   COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x05)
#define STATUS_INVALID_ALIGNMENT   COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x06)

// 0x10 - 0x1F (NOT_*)
#define STATUS_NOT_IMPLEMENTED     COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x10)
#define STATUS_NOT_SUPPORTED       COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x11)
#define STATUS_NOT_FOUND           COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x12)
#define STATUS_NOT_READY           COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x13)
#define STATUS_NOT_INITIALIZED     COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x14)

// 0x20 - 0x2F (ALREADY_*)
#define STATUS_ALREADY_EXISTS      COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x20)
#define STATUS_ALREADY_INITIALIZED COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x21)

// 0x30 - 0x3F (OUT_OF_*)
#define STATUS_OUT_OF_MEMORY       COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x30)
#define STATUS_OUT_OF_RESOURCES    COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x31)
#define STATUS_OUT_OF_BOUNDS       COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x32)
#define STATUS_OUT_OF_RANGE        COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x33)

// 0x40 - 0x4F (TOO_*)
#define STATUS_BUFFER_TOO_SMALL    COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x40)
#define STATUS_BUFFER_TOO_LARGE    COMBINE_STATUS(STATUS_SEVERITY_ERROR,   0,   STATUS_FACILITY_GENERIC, 0x41)

EXTERN_C_END
