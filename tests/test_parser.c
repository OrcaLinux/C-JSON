// tests/test_parser.c

#include "json_parser.h"
#include "json_accessor.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Tests parsing of an empty JSON object.
 */
void test_parse_empty_object()
{
    const char *json = "{ }";
    JsonValue *value = json_parse(json);
    assert(value != NULL);
    assert(value->type == JSON_OBJECT);
    assert(value->value.object->count == 0);
    json_free_value(value);
    printf("test_parse_empty_object passed.\n");
}

/**
 * @brief Tests parsing of an empty JSON array.
 */
void test_parse_empty_array()
{
    const char *json = "[]";
    JsonValue *value = json_parse(json);
    assert(value != NULL);
    assert(value->type == JSON_ARRAY);
    assert(value->value.array->count == 0);
    json_free_value(value);
    printf("test_parse_empty_array passed.\n");
}

/**
 * @brief Tests parsing of nested JSON structures.
 */
void test_parse_nested_structures()
{
    const char *json = "{ \"user\": { \"id\": 1, \"name\": \"John\" }, \"roles\": [\"admin\", \"user\"] }";
    JsonValue *value = json_parse(json);
    assert(value != NULL);
    assert(value->type == JSON_OBJECT);
    assert(value->value.object->count == 2);

    // Check "user" object
    JsonValue *user = json_get_object(value, "user");
    assert(user != NULL);
    assert(user->type == JSON_OBJECT);
    assert(user->value.object->count == 2);

    // Check "id"
    const char *id_key = "id";
    double id = json_get_number(user, id_key);
    assert(id == 1);

    // Check "name"
    const char *name_key = "name";
    const char *name = json_get_string(user, name_key);
    assert(name != NULL);
    assert(strcmp(name, "John") == 0);

    // Check "roles" array
    JsonValue *roles = json_get_array(value, "roles");
    assert(roles != NULL);
    assert(roles->type == JSON_ARRAY);
    assert(roles->value.array->count == 2);

    // Accessing array elements as pointers
    const char *role1 = json_get_string(roles->value.array->items[0], NULL);
    const char *role2 = json_get_string(roles->value.array->items[1], NULL);
    assert(role1 != NULL && strcmp(role1, "admin") == 0);
    assert(role2 != NULL && strcmp(role2, "user") == 0);

    json_free_value(value);
    printf("test_parse_nested_structures passed.\n");
}

/**
 * @brief Tests parsing of a simple JSON object.
 */
void test_parse_simple_object()
{
    const char *json = "{ \"key\": \"value\" }";
    JsonValue *value = json_parse(json);
    assert(value != NULL);
    assert(value->type == JSON_OBJECT);
    assert(value->value.object->count == 1);
    assert(strcmp(value->value.object->pairs[0].key, "key") == 0);
    assert(value->value.object->pairs[0].value->type == JSON_STRING);
    assert(strcmp(value->value.object->pairs[0].value->value.string, "value") == 0);
    json_free_value(value);
    printf("test_parse_simple_object passed.\n");
}

/**
 * @brief Tests parsing of a JSON array with numbers.
 */
void test_parse_array()
{
    const char *json = "[1, 2, 3]";
    JsonValue *value = json_parse(json);
    assert(value != NULL);
    assert(value->type == JSON_ARRAY);
    assert(value->value.array->count == 3);
    assert(value->value.array->items[0]->type == JSON_NUMBER && value->value.array->items[0]->value.number == 1);
    assert(value->value.array->items[1]->type == JSON_NUMBER && value->value.array->items[1]->value.number == 2);
    assert(value->value.array->items[2]->type == JSON_NUMBER && value->value.array->items[2]->value.number == 3);
    json_free_value(value);
    printf("test_parse_array passed.\n");
}

/**
 * @brief Tests parsing of nested JSON objects and arrays.
 */
void test_parse_nested()
{
    const char *json = "{ \"person\": { \"name\": \"Alice\", \"age\": 25 }, \"hobbies\": [\"reading\", \"swimming\"] }";
    JsonValue *value = json_parse(json);
    assert(value != NULL);
    assert(value->type == JSON_OBJECT);
    assert(value->value.object->count == 2);

    // Check "person" object
    JsonValue *person = json_get_object(value, "person");
    assert(person != NULL);
    assert(person->type == JSON_OBJECT);
    assert(person->value.object->count == 2);

    // Check "name" within "person"
    const char *name_key = "name";
    const char *name = json_get_string(person, name_key);
    assert(name != NULL);
    assert(strcmp(name, "Alice") == 0);

    // Check "age" within "person"
    const char *age_key = "age";
    double age = json_get_number(person, age_key);
    assert(age == 25);

    // Check "hobbies" array
    JsonValue *hobbies = json_get_array(value, "hobbies");
    assert(hobbies != NULL);
    assert(hobbies->type == JSON_ARRAY);
    assert(hobbies->value.array->count == 2);

    // Accessing array elements as pointers
    const char *hobby1 = json_get_string(hobbies->value.array->items[0], NULL);
    const char *hobby2 = json_get_string(hobbies->value.array->items[1], NULL);
    assert(hobby1 != NULL && strcmp(hobby1, "reading") == 0);
    assert(hobby2 != NULL && strcmp(hobby2, "swimming") == 0);

    json_free_value(value);
    printf("test_parse_nested passed.\n");
}

int main()
{
    test_parse_empty_object();
    test_parse_empty_array();
    test_parse_nested_structures();
    test_parse_simple_object();
    test_parse_array();
    test_parse_nested();
    printf("All tests passed!\n");
    return 0;
}
