#ifndef __RUNTIME_LIB_H__
#define __RUNTIME_LIB_H__

#pragma once

#include "RuntimeLib/TypeDefs.h"

#include "RuntimeLib/Error.h"
#include "RuntimeLib/GUID.h"
#include "RuntimeLib/Memory.h"

STATIC_ASSERT (sizeof (INT8)   == 1, "Invalid size for INT8");
STATIC_ASSERT (sizeof (INT16)  == 2, "Invalid size for INT16");
STATIC_ASSERT (sizeof (INT32)  == 4, "Invalid size for INT32");
STATIC_ASSERT (sizeof (INT64)  == 8, "Invalid size for INT64");

STATIC_ASSERT (sizeof (UINT8)  == 1, "Invalid size for UINT8");
STATIC_ASSERT (sizeof (UINT16) == 2, "Invalid size for UINT16");
STATIC_ASSERT (sizeof (UINT32) == 4, "Invalid size for UINT32");
STATIC_ASSERT (sizeof (UINT64) == 8, "Invalid size for UINT64");

STATIC_ASSERT (sizeof (UINTN)  == sizeof (VOID*), "Invalid size for UINTN");
STATIC_ASSERT (sizeof (INTN)   == sizeof (VOID*), "Invalid size for INTN");

#endif // __RUNTIME_LIB_H__
