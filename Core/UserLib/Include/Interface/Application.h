#pragma once

#include <Base.h>

// -------------------------------------------------------------------- Defines

#define COMMAND_LINE_PROTOCOL_REVISION         0
#define ENVIRONMENT_VARIABLE_PROTOCOL_REVISION 0
#define APPLICATION_IMAGE_PROTOCOL_REVISION    0

// 152B9E38-9050-4C9B-ACA3-EC06893E6004
extern GUID COMMAND_LINE_PROTOCOL_GUID;

// 8EAA43F4-556E-4EB0-98F3-04EF8AA9920F
extern GUID ENVIRONMENT_VARIABLE_PROTOCOL_GUID;

// D3F2073D-43B0-4BA1-AAB4-4689572E1FB2
extern GUID APPLICATION_IMAGE_PROTOCOL_GUID;

// ---------------------------------------------------------------------- Types

typedef struct COMMAND_LINE_DATA {
    // \Revision 0
    UINT32 Length;
    CHAR8  Arguments[0]; // Arguments[Length]
} PROTOCOL_COMMAND_LINE_DATA;

typedef struct ENVIRONMENT_VARIABLE_DATA {
    // \Revision 0
    UINT32 Length;
    CHAR8  Variables[0]; // Variables[Length]
} PROTOCOL_ENVIRONMENT_VARIABLE_DATA;

typedef struct APPLICATION_IMAGE_DATA {
    UINTN ImageSize;
    VOID* ImageBase;
    VOID* EntryPoint;
} PROTOCOL_APPLICATION_IMAGE_DATA;
