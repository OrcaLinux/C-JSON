#include "json_tokenizer.h"
#include <stdio.h>
#include <assert.h>

// Utility function to print tokens for debugging
void print_token(const JsonToken *token)
{
    printf("Token Type: %s", json_token_type_to_string(token->type));
    if (token->value)
    {
        printf(", Value: '%s'", token->value);
    }
    printf("\n");
}

// Test a single JSON string
void test_tokenizer_single(const char *json)
{
    printf("\nTesting JSON: %s\n", json);

    JsonTokenizer tokenizer;
    json_tokenizer_init(&tokenizer, json);

    while (1)
    {
        JsonToken token = json_get_next_token(&tokenizer);
        if (token.type == TOKEN_EOF)
        {
            printf("Tokenizer Test: Reached EOF\n");
            break;
        }
        if (token.type == TOKEN_ERROR)
        {
            printf("Tokenizer Test: Encountered an error\n");
            break;
        }

        print_token(&token);

        json_token_free(&token);
    }
}

// Test cases for edge cases
void test_empty_json()
{
    test_tokenizer_single("{}");
}

void test_empty_array()
{
    test_tokenizer_single("[]");
}

void test_nested_objects()
{
    test_tokenizer_single("{ \"key\": { \"nestedKey\": \"nestedValue\" } }");
}

void test_array_with_values()
{
    test_tokenizer_single("[1, 2, 3, \"four\", true, null]");
}

void test_complex_json()
{
    test_tokenizer_single("{ \"user\": { \"id\": 1, \"name\": \"John\" }, \"roles\": [\"admin\", \"user\"] }");
}

void test_invalid_json()
{
    test_tokenizer_single("{ key: value }");
}

void test_escaped_characters()
{
    test_tokenizer_single("{ \"escaped\": \"This \\\"is\\\" a test\" }");
}

void test_large_number()
{
    test_tokenizer_single("{ \"largeNumber\": 1234567890123456789 }");
}

void test_negative_number()
{
    test_tokenizer_single("{ \"negative\": -42 }");
}

void test_whitespace_handling()
{
    test_tokenizer_single("  {  \"key\"  :  \"value\"  }  ");
}

int main()
{
    // Test cases
    printf("Running Tokenizer Edge Case Tests...\n");

    test_empty_json();
    test_empty_array();
    test_nested_objects();
    test_array_with_values();
    test_complex_json();
    test_invalid_json();
    test_escaped_characters();
    test_large_number();
    test_negative_number();
    test_whitespace_handling();

    printf("\nAll tests completed.\n");

    return 0;
}
