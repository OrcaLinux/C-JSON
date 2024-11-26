#include "json_accessor.h"
#include <string.h>

/**
 * @brief Helper function to find a JsonPair by key in a JsonObject.
 *
 * @param[in] object Pointer to the JsonValue object (must be of type JSON_OBJECT).
 * @param[in] key    The key string to search for.
 * @return Pointer to the JsonPair if found, NULL otherwise.
 */
static JsonPair *find_pair(const JsonValue *object, const char *key)
{
    if (!object || object->type != JSON_OBJECT || !key)
    {
        return NULL;
    }

    for (size_t i = 0; i < object->value.object->count; i++)
    {
        if (strcmp(object->value.object->pairs[i].key, key) == 0)
        {
            return &(object->value.object->pairs[i]);
        }
    }

    return NULL;
}

const char *json_get_string(const JsonValue *object, const char *key)
{
    JsonPair *pair = find_pair(object, key);
    if (pair && pair->value->type == JSON_STRING)
    {
        return pair->value->value.string;
    }
    return NULL;
}

double json_get_number(const JsonValue *object, const char *key)
{
    JsonPair *pair = find_pair(object, key);
    if (pair && pair->value->type == JSON_NUMBER)
    {
        return pair->value->value.number;
    }
    return 0.0;
}

bool json_get_bool(const JsonValue *object, const char *key)
{
    JsonPair *pair = find_pair(object, key);
    if (pair && pair->value->type == JSON_BOOL)
    {
        return pair->value->value.boolean ? true : false;
    }
    return false;
}

JsonValue *json_get_array(const JsonValue *object, const char *key)
{
    JsonPair *pair = find_pair(object, key);
    if (pair && pair->value->type == JSON_ARRAY)
    {
        return pair->value;
    }
    return NULL;
}

JsonValue *json_get_object(const JsonValue *object, const char *key)
{
    JsonPair *pair = find_pair(object, key);
    if (pair && pair->value->type == JSON_OBJECT)
    {
        return pair->value;
    }
    return NULL;
}

bool json_is_null(const JsonValue *object, const char *key)
{
    JsonPair *pair = find_pair(object, key);
    if (pair && pair->value->type == JSON_NULL)
    {
        return true;
    }
    return false;
}
