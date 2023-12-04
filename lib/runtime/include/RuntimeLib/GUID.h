/**
 * @file GUID.h
 * @brief This file contains the definition of the GUID structure and related macros.
 *
 * A GUID is a 128-bit integer that can be used to uniquely identify an object.
 * This file defines the structure of a GUID and provides macros to define, compare, and manipulate GUIDs.
 * It also defines a pre-defined GUID with all bits set to zero, which is used to represent a NULL GUID.
 **/

#pragma once

#ifndef __RUNTIME_LIB_H__
    #error "This file should be included from RuntimeLib.h"
#endif

EXTERN_C_START

/**
 * @brief A structure representing a globally unique identifier (GUID).
 *
 * A GUID is a 128-bit integer that can be used to uniquely identify an object.
 * similar to an UUID, but with arbitrary data.
 *
 **/
typedef struct _GUID {
    UINT32 Data1;    /**< The first 32-bit integer of the GUID.  */
    UINT16 Data2;    /**< The first 16-bit integer of the GUID.  */
    UINT16 Data3;    /**< The second 16-bit integer of the GUID. */
    UINT8  Data4[8]; /**< The last 64 bits of the GUID.          */
} GUID;

/**
 * @brief Macro to define a GUID. each
 *
 * @param[in] _NAME    The name of the GUID.
 * @param[in] _L       The first 32-bit integer of the GUID.
 * @param[in] _W1      The first 16-bit integer of the GUID.
 * @param[in] _W2      The second 16-bit integer of the GUID.
 * @param[in] _B1.._B8 The last 64 bits of the GUID.
 *
 * it will be defined as a const GUID with the given name.
 **/
#define DEFINE_GUID(_NAME, _L, _W1, _W2, _B1, _B2, _B3, _B4, _B5, _B6, _B7, _B8) \
    static const GUID _NAME = { _L, _W1, _W2, { _B1, _B2, _B3, _B4, _B5, _B6, _B7, _B8 } }

/**
 * @brief Macro to compare two GUIDs for equality.
 *
 * @param[in] _G1 The first GUID to compare.
 * @param[in] _G2 The second GUID to compare.
 *
 * @return TRUE if the GUIDs are equal, FALSE otherwise.
 **/
#define GUID_EQ(_G1, _G2) \
    ((_G1).Data1    == (_G2).Data1    && (_G1).Data2    == (_G2).Data2    && (_G1).Data3    == (_G2).Data3    && \
     (_G1).Data4[0] == (_G2).Data4[0] && (_G1).Data4[1] == (_G2).Data4[1] && (_G1).Data4[2] == (_G2).Data4[2] && \
     (_G1).Data4[3] == (_G2).Data4[3] && (_G1).Data4[4] == (_G2).Data4[4] && (_G1).Data4[5] == (_G2).Data4[5] && \
     (_G1).Data4[6] == (_G2).Data4[6] && (_G1).Data4[7] == (_G2).Data4[7])

/**
 * @brief Macro to compare two GUIDs for inequality.
 *
 * @param[in] _G1 The first GUID to compare.
 * @param[in] _G2 The second GUID to compare.
 *
 * @return TRUE if the GUIDs are not equal, FALSE otherwise.
 **/
#define GUID_NE(_G1, _G2) (!GUID_EQ(_G1, _G2))

/**
 * @brief A pre-defined GUID with all bits set to zero.
 *
 * This GUID is used to represent a NULL GUID, similar to a NULL pointer.
 *
 **/
DEFINE_GUID (GUID_NULL, 0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

EXTERN_C_END
