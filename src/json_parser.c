#include "json_parser.h"
#include "json_tokenizer.h"
#include "json_utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Parser State Structure */
typedef struct
{
    JsonTokenizer tokenizer; /**< The tokenizer instance. */
    JsonToken current_token; /**< The current token being processed. */
} ParserState;

/* Function Prototypes */
static JsonValue *parse_value(ParserState *state);
static JsonValue *parse_object(ParserState *state);
static JsonValue *parse_array(ParserState *state);
static JsonValue *parse_string(ParserState *state, const char *str);
static JsonValue *parse_number(ParserState *state, const char *num_str);
static JsonValue *parse_true(ParserState *state);
static JsonValue *parse_false(ParserState *state);
static JsonValue *parse_null(ParserState *state);

/**
 * @brief Advances to the next token.
 *
 * This function retrieves the next token from the tokenizer and updates the parser's state.
 *
 * @param[in,out] state Pointer to the ParserState instance.
 */
static void parser_advance(ParserState *state)
{
    json_token_free(&state->current_token);
    state->current_token = json_get_next_token(&state->tokenizer);
}

/**
 * @brief Expects the current token to be of a specific type and consumes it.
 *
 * If the current token matches the expected type, the parser advances to the next token.
 * Otherwise, it reports an error and terminates parsing.
 *
 * @param[in,out] state Pointer to the ParserState instance.
 * @param[in]     type  The expected JsonTokenType.
 */
static void parser_expect(ParserState *state, JsonTokenType type)
{
    if (state->current_token.type != type)
    {
        fprintf(stderr, "Parser Error at position %zu: Expected token %s but found %s\n",
                state->tokenizer.pos,
                json_token_type_to_string(type),
                json_token_type_to_string(state->current_token.type));
        json_free_value(NULL); // Adjust based on your cleanup strategy
        exit(EXIT_FAILURE);
    }
    parser_advance(state); // Consume the expected token
}

/**
 * @brief Parses a JSON string token.
 *
 * This function converts a string token into a JsonValue of type JSON_STRING.
 *
 * @param[in,out] state Pointer to the ParserState instance.
 * @param[in]     str   The string value to parse.
 * @return Pointer to the parsed JsonValue, or NULL on failure.
 */
static JsonValue *parse_string(ParserState *state, const char *str)
{
    (void)state;
    printf("Parser: Parsing string: '%s'\n", str);
    JsonValue *value = json_alloc(sizeof(JsonValue));
    if (!value)
    {
        fprintf(stderr, "Parser: Memory allocation failed for JsonValue (STRING)\n");
        return NULL;
    }
    value->type = JSON_STRING;
    value->value.string = json_strdup(str);
    if (!value->value.string)
    {
        fprintf(stderr, "Parser: Memory allocation failed for string value '%s'\n", str);
        json_free(value);
        return NULL;
    }
    return value;
}

/**
 * @brief Parses a JSON number token.
 *
 * This function converts a number token into a JsonValue of type JSON_NUMBER.
 *
 * @param[in,out] state Pointer to the ParserState instance.
 * @param[in]     num_str The number string to parse.
 * @return Pointer to the parsed JsonValue, or NULL on failure.
 */
static JsonValue *parse_number(ParserState *state, const char *num_str)
{
    (void)state;
    printf("Parser: Parsing number: '%s'\n", num_str);
    JsonValue *value = json_alloc(sizeof(JsonValue));
    if (!value)
    {
        fprintf(stderr, "Parser: Memory allocation failed for JsonValue (NUMBER)\n");
        return NULL;
    }
    value->type = JSON_NUMBER;
    value->value.number = atof(num_str);
    return value;
}

/**
 * @brief Parses a JSON boolean token (true).
 *
 * @param[in,out] state Pointer to the ParserState instance.
 * @return Pointer to the parsed JsonValue, or NULL on failure.
 */
static JsonValue *parse_true(ParserState *state)
{
    (void)state;
    printf("Parser: Parsing true\n");
    JsonValue *value = json_alloc(sizeof(JsonValue));
    if (!value)
    {
        fprintf(stderr, "Parser: Memory allocation failed for JsonValue (TRUE)\n");
        return NULL;
    }
    value->type = JSON_BOOL;
    value->value.boolean = 1;
    return value;
}

/**
 * @brief Parses a JSON boolean token (false).
 *
 * @param[in,out] state Pointer to the ParserState instance.
 * @return Pointer to the parsed JsonValue, or NULL on failure.
 */
static JsonValue *parse_false(ParserState *state)
{
    (void)state;
    printf("Parser: Parsing false\n");
    JsonValue *value = json_alloc(sizeof(JsonValue));
    if (!value)
    {
        fprintf(stderr, "Parser: Memory allocation failed for JsonValue (FALSE)\n");
        return NULL;
    }
    value->type = JSON_BOOL;
    value->value.boolean = 0;
    return value;
}

/**
 * @brief Parses a JSON null token.
 *
 * @param[in,out] state Pointer to the ParserState instance.
 * @return Pointer to the parsed JsonValue, or NULL on failure.
 */
static JsonValue *parse_null(ParserState *state)
{
    (void)state;
    printf("Parser: Parsing null\n");
    JsonValue *value = json_alloc(sizeof(JsonValue));
    if (!value)
    {
        fprintf(stderr, "Parser: Memory allocation failed for JsonValue (NULL)\n");
        return NULL;
    }
    value->type = JSON_NULL;
    return value;
}

/**
 * @brief Parses a JSON object.
 *
 * This function handles parsing of JSON objects, which are collections of key-value pairs.
 *
 * @param[in,out] state Pointer to the ParserState instance.
 * @return Pointer to the parsed JsonObject, or NULL on failure.
 */
static JsonValue *parse_object(ParserState *state)
{
    printf("Parser: Starting to parse object.\n");
    JsonValue *object = json_alloc(sizeof(JsonValue));
    if (!object)
    {
        fprintf(stderr, "Parser: Memory allocation failed for JsonValue (OBJECT)\n");
        return NULL;
    }
    object->type = JSON_OBJECT;
    object->value.object = json_alloc(sizeof(JsonObject));
    if (!object->value.object)
    {
        fprintf(stderr, "Parser: Memory allocation failed for JsonObject\n");
        json_free(object);
        return NULL;
    }
    object->value.object->pairs = NULL;
    object->value.object->count = 0;

    /* Expecting the opening '{' has already been consumed before calling parse_object */

    while (1)
    {
        if (state->current_token.type == TOKEN_RIGHT_BRACE)
        {
            printf("Parser: Object parsing complete.\n");
            parser_advance(state);
            break;
        }

        // If not the first key-value pair, expect a comma
        if (object->value.object->count > 0)
        {
            parser_expect(state, TOKEN_COMMA); // Consume the comma
        }

        // Now, expect a key string
        if (state->current_token.type != TOKEN_STRING)
        {
            fprintf(stderr, "Parser: Expected TOKEN_STRING for key, but got %s\n",
                    json_token_type_to_string(state->current_token.type));
            json_free_value(object);
            exit(EXIT_FAILURE);
        }

        // Duplicate the key string to ensure safe memory management
        char *key = json_strdup(state->current_token.value);
        if (!key)
        {
            fprintf(stderr, "Parser: Memory allocation failed for key '%s'\n", state->current_token.value);
            json_free_value(object);
            exit(EXIT_FAILURE);
        }
        printf("Parser: Object key: '%s'\n", key);
        parser_advance(state); // Consume the string token

        // Expect colon
        if (state->current_token.type != TOKEN_COLON)
        {
            fprintf(stderr, "Parser: Expected TOKEN_COLON after key '%s', but got %s\n",
                    key, json_token_type_to_string(state->current_token.type));
            json_free(key);
            json_free_value(object);
            exit(EXIT_FAILURE);
        }
        parser_advance(state); // Consume the colon
        printf("Parser: Successfully processed colon. Parsing value for key: '%s'\n", key);

        // Parse value
        JsonValue *value = parse_value(state);
        if (!value)
        {
            fprintf(stderr, "Parser: Failed to parse value for key '%s'\n", key);
            json_free(key);
            json_free_value(object);
            exit(EXIT_FAILURE);
        }

        // Add key-value pair to object
        JsonPair *new_pairs = json_realloc(object->value.object->pairs, sizeof(JsonPair) * (object->value.object->count + 1));
        if (!new_pairs)
        {
            fprintf(stderr, "Parser: Memory allocation failed for JsonPair array.\n");
            json_free(key);
            json_free_value(value);
            json_free_value(object);
            exit(EXIT_FAILURE);
        }
        object->value.object->pairs = new_pairs;
        object->value.object->pairs[object->value.object->count].key = key;
        object->value.object->pairs[object->value.object->count].value = value;
        object->value.object->count++;

        printf("Parser: Added key-value pair: '%s': <value>\n", key);
    }

    return object;
}

/**
 * @brief Parses a JSON array.
 *
 * This function handles parsing of JSON arrays, which are ordered lists of JSON values.
 *
 * @param[in,out] state Pointer to the ParserState instance.
 * @return Pointer to the parsed JsonValue (ARRAY), or NULL on failure.
 */
static JsonValue *parse_array(ParserState *state)
{
    printf("Parser: Starting to parse array.\n");
    JsonValue *array = json_alloc(sizeof(JsonValue));
    if (!array)
    {
        fprintf(stderr, "Parser: Memory allocation failed for JsonValue (ARRAY)\n");
        return NULL;
    }
    array->type = JSON_ARRAY;
    array->value.array = json_alloc(sizeof(JsonArray));
    if (!array->value.array)
    {
        fprintf(stderr, "Parser: Memory allocation failed for JsonArray\n");
        json_free(array);
        return NULL;
    }
    array->value.array->items = NULL;
    array->value.array->count = 0;

    /* Expecting the opening '[' has already been consumed before calling parse_array */

    while (1)
    {
        if (state->current_token.type == TOKEN_RIGHT_BRACKET)
        {
            printf("Parser: Array parsing complete.\n");
            parser_advance(state); // Consume ']'
            break;
        }

        // If not the first element, expect a comma
        if (array->value.array->count > 0)
        {
            parser_expect(state, TOKEN_COMMA); // Ensures a comma is present and consumes it
        }

        // Parse value
        JsonValue *value = parse_value(state);
        if (!value)
        {
            fprintf(stderr, "Parser: Failed to parse value in array.\n");
            json_free_value(array);
            exit(EXIT_FAILURE);
        }

        // Add value to array
        JsonValue **new_items = json_realloc(array->value.array->items, sizeof(JsonValue *) * (array->value.array->count + 1));
        if (!new_items)
        {
            fprintf(stderr, "Parser: Memory allocation failed for JsonArray items.\n");
            json_free_value(value);
            json_free_value(array);
            exit(EXIT_FAILURE);
        }
        array->value.array->items = new_items;
        array->value.array->items[array->value.array->count] = value;
        array->value.array->count++;

        printf("Parser: Added value to array.\n");
    }

    return array;
}

/**
 * @brief Parses a JSON value based on the current token.
 *
 * This function determines the type of JSON value to parse (object, array, string, number, etc.)
 * and delegates to the appropriate parsing function.
 *
 * @param[in,out] state Pointer to the ParserState instance.
 * @return Pointer to the parsed JsonValue, or NULL on failure.
 */
static JsonValue *parse_value(ParserState *state)
{
    printf("Parser: Entering parse_value. Current token: %s\n", json_token_type_to_string(state->current_token.type));
    JsonValue *value = NULL;

    switch (state->current_token.type)
    {
    case TOKEN_LEFT_BRACE:
        printf("Parser: Detected object.\n");
        parser_advance(state); // Consume TOKEN_LEFT_BRACE
        value = parse_object(state);
        break;
    case TOKEN_LEFT_BRACKET:
        printf("Parser: Detected array.\n");
        parser_advance(state); // Consume TOKEN_LEFT_BRACKET
        value = parse_array(state);
        break;
    case TOKEN_STRING:
        printf("Parser: Detected string '%s'\n", state->current_token.value);
        value = parse_string(state, state->current_token.value);
        parser_advance(state); // Consume TOKEN_STRING
        break;
    case TOKEN_NUMBER:
        printf("Parser: Detected number '%s'\n", state->current_token.value);
        value = parse_number(state, state->current_token.value);
        parser_advance(state); // Consume TOKEN_NUMBER
        break;
    case TOKEN_TRUE:
        printf("Parser: Detected true.\n");
        value = parse_true(state);
        parser_advance(state); // Consume TOKEN_TRUE
        break;
    case TOKEN_FALSE:
        printf("Parser: Detected false.\n");
        value = parse_false(state);
        parser_advance(state); // Consume TOKEN_FALSE
        break;
    case TOKEN_NULL:
        printf("Parser: Detected null.\n");
        value = parse_null(state);
        parser_advance(state); // Consume TOKEN_NULL
        break;
    default:
        fprintf(stderr, "Parser Error: Unexpected token %s while parsing value.\n",
                json_token_type_to_string(state->current_token.type));
        exit(EXIT_FAILURE);
    }

    return value;
}

/**
 * @brief Parses a JSON string and constructs a JsonValue.
 *
 * @param[in] json The JSON string to parse.
 * @return Pointer to the root JsonValue if parsing is successful, NULL otherwise.
 */
JsonValue *json_parse(const char *json)
{
    printf("Parser: Starting JSON parsing...\n");
    ParserState state;
    json_tokenizer_init(&state.tokenizer, json);
    state.current_token = json_get_next_token(&state.tokenizer);

    JsonValue *root = parse_value(&state);
    if (root)
    {
        if (state.current_token.type != TOKEN_EOF)
        {
            fprintf(stderr, "Parser Error: Extra data detected after JSON root.\n");
            json_free_value(root);
            root = NULL;
        }
        else
        {
            printf("Parser: JSON parsing completed successfully.\n");
        }
    }
    else
    {
        fprintf(stderr, "Parser: Failed to parse JSON.\n");
    }

    json_token_free(&state.current_token);
    return root;
}

/**
 * @brief Frees the memory allocated for a JsonValue and its nested structures.
 *
 * This function recursively frees all memory associated with a JsonValue,
 * including any nested JsonObjects or JsonArrays.
 *
 * @param[in,out] value Pointer to the JsonValue to free.
 */
void json_free_value(JsonValue *value)
{
    if (!value)
        return;

    switch (value->type)
    {
    case JSON_STRING:
        json_free(value->value.string);
        break;
    case JSON_ARRAY:
        for (size_t i = 0; i < value->value.array->count; i++)
        {
            json_free_value(value->value.array->items[i]); // Correctly free each JsonValue* in the array
        }
        json_free(value->value.array->items);
        json_free(value->value.array);
        break;
    case JSON_OBJECT:
        for (size_t i = 0; i < value->value.object->count; i++)
        {
            json_free(value->value.object->pairs[i].key);
            json_free_value(value->value.object->pairs[i].value); // Correctly free each JsonValue* in the object
        }
        json_free(value->value.object->pairs);
        json_free(value->value.object);
        break;
    case JSON_BOOL:
    case JSON_NUMBER:
    case JSON_NULL:
        /* No additional memory to free */
        break;
    default:
        fprintf(stderr, "Parser Error: Unknown JsonType encountered during free.\n");
        break;
    }

    /* Free the JsonValue struct itself */
    json_free(value);
}
