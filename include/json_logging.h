#ifndef JSON_LOGGING_H
#define JSON_LOGGING_H

#include "json_config.h"

/**
 * @brief Conditional DEBUG print macro.
 *
 * Prints debug information if DEBUG_ENABLED is set to a non-zero value.
 * This allows enabling or disabling debug output at compile-time.
 *
 * @param fmt The format string for the debug message.
 * @param ... Additional arguments for the format string.
 */
#define DEBUG_PRINT(fmt, ...)           \
    do                                  \
    {                                   \
        if (DEBUG_ENABLED)              \
            printf(fmt, ##__VA_ARGS__); \
    } while (0)

/**
 * @brief Conditional error logging macro.
 *
 * Logs error messages to `stderr` if ERROR_LOGGING_ENABLED is set to a non-zero value.
 * This allows enabling or disabling error output at compile-time, providing flexibility
 * for error reporting without affecting the rest of the code.
 *
 * @param fmt The format string for the error message.
 * @param ... Additional arguments for the format string.
 */
#define ERROR_LOG(fmt, ...)                      \
    do                                           \
    {                                            \
        if (ERROR_LOGGING_ENABLED)               \
            fprintf(stderr, fmt, ##__VA_ARGS__); \
    } while (0)

#endif // JSON_LOGGING_H
