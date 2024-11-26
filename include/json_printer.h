#ifndef JSON_PRINTER_H
#define JSON_PRINTER_H

#include "json_types.h"

/**
 * @file json_printer.h
 * @brief Declares the JSON printer API.
 *
 * This header file provides the interface for printing the JSON parse tree
 * in a formatted, human-readable manner.
 */

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Prints the JSON parse tree starting from the given JsonValue.
     *
     * This function traverses the JsonValue hierarchy and prints the JSON structure
     * with proper indentation for readability.
     *
     * @param[in] value  Pointer to the root JsonValue to print.
     * @param[in] indent Current indentation level (number of spaces).
     */
    void json_print(const JsonValue *value, int indent);

#ifdef __cplusplus
}
#endif

#endif // JSON_PRINTER_H
