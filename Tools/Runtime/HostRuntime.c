// Simple implementation of the host runtime functions.
// by utilizing libc and other OS specific functions.
//
// TODO: move this to an actual application loader once we have one.
//

#include <Base.h>

#include <RuntimeProtocol.h>
#include <Interface/Application.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

EXTERN
VOID
SYSAPI
Main (
    PROTOCOL_STACK* ProtocolStack
    );

VOID
DumpProtocolStack (
    PROTOCOL_STACK* ProtocolStack
    )
{
    PROTOCOL* Protocol = NULL;
    UINTN     Offset   = 0;
    UINTN     Index    = 0;

    printf ("ProtocolStack->Count: %llu\n", ProtocolStack->Count);

    while (IterateProtocolStack(ProtocolStack, &Index, &Offset, &Protocol)) {
        printf (
            "ProtocolStack->Protocol[%llu]: {%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}\n",
            Index,
            Protocol->Guid.Data1,
            Protocol->Guid.Data2,
            Protocol->Guid.Data3,
            Protocol->Guid.Data4[0],
            Protocol->Guid.Data4[1],
            Protocol->Guid.Data4[2],
            Protocol->Guid.Data4[3],
            Protocol->Guid.Data4[4],
            Protocol->Guid.Data4[5],
            Protocol->Guid.Data4[6],
            Protocol->Guid.Data4[7]
        );
        
        printf ("  ->Revision: %hu\n", Protocol->Revision);
        printf ("  ->DataFlags: %hhu\n", Protocol->DataFlags);
        printf ("  ->Reserved: %hhu\n", Protocol->Reserved);
        printf ("  ->DataLength: %u\n", Protocol->DataLength);
        
        if (GUID_EQ (Protocol->Guid, COMMAND_LINE_PROTOCOL_GUID)) {
            PROTOCOL_COMMAND_LINE_DATA* CommandLineData = PROTOCOL_DATA(PROTOCOL_COMMAND_LINE_DATA, Protocol);
            printf ("  ->CommandLineData:\n");
            printf ("    ->Length: %u\n", CommandLineData->Length);
            printf ("    ->Arguments: %s\n", CommandLineData->Arguments);
        } else if (GUID_EQ (Protocol->Guid, ENVIRONMENT_VARIABLE_PROTOCOL_GUID)) {
            PROTOCOL_ENVIRONMENT_VARIABLE_DATA* EnvironmentVariableData = PROTOCOL_DATA(PROTOCOL_ENVIRONMENT_VARIABLE_DATA, Protocol);
            printf ("  ->EnvironmentVariableData:\n");
            printf ("    ->Length: %u\n", EnvironmentVariableData->Length);
            printf ("    ->Variables: %s\n", EnvironmentVariableData->Variables);
        } else if (GUID_EQ (Protocol->Guid, APPLICATION_IMAGE_PROTOCOL_GUID)) {
            PROTOCOL_APPLICATION_IMAGE_DATA* ApplicationImageData = PROTOCOL_DATA(PROTOCOL_APPLICATION_IMAGE_DATA, Protocol);
            printf ("  ->ApplicationImageData:\n");
            printf ("    ->ImageSize: %llu\n", ApplicationImageData->ImageSize);
            printf ("    ->ImageBase: %p\n", ApplicationImageData->ImageBase);
            printf ("    ->EntryPoint: %p\n", ApplicationImageData->EntryPoint);
        }
    }
}

int
main (
    int   argc,
    char* argv[],
    char* envp[]
    )
{
    printf ("---- Running %s on the host ----\n", argv[0]);

    // TODO: Simplify this code.
    
    UINTN TotalLength = 0;
    
    UINTN CommandLineLength = 0;
    for (int i = 0; i < argc; ++i) {
        CommandLineLength += strlen (argv[i]) + 2; // +2 for the separator and the null terminator.
    }

    UINTN EnvironmentVariableLength = 0;
    for (int i = 0; envp[i] != NULL; ++i) {
        EnvironmentVariableLength += strlen (envp[i]) + 2; // +2 For the separator and the null terminator.
    }

    TotalLength += sizeof (PROTOCOL_STACK);

    TotalLength += sizeof (PROTOCOL);
    TotalLength += sizeof (PROTOCOL_COMMAND_LINE_DATA);
    TotalLength += CommandLineLength;

    TotalLength += sizeof (PROTOCOL);
    TotalLength += sizeof (PROTOCOL_ENVIRONMENT_VARIABLE_DATA);
    TotalLength += EnvironmentVariableLength;

    TotalLength += sizeof (PROTOCOL);
    TotalLength += sizeof (PROTOCOL_APPLICATION_IMAGE_DATA);

    // Let's begin emulating the PROTOCOL_STACK.
    // We will allocate a buffer on the stack and fill it with the protocols.

    PROTOCOL_STACK* ProtocolStack = malloc (TotalLength);

    if (ProtocolStack == NULL) {
        printf ("Failed to allocate memory for the protocol stack.\n");
        return 1;
    }

    ProtocolStack->Count = 3;

    UINTN offset = sizeof (PROTOCOL_STACK);

    for (UINTN i = 0; i < ProtocolStack->Count; i++) {
        PROTOCOL*                           Protocol;
        PROTOCOL_APPLICATION_IMAGE_DATA*    ApplicationImageData;
        PROTOCOL_ENVIRONMENT_VARIABLE_DATA* EnvironmentVariableData;
        PROTOCOL_COMMAND_LINE_DATA*         CommandLineData;

        Protocol = (PROTOCOL*)((UINT8*)ProtocolStack + offset);

        ApplicationImageData    = (PROTOCOL_APPLICATION_IMAGE_DATA*)(Protocol->Data);
        EnvironmentVariableData = (PROTOCOL_ENVIRONMENT_VARIABLE_DATA*)(Protocol->Data);
        CommandLineData         = (PROTOCOL_COMMAND_LINE_DATA*)(Protocol->Data);

        switch (i) {
            case 0:
                Protocol->Guid          = COMMAND_LINE_PROTOCOL_GUID;
                Protocol->Revision      = COMMAND_LINE_PROTOCOL_REVISION;
                Protocol->DataFlags     = PDF_NONE;
                Protocol->Reserved      = 0;
                Protocol->DataLength    = sizeof (PROTOCOL_COMMAND_LINE_DATA) + CommandLineLength;
                CommandLineData->Length = CommandLineLength;

                for (int j = 0; j < argc; ++j) {
                    strcat (CommandLineData->Arguments, argv[j]);

                    if (j + 1 < argc) {
                        strcat (CommandLineData->Arguments, " ");
                    }
                }

                break;

            case 1:
                Protocol->Guid                  = ENVIRONMENT_VARIABLE_PROTOCOL_GUID;
                Protocol->Revision              = ENVIRONMENT_VARIABLE_PROTOCOL_REVISION;
                Protocol->DataFlags             = PDF_NONE;
                Protocol->Reserved              = 0;
                Protocol->DataLength            = sizeof (PROTOCOL_ENVIRONMENT_VARIABLE_DATA) + EnvironmentVariableLength;
                EnvironmentVariableData->Length = EnvironmentVariableLength;
                for (int j = 0; envp[j] != NULL; ++j) {
                    strcat (EnvironmentVariableData->Variables, envp[j]);

                    if (envp[j + 1] != NULL) {
                        strcat (EnvironmentVariableData->Variables, ";");
                    }
                }

                break;

            case 2:
                Protocol->Guid                   = APPLICATION_IMAGE_PROTOCOL_GUID;
                Protocol->Revision               = APPLICATION_IMAGE_PROTOCOL_REVISION;
                Protocol->DataFlags              = PDF_NONE;
                Protocol->Reserved               = 0;
                Protocol->DataLength             = sizeof (PROTOCOL_APPLICATION_IMAGE_DATA);
                ApplicationImageData->ImageSize  = 0;    // TODO
                ApplicationImageData->ImageBase  = NULL; // TODO
                ApplicationImageData->EntryPoint = NULL; // TODO
                break;
                
            default:
                printf("HOW THE FUCK DID YOU GET HERE ?!\n");
                abort();
        }

        offset += sizeof (PROTOCOL) + Protocol->DataLength;
    }

    DumpProtocolStack (ProtocolStack);
    
    Main(ProtocolStack);

    return 0;
}
