// src/json_parser.c

#include "json_parser.h"
#include "json_tokenizer.h"
#include "json_utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static JsonValue *parse_value(JsonTokenizer *tokenizer);
static JsonValue *parse_object(JsonTokenizer *tokenizer);
static JsonValue *parse_array(JsonTokenizer *tokenizer);

static JsonValue *parse_string(JsonTokenizer *tokenizer, const char *str)
{
    printf("Parser: Parsing string: '%s'\n", str);
    (void)tokenizer; // Suppress unused parameter warning
    JsonValue *value = json_alloc(sizeof(JsonValue));
    if (!value)
    {
        printf("Parser: Memory allocation failed for JsonValue (STRING)\n");
        return NULL;
    }
    value->type = JSON_STRING;
    value->string = json_strdup(str);
    if (!value->string)
    {
        printf("Parser: Memory allocation failed for string value '%s'\n", str);
        json_free(value);
        return NULL;
    }
    return value;
}

static JsonValue *parse_number(JsonTokenizer *tokenizer, const char *num_str)
{
    printf("Parser: Parsing number: '%s'\n", num_str);
    (void)tokenizer; // Suppress unused parameter warning
    JsonValue *value = json_alloc(sizeof(JsonValue));
    if (!value)
    {
        printf("Parser: Memory allocation failed for JsonValue (NUMBER)\n");
        return NULL;
    }
    value->type = JSON_NUMBER;
    value->number = atof(num_str);
    return value;
}

static JsonValue *parse_true(JsonTokenizer *tokenizer)
{
    printf("Parser: Parsing true\n"); // Debug
    (void)tokenizer;                  // Suppress unused parameter warning
    JsonValue *value = json_alloc(sizeof(JsonValue));
    if (!value)
    {
        printf("Parser: Memory allocation failed for JsonValue (TRUE)\n"); // Debug
        return NULL;
    }
    value->type = JSON_BOOL;
    value->boolean = 1;
    return value;
}

static JsonValue *parse_false(JsonTokenizer *tokenizer)
{
    printf("Parser: Parsing false\n"); // Debug
    (void)tokenizer;                   // Suppress unused parameter warning
    JsonValue *value = json_alloc(sizeof(JsonValue));
    if (!value)
    {
        printf("Parser: Memory allocation failed for JsonValue (FALSE)\n"); // Debug
        return NULL;
    }
    value->type = JSON_BOOL;
    value->boolean = 0;
    return value;
}

static JsonValue *parse_null(JsonTokenizer *tokenizer)
{
    printf("Parser: Parsing null\n"); // Debug
    (void)tokenizer;                  // Suppress unused parameter warning
    JsonValue *value = json_alloc(sizeof(JsonValue));
    if (!value)
    {
        printf("Parser: Memory allocation failed for JsonValue (NULL)\n"); // Debug
        return NULL;
    }
    value->type = JSON_NULL;
    return value;
}

static JsonValue *parse_object(JsonTokenizer *tokenizer)
{
    printf("Parser: Starting to parse object.\n");
    JsonValue *object = json_alloc(sizeof(JsonValue));
    if (!object)
    {
        printf("Parser: Memory allocation failed for JsonValue (OBJECT)\n");
        return NULL;
    }
    object->type = JSON_OBJECT;
    object->object = json_alloc(sizeof(JsonObject));
    if (!object->object)
    {
        printf("Parser: Memory allocation failed for JsonObject\n");
        json_free(object);
        return NULL;
    }
    object->object->pairs = NULL;
    object->object->size = 0;

    while (1)
    {
        JsonToken token = json_get_next_token(tokenizer);

        if (token.type == TOKEN_RIGHT_BRACE)
        {
            printf("Parser: Object parsing complete.\n");
            json_token_free(&token);
            break;
        }

        // If not the first key-value pair, expect a comma
        if (object->object->size > 0)
        {
            if (token.type != TOKEN_COMMA)
            {
                printf("Parser: Expected TOKEN_COMMA between key-value pairs, but got %s\n", json_token_type_to_string(token.type));
                json_token_free(&token);
                json_free_value(object);
                return NULL;
            }
            json_token_free(&token);
            token = json_get_next_token(tokenizer);

            if (token.type == TOKEN_RIGHT_BRACE)
            {
                printf("Parser: Object parsing complete.\n");
                json_token_free(&token);
                break;
            }
        }

        // Now, expect a key string
        if (token.type != TOKEN_STRING)
        {
            printf("Parser: Expected TOKEN_STRING for key, but got %s\n", json_token_type_to_string(token.type));
            json_token_free(&token);
            json_free_value(object);
            return NULL;
        }

        // Duplicate the key string to ensure safe memory management
        char *key = json_strdup(token.value);
        if (!key)
        {
            printf("Parser: Memory allocation failed for key '%s'\n", token.value);
            json_token_free(&token);
            json_free_value(object);
            return NULL;
        }
        printf("Parser: Object key: '%s'\n", key);
        json_token_free(&token);

        // Expect colon
        token = json_get_next_token(tokenizer);
        printf("Parser: Received token: %s\n", json_token_type_to_string(token.type));
        if (token.type != TOKEN_COLON)
        {
            printf("Parser: Expected TOKEN_COLON after key '%s', but got %s\n", key, json_token_type_to_string(token.type));
            json_free(key);
            json_token_free(&token);
            json_free_value(object);
            return NULL;
        }
        json_token_free(&token); // Successfully processed the colon
        printf("Parser: Successfully processed colon. Parsing value for key: '%s'\n", key);

        // Parse value
        JsonValue *value = parse_value(tokenizer);
        if (!value)
        {
            printf("Parser: Failed to parse value for key '%s'\n", key);
            json_free(key);
            json_free_value(object);
            return NULL;
        }

        // Add key-value pair to object
        JsonPair *new_pairs = json_realloc(object->object->pairs, sizeof(JsonPair) * (object->object->size + 1));
        if (!new_pairs)
        {
            printf("Parser: Memory allocation failed for JsonPair array.\n");
            json_free(key);
            json_free_value(value);
            json_free_value(object);
            return NULL;
        }
        object->object->pairs = new_pairs;
        object->object->pairs[object->object->size].key = key;
        object->object->pairs[object->object->size].value = value;
        object->object->size++;
        printf("Parser: Added key-value pair: '%s': <value>\n", key);
    }

    return object;
}

static JsonValue *parse_array(JsonTokenizer *tokenizer)
{
    printf("Parser: Parsing array\n"); // Debug
    JsonValue *array = json_alloc(sizeof(JsonValue));
    if (!array)
    {
        printf("Parser: Memory allocation failed for JsonValue (ARRAY)\n"); // Debug
        return NULL;
    }
    array->type = JSON_ARRAY;
    array->array = json_alloc(sizeof(JsonArray));
    if (!array->array)
    {
        printf("Parser: Memory allocation failed for JsonArray\n"); // Debug
        json_free(array);
        return NULL;
    }
    array->array->values = NULL;
    array->array->size = 0;

    while (1)
    {
        JsonToken token = json_get_next_token(tokenizer);

        if (token.type == TOKEN_RIGHT_BRACKET)
        {
            printf("Parser: Array parsing complete.\n");
            json_token_free(&token);
            break;
        }

        // If not the first element, expect a comma
        if (array->array->size > 0)
        {
            if (token.type != TOKEN_COMMA)
            {
                printf("Parser: Expected TOKEN_COMMA between array elements, but got %s\n", json_token_type_to_string(token.type));
                json_token_free(&token);
                json_free_value(array);
                return NULL;
            }
            json_token_free(&token);
            token = json_get_next_token(tokenizer);

            if (token.type == TOKEN_RIGHT_BRACKET)
            {
                printf("Parser: Array parsing complete.\n");
                json_token_free(&token);
                break;
            }
        }

        // Parse value
        JsonValue *value = parse_value(tokenizer);
        if (!value)
        {
            printf("Parser: Failed to parse value in array\n"); // Debug
            json_free_value(array);
            return NULL;
        }

        // Add value to array
        JsonValue **new_values = json_realloc(array->array->values, sizeof(JsonValue *) * (array->array->size + 1));
        if (!new_values)
        {
            printf("Parser: Memory allocation failed for JsonValue array\n"); // Debug
            json_free_value(value);
            json_free_value(array);
            return NULL;
        }
        array->array->values = new_values;
        array->array->values[array->array->size] = value; // Store pointer directly
        array->array->size++;
        printf("Parser: Added value to array\n"); // Debug
    }

    return array;
}

static JsonValue *parse_value(JsonTokenizer *tokenizer)
{
    JsonToken token = json_get_next_token(tokenizer);
    JsonValue *value = NULL;

    printf("Parser: Entering parse_value. Received token: %s\n", json_token_type_to_string(token.type));

    switch (token.type)
    {
    case TOKEN_LEFT_BRACE:
        printf("Parser: Detected object\n");
        value = parse_object(tokenizer);
        break;
    case TOKEN_LEFT_BRACKET:
        printf("Parser: Detected array\n");
        value = parse_array(tokenizer);
        break;
    case TOKEN_STRING:
        printf("Parser: Detected string '%s'\n", token.value);
        value = parse_string(tokenizer, token.value);
        break;
    case TOKEN_NUMBER:
        printf("Parser: Detected number '%s'\n", token.value);
        value = parse_number(tokenizer, token.value);
        break;
    case TOKEN_TRUE:
        printf("Parser: Detected true\n");
        value = parse_true(tokenizer);
        break;
    case TOKEN_FALSE:
        printf("Parser: Detected false\n");
        value = parse_false(tokenizer);
        break;
    case TOKEN_NULL:
        printf("Parser: Detected null\n");
        value = parse_null(tokenizer);
        break;
    default:
        printf("Parser: Detected unexpected or error token: %s\n", json_token_type_to_string(token.type));
        value = NULL;
        break;
    }

    json_token_free(&token);
    return value;
}

JsonValue *json_parse(const char *json)
{
    printf("Parser: Starting JSON parsing...\n");
    JsonTokenizer tokenizer;
    json_tokenizer_init(&tokenizer, json);

    JsonValue *root = parse_value(&tokenizer);
    if (root)
    {
        JsonToken token = json_get_next_token(&tokenizer);
        printf("Parser: Checking for extra data after root. Token: %s\n", json_token_type_to_string(token.type));
        if (token.type != TOKEN_EOF)
        {
            printf("Parser: Extra data detected after JSON root.\n");
            json_free_value(root);
            root = NULL;
        }
        else
        {
            printf("Parser: JSON parsing completed successfully.\n");
        }
        json_token_free(&token);
    }
    else
    {
        printf("Parser: Failed to parse JSON.\n");
    }

    return root;
}

void json_free_value(JsonValue *value)
{
    if (!value)
        return;

    switch (value->type)
    {
    case JSON_STRING:
        json_free(value->string);
        break;
    case JSON_ARRAY:
        for (size_t i = 0; i < value->array->size; i++)
        {
            json_free_value(value->array->values[i]); // Correctly free each JsonValue* in the array
        }
        json_free(value->array->values);
        json_free(value->array);
        break;
    case JSON_OBJECT:
        for (size_t i = 0; i < value->object->size; i++)
        {
            json_free(value->object->pairs[i].key);
            json_free_value(value->object->pairs[i].value); // Correctly free each JsonValue* in the object
        }
        json_free(value->object->pairs);
        json_free(value->object);
        break;
    default:
        break;
    }

    // Only free the JsonValue struct itself if it's dynamically allocated (e.g., root)
    // In this implementation, all JsonValue structs passed to json_free_value are assumed to be dynamically allocated
    json_free(value);
}
