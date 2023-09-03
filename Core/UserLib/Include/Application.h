#pragma once

#include <Base.h>

/**
 * @brief Get the whole command line.
 *
 * @attention The command line is a single string containing all arguments.
 *            It is usually being split by ' ' character, but depending on the API callee,
 *            it might be split by other characters.
 *
 * @param[in] ImageHandle A handle of the current image, is required to get specific information about the image.
 *
 * @return The whole command line as a single string.
 * @return NULL no command line is present.
 */
CHAR8*
SYSAPI
GetCommandLine (
    IN HANDLE ImageHandle
    );

/**
 * @brief Get the environment string.
 *
 * @Note The environment string is a single string containing all environment variables.
 *       it is being split by ';' characters.
 *
 * @param[in] ImageHandle A handle of the current image, is required to get specific information about the image.
 *
 * @return The environment string.
 * @return NULL no environment string is present.
 */
CHAR8*
SYSAPI
GetEnvironmentString (
    IN HANDLE ImageHandle
    );

/**
 * @brief Get an environment variable.
 *
 * @param[in]           ImageHandle A handle of the current image, is required to get specific information about the image.
 *
 * @param[in,optional]  Name        The name of the environment variable.
 *                                  NULL means the whole environment string,
 *                                  basically like `GetEnvironmentString()`.
 *
 * @param[out,optional] Value       The buffer to store the value of the environment variable.
 *                                  If NULL, ValueLength will be set to the length of the environment variable.
 *
 * @param[in,out]       ValueLength The length of the buffer pointed to by Value.
 *
 * @return STATUS_SUCCESS           Was found, copied to Value and ValueLength was set to the length of the environment variable.
 * @return STATUS_NOT_FOUND         The environment variable does not exist.
 * @return STATUS_BUFFER_TOO_SMALL  The buffer pointed to by Value is too small.
 *                                  ValueLength was set to the length of the environment variable.
 *                                  The caller can retry with a larger buffer.
 * @return STATUS_INVALID_PARAMETER ImageHandle or ValueLength is NULL.
 * @return STATUS_UNSUPPORTED       The environment variable is not supported in the current environment.
 */
STATUS
SYSAPI
GetEnvironmentVariable (
    IN     HANDLE                ImageHandle,
    IN     OPTIONAL CONST CHAR8* Name,
    OUT    OPTIONAL       CHAR8* Value,
    IN OUT                UINTN* ValueLength
    );

/**
 * @brief The entry point of the application.
 *
 * @param ImageHandle A handle of the current image, is required to get specific information about the image.
 *                    This parameter is passed by the Application Loader.
 *
 * @return STATUS_SUCCESS           The application exited successfully.
 * @return ANYTHING_ELSE            The application exited with an error.
 *                                  The return value is the error code.
 */
EXTERN
STATUS
SYSAPI
BoltMain (
    HANDLE ImageHandle
    );
