#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "json_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @file json_parser.h
     * @brief Declares the JSON parser API.
     *
     * This header file provides the interface for parsing JSON strings into
     * JsonValue structures and for freeing the allocated JSON data structures.
     */

    /**
     * @brief Parses a JSON string and constructs a JsonValue data structure.
     *
     * This function takes a null-terminated JSON string and parses it into
     * a hierarchical JsonValue structure that represents the JSON data.
     *
     * @param[in] json The null-terminated JSON string to parse.
     * @return Pointer to the root JsonValue if parsing is successful, NULL otherwise.
     *
     * @note It is the caller's responsibility to free the returned JsonValue using `json_free_value` to avoid memory leaks.
     */
    JsonValue *json_parse(const char *json);

    /**
     * @brief Frees the memory allocated for a JsonValue and its nested structures.
     *
     * This function recursively frees all memory associated with a JsonValue,
     * including any nested JsonObjects or JsonArrays.
     *
     * @param[in,out] value Pointer to the JsonValue to free.
     *
     * @note After calling this function, the pointer should not be used unless it is reassigned.
     */
    void json_free_value(JsonValue *value);

#ifdef __cplusplus
}
#endif

#endif // JSON_PARSER_H
