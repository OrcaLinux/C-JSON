#ifndef JSON_TOKENIZER_H
#define JSON_TOKENIZER_H

#include "json_types.h"

/**
 * @file json_tokenizer.h
 * @brief Tokenizer (lexer) for JSON parsing.
 *
 * This header file declares the tokenizer functions and data structures
 * used to break down a JSON string into meaningful tokens.
 */

/**
 * @enum JsonTokenType
 * @brief Enumerates the different types of JSON tokens.
 *
 * These token types represent the various syntactical elements that
 * can be encountered while parsing a JSON string.
 */
typedef enum
{
    TOKEN_NONE,          /**< No token has been identified. */
    TOKEN_LEFT_BRACE,    /**< `{` Start of a JSON object. */
    TOKEN_RIGHT_BRACE,   /**< `}` End of a JSON object. */
    TOKEN_LEFT_BRACKET,  /**< `[` Start of a JSON array. */
    TOKEN_RIGHT_BRACKET, /**< `]` End of a JSON array. */
    TOKEN_COLON,         /**< `:` Separator between key and value in objects. */
    TOKEN_COMMA,         /**< `,` Separator between elements in arrays or key-value pairs in objects. */
    TOKEN_STRING,        /**< String value enclosed in double quotes. */
    TOKEN_NUMBER,        /**< Numeric value, can be integer or floating-point. */
    TOKEN_TRUE,          /**< Boolean value `true`. */
    TOKEN_FALSE,         /**< Boolean value `false`. */
    TOKEN_NULL,          /**< Null value `null`. */
    TOKEN_EOF,           /**< End of the JSON input. */
    TOKEN_ERROR          /**< Represents an error encountered during tokenization. */
} JsonTokenType;

/**
 * @struct JsonToken
 * @brief Represents a JSON token with its type and optional value.
 *
 * Each token identified by the tokenizer includes its type and, if applicable,
 * the string value associated with it (e.g., the actual string or number).
 */
typedef struct
{
    JsonTokenType type; /**< The type of the token, as defined in JsonTokenType. */
    char *value;        /**< The string value of the token, if applicable. For example, the actual string content or number in string form. */
} JsonToken;

/**
 * @struct JsonTokenizer
 * @brief Maintains the state of the tokenizer while parsing a JSON string.
 *
 * This structure keeps track of the JSON string being parsed and the current
 * position within that string.
 */
typedef struct
{
    const char *json; /**< Pointer to the JSON string being tokenized. */
    size_t pos;       /**< Current position (index) within the JSON string. */
} JsonTokenizer;

/**
 * @brief Converts a JsonTokenType to its string representation.
 *
 * This function maps each JsonTokenType enum value to a human-readable string
 * for debugging and logging purposes.
 *
 * @param[in] token_type The JsonTokenType to convert.
 * @return A pointer to the string representation of the token type.
 */
const char *json_token_type_to_string(JsonTokenType token_type);

/**
 * @brief Initializes the JSON tokenizer with a JSON string.
 *
 * This function sets up the tokenizer to begin parsing a new JSON string.
 *
 * @param[in,out] tokenizer Pointer to the JsonTokenizer instance to initialize.
 * @param[in]     json      The JSON string to tokenize.
 */
void json_tokenizer_init(JsonTokenizer *tokenizer, const char *json);

/**
 * @brief Retrieves the next token from the JSON string.
 *
 * This function analyzes the JSON string and returns the next token in sequence.
 * It updates the tokenizer's position accordingly.
 *
 * @param[in,out] tokenizer Pointer to the JsonTokenizer instance.
 * @return The next JsonToken identified in the JSON string.
 *
 * @note The caller is responsible for freeing the token's value using `json_token_free` if the token type includes a value (e.g., TOKEN_STRING, TOKEN_NUMBER).
 */
JsonToken json_get_next_token(JsonTokenizer *tokenizer);

/**
 * @brief Resets the JSON tokenizer to parse a new JSON string.
 *
 * This function reinitializes the tokenizer's state and sets a new JSON string for tokenization.
 *
 * @param[in,out] tokenizer Pointer to the JsonTokenizer instance to reset.
 * @param[in]     json      The new JSON string to tokenize.
 */
void json_tokenizer_reset(JsonTokenizer *tokenizer, const char *json);

/**
 * @brief Frees any dynamically allocated memory within a JsonToken.
 *
 * This function cleans up the memory associated with a JsonToken's value, if applicable.
 *
 * @param[in,out] token Pointer to the JsonToken to free.
 *
 * @note Only the `value` field is freed. The `token` itself should not be freed unless it was dynamically allocated elsewhere.
 */
void json_token_free(JsonToken *token);

#endif // JSON_TOKENIZER_H
