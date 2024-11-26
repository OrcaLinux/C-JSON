#ifndef JSON_ACCESSOR_H
#define JSON_ACCESSOR_H

#include "json_types.h"
#include <stdbool.h>

/**
 * @file json_accessor.h
 * @brief Declares the JSON accessor API.
 *
 * This header file provides utility functions to access specific data
 * within the JSON parse tree, such as retrieving strings, numbers, booleans,
 * arrays, and nested objects by their keys.
 */

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Retrieves a string value from a JSON object by key.
     *
     * @param[in] object Pointer to the JsonValue object (must be of type JSON_OBJECT).
     * @param[in] key    The key string to search for.
     * @return Pointer to the string if found and is of type JSON_STRING, NULL otherwise.
     */
    const char *json_get_string(const JsonValue *object, const char *key);

    /**
     * @brief Retrieves a number value from a JSON object by key.
     *
     * @param[in] object Pointer to the JsonValue object (must be of type JSON_OBJECT).
     * @param[in] key    The key string to search for.
     * @return The number value if found and is of type JSON_NUMBER, 0.0 otherwise.
     */
    double json_get_number(const JsonValue *object, const char *key);

    /**
     * @brief Retrieves a boolean value from a JSON object by key.
     *
     * @param[in] object Pointer to the JsonValue object (must be of type JSON_OBJECT).
     * @param[in] key    The key string to search for.
     * @return true if the value is true and of type JSON_BOOL, false otherwise.
     */
    bool json_get_bool(const JsonValue *object, const char *key);

    /**
     * @brief Retrieves an array from a JSON object by key.
     *
     * @param[in] object Pointer to the JsonValue object (must be of type JSON_OBJECT).
     * @param[in] key    The key string to search for.
     * @return Pointer to the JsonValue array if found and is of type JSON_ARRAY, NULL otherwise.
     */
    JsonValue *json_get_array(const JsonValue *object, const char *key);

    /**
     * @brief Retrieves a nested object from a JSON object by key.
     *
     * @param[in] object Pointer to the JsonValue object (must be of type JSON_OBJECT).
     * @param[in] key    The key string to search for.
     * @return Pointer to the nested JsonValue object if found and is of type JSON_OBJECT, NULL otherwise.
     */
    JsonValue *json_get_object(const JsonValue *object, const char *key);

    /**
     * @brief Checks if a value in a JSON object by key is null.
     *
     * @param[in] object Pointer to the JsonValue object (must be of type JSON_OBJECT).
     * @param[in] key    The key string to search for.
     * @return true if the value is null, false otherwise.
     */
    bool json_is_null(const JsonValue *object, const char *key);

#ifdef __cplusplus
}
#endif

#endif // JSON_ACCESSOR_H
