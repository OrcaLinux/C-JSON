#ifndef JSON_TYPES_H
#define JSON_TYPES_H

#include <stddef.h>

/**
 * @file json_types.h
 * @brief Defines data structures representing JSON types.
 *
 * This header file contains the definitions of enumerations and structures
 * used to represent various JSON data types, such as objects, arrays, strings,
 * numbers, booleans, and null values.
 */

/**
 * @enum JsonType
 * @brief Enumerates the different types of JSON values.
 *
 * These types correspond to the possible data types that a JSON value can hold.
 */
typedef enum
{
    JSON_NULL,   /**< Represents a JSON null value. */
    JSON_BOOL,   /**< Represents a JSON boolean value (`true` or `false`). */
    JSON_NUMBER, /**< Represents a JSON number (integer or floating-point). */
    JSON_STRING, /**< Represents a JSON string. */
    JSON_ARRAY,  /**< Represents a JSON array. */
    JSON_OBJECT  /**< Represents a JSON object. */
} JsonType;

/**
 * @struct JsonValue
 * @brief Represents a JSON value, which can be of various types.
 *
 * The JsonValue structure uses a union to store different types of JSON values
 * based on the current JsonType.
 */
typedef struct JsonValue JsonValue;
typedef struct JsonArray JsonArray;
typedef struct JsonPair JsonPair;
typedef struct JsonObject JsonObject;

struct JsonValue
{
    JsonType type; /**< The type of the JSON value. */
    union
    {
        double number;      /**< Numeric value if type is JSON_NUMBER. */
        char *string;       /**< String value if type is JSON_STRING. */
        int boolean;        /**< Boolean value if type is JSON_BOOL. */
        JsonArray *array;   /**< Array value if type is JSON_ARRAY. */
        JsonObject *object; /**< Object value if type is JSON_OBJECT. */
    } value;                /**< The value data based on the type. */
};

/**
 * @struct JsonPair
 * @brief Represents a key-value pair within a JSON object.
 *
 * Each JsonPair consists of a key (string) and a corresponding JsonValue.
 */
struct JsonPair
{
    char *key;        /**< The key string. */
    JsonValue *value; /**< Pointer to the corresponding JSON value. */
};

/**
 * @struct JsonObject
 * @brief Represents a JSON object containing multiple JsonPair elements.
 *
 * A JsonObject consists of an array of JsonPairs and a count of how many pairs it contains.
 */
struct JsonObject
{
    JsonPair *pairs; /**< Array of key-value pairs. */
    size_t count;    /**< Number of key-value pairs. */
};

/**
 * @struct JsonArray
 * @brief Represents a JSON array containing multiple JsonValue elements.
 *
 * A JsonArray consists of an array of JsonValues and a count of how many values it contains.
 */
struct JsonArray
{
    JsonValue **items; /**< Dynamic array of pointers to JSON values. */
    size_t count;      /**< Number of items in the array. */
};

#endif // JSON_TYPES_H
