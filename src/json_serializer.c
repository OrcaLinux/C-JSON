// json_serializer.c

#include "json_serializer.h"
#include "json_utils.h"
#include "json_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Helper function to escape special characters in a string for JSON serialization.
 *
 * @param[in] str The input string to escape.
 * @return A new dynamically allocated string with escaped characters.
 *         The caller must free the returned string.
 */
static char *escape_string(const char *str)
{
    if (!str)
        return json_strdup("");

    size_t len = 0;
    const char *p = str;

    // First pass: calculate the length of the escaped string
    while (*p)
    {
        switch (*p)
        {
        case '\"':
        case '\\':
        case '/':
        case '\b':
        case '\f':
        case '\n':
        case '\r':
        case '\t':
            len += 2; // These characters become two-character sequences
            break;
        default:
            if ((unsigned char)*p < 0x20)
            {
                len += 6; // Control characters become \uXXXX
            }
            else
            {
                len += 1;
            }
            break;
        }
        p++;
    }

    // Allocate memory for the escaped string
    char *escaped_str = json_alloc(len + 1); // +1 for null terminator
    if (!escaped_str)
        return NULL;

    const char *src = str;
    char *dst = escaped_str;

    // Second pass: copy characters and apply escaping
    while (*src)
    {
        switch (*src)
        {
        case '\"':
            *dst++ = '\\';
            *dst++ = '\"';
            break;
        case '\\':
            *dst++ = '\\';
            *dst++ = '\\';
            break;
        case '/':
            *dst++ = '\\';
            *dst++ = '/';
            break;
        case '\b':
            *dst++ = '\\';
            *dst++ = 'b';
            break;
        case '\f':
            *dst++ = '\\';
            *dst++ = 'f';
            break;
        case '\n':
            *dst++ = '\\';
            *dst++ = 'n';
            break;
        case '\r':
            *dst++ = '\\';
            *dst++ = 'r';
            break;
        case '\t':
            *dst++ = '\\';
            *dst++ = 't';
            break;
        default:
            if ((unsigned char)*src < 0x20)
            {
                // Control characters (less than ASCII 0x20)
                sprintf(dst, "\\u%04x", (unsigned char)*src);
                dst += 6;
            }
            else
            {
                *dst++ = *src;
            }
            break;
        }
        src++;
    }

    *dst = '\0'; // Null-terminate the string

    return escaped_str;
}

char *json_serialize(const JsonValue *value)
{
    if (!value)
        return json_strdup("null");

    char *result = NULL;
    char buffer[64];

    switch (value->type)
    {
    case JSON_STRING:
    {
        char *escaped_str = escape_string(value->value.string);
        if (!escaped_str)
            return NULL;
        size_t len = strlen(escaped_str) + 3; // For quotes and null terminator
        result = json_alloc(len);
        if (!result)
        {
            json_free(escaped_str);
            return NULL;
        }
        snprintf(result, len, "\"%s\"", escaped_str);
        json_free(escaped_str);
        break;
    }
    case JSON_NUMBER:
        snprintf(buffer, sizeof(buffer), "%g", value->value.number);
        result = json_strdup(buffer);
        break;
    case JSON_BOOL:
        result = json_strdup(value->value.boolean ? "true" : "false");
        break;
    case JSON_NULL:
        result = json_strdup("null");
        break;
    case JSON_OBJECT:
    {
        result = json_strdup("{");
        if (!result)
            return NULL;

        for (size_t i = 0; i < value->value.object->count; i++)
        {
            char *key = escape_string(value->value.object->pairs[i].key);
            char *val = json_serialize(value->value.object->pairs[i].value);
            if (!key || !val)
            {
                json_free(key);
                json_free(val);
                json_free(result);
                return NULL;
            }
            size_t new_len = strlen(result) + strlen(key) + strlen(val) + 5; // For quotes, colon, comma
            char *new_result = json_realloc(result, new_len);
            if (!new_result)
            {
                json_free(key);
                json_free(val);
                json_free(result);
                return NULL;
            }
            result = new_result;
            strcat(result, "\"");
            strcat(result, key);
            strcat(result, "\":");
            strcat(result, val);
            if (i < value->value.object->count - 1)
                strcat(result, ",");
            json_free(key);
            json_free(val);
        }
        // Add closing brace
        size_t final_len = strlen(result) + 2; // For closing brace and null terminator
        char *final_result = json_realloc(result, final_len);
        if (!final_result)
        {
            json_free(result);
            return NULL;
        }
        result = final_result;
        strcat(result, "}");
        break;
    }
    case JSON_ARRAY:
    {
        result = json_strdup("[");
        if (!result)
            return NULL;

        for (size_t i = 0; i < value->value.array->count; i++)
        {
            char *val = json_serialize(value->value.array->items[i]);
            if (!val)
            {
                json_free(result);
                return NULL;
            }
            size_t new_len = strlen(result) + strlen(val) + 2; // For comma
            char *new_result = json_realloc(result, new_len);
            if (!new_result)
            {
                json_free(val);
                json_free(result);
                return NULL;
            }
            result = new_result;
            strcat(result, val);
            if (i < value->value.array->count - 1)
                strcat(result, ",");
            json_free(val);
        }
        // Add closing bracket
        size_t final_len = strlen(result) + 2; // For closing bracket and null terminator
        char *final_result = json_realloc(result, final_len);
        if (!final_result)
        {
            json_free(result);
            return NULL;
        }
        result = final_result;
        strcat(result, "]");
        break;
    }
    default:
        result = json_strdup("null");
        break;
    }

    return result;
}
