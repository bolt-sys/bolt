#pragma once

#ifndef __KERNEL_H__
    #error "This file should be included from Kernel.h"
#endif

#include <RuntimeLib.h>

#define LOG_LEVEL_DEBUG   0
#define LOG_LEVEL_INFO    1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_ERROR   3

/**
 * @brief      Logs a message.
 *
 * @param[in]  _ID     The Filter ID. LFID_DEFAULT for default filter.
 * @param[in]  _LEVEL  The log level.
 * @param[in]  _Format The format string.
 **/
#define LOG(_ID, _LEVEL, _Format, ...)

/// Default filter, used to everything that doesn't have a specific filter.
DEFINE_GUID (LFID_DEFAULT, 0x8BA890B8, 0xF167, 0x4AFD, 0x86, 0x45, 0x44, 0x07, 0x43, 0xA0, 0x30, 0x6A);

/// Memory filter, used to log memory related messages.
DEFINE_GUID (LFID_MEMORY, 0x8BA890B8, 0xF167, 0x4AFD, 0x86, 0x45, 0x44, 0x07, 0x43, 0xA0, 0x30, 0x6B);

typedef struct _LOG_FILTER {
    GUID    ID;
    CHAR8   Name[64];
    BOOLEAN Enabled;
    UINT8   Level;
} LOG_FILTER;

typedef struct _LOG_ENTRY {
    GUID   FilterID;
    UINTN  Level;
    CHAR8* Message;
} LOG_ENTRY;

typedef VOID (* LOG_CALLBACK)(
    LOG_ENTRY* Entry
    );
