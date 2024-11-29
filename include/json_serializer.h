#ifndef JSON_SERIALIZER_H
#define JSON_SERIALIZER_H

#include "json_types.h"

/**
 * @file json_serializer.h
 * @brief Declares the JSON serializer API.
 *
 * This header file provides the interface for serializing JsonValue structures
 * into JSON-formatted strings.
 */

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Serializes a JsonValue into a JSON-formatted string.
     *
     * This function recursively converts a JsonValue into a string,
     * handling different JSON types appropriately.
     *
     * @param[in] value The JsonValue to serialize.
     * @return A dynamically allocated string representing the JsonValue.
     *         The caller is responsible for freeing the returned string.
     */
    char *json_serialize(const JsonValue *value);

#ifdef __cplusplus
}
#endif

#endif // JSON_SERIALIZER_H
