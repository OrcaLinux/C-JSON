#include "json_parser.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_parse_empty_object()
{
    const char *json = "{ }";
    JsonValue *value = json_parse(json);
    assert(value != NULL);
    assert(value->type == JSON_OBJECT);
    assert(value->object->size == 0);
    json_free_value(value);
}

void test_parse_empty_array()
{
    const char *json = "[]";
    JsonValue *value = json_parse(json);
    assert(value != NULL);
    assert(value->type == JSON_ARRAY);
    assert(value->array->size == 0);
    json_free_value(value);
}

void test_parse_nested_structures()
{
    const char *json = "{ \"user\": { \"id\": 1, \"name\": \"John\" }, \"roles\": [\"admin\", \"user\"] }";
    JsonValue *value = json_parse(json);
    assert(value != NULL);
    assert(value->type == JSON_OBJECT);
    assert(value->object->size == 2);
    // Further assertions...
    json_free_value(value);
}

void test_parse_simple_object()
{
    const char *json = "{ \"key\": \"value\" }";
    JsonValue *value = json_parse(json);
    assert(value != NULL);
    assert(value->type == JSON_OBJECT);
    assert(value->object->size == 1);
    assert(strcmp(value->object->pairs[0].key, "key") == 0);
    assert(value->object->pairs[0].value->type == JSON_STRING);
    assert(strcmp(value->object->pairs[0].value->string, "value") == 0);
    json_free_value(value);
}

void test_parse_array()
{
    const char *json = "[1, 2, 3]";
    JsonValue *value = json_parse(json);
    assert(value != NULL);
    assert(value->type == JSON_ARRAY);
    assert(value->array->size == 3);
    assert(value->array->values[0]->type == JSON_NUMBER && value->array->values[0]->number == 1);
    assert(value->array->values[1]->type == JSON_NUMBER && value->array->values[1]->number == 2);
    assert(value->array->values[2]->type == JSON_NUMBER && value->array->values[2]->number == 3);
    json_free_value(value);
}

void test_parse_nested()
{
    const char *json = "{ \"person\": { \"name\": \"Alice\", \"age\": 25 }, \"hobbies\": [\"reading\", \"swimming\"] }";
    JsonValue *value = json_parse(json);
    assert(value != NULL);
    assert(value->type == JSON_OBJECT);
    assert(value->object->size == 2);

    // Check "person"
    JsonPair *person_pair = &value->object->pairs[0];
    assert(strcmp(person_pair->key, "person") == 0);
    assert(person_pair->value->type == JSON_OBJECT);
    assert(person_pair->value->object->size == 2);

    // Check "name" within "person"
    JsonPair *name_pair = &person_pair->value->object->pairs[0];
    assert(strcmp(name_pair->key, "name") == 0);
    assert(name_pair->value->type == JSON_STRING);
    assert(strcmp(name_pair->value->string, "Alice") == 0);

    // Check "age" within "person"
    JsonPair *age_pair = &person_pair->value->object->pairs[1];
    assert(strcmp(age_pair->key, "age") == 0);
    assert(age_pair->value->type == JSON_NUMBER);
    assert(age_pair->value->number == 25);

    // Check "hobbies"
    JsonPair *hobbies_pair = &value->object->pairs[1];
    assert(strcmp(hobbies_pair->key, "hobbies") == 0);
    assert(hobbies_pair->value->type == JSON_ARRAY);
    assert(hobbies_pair->value->array->size == 2);

    // Accessing array elements as pointers
    assert(hobbies_pair->value->array->values[0]->type == JSON_STRING);
    assert(strcmp(hobbies_pair->value->array->values[0]->string, "reading") == 0);

    assert(hobbies_pair->value->array->values[1]->type == JSON_STRING);
    assert(strcmp(hobbies_pair->value->array->values[1]->string, "swimming") == 0);

    json_free_value(value);
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
