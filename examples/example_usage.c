#include <stdio.h>
#include "json_parser.h"

void print_json_value(const JsonValue *value, int indent)
{
    for (int i = 0; i < indent; i++)
        printf("  ");
    switch (value->type)
    {
    case JSON_NULL:
        printf("null\n");
        break;
    case JSON_BOOL:
        printf(value->boolean ? "true\n" : "false\n");
        break;
    case JSON_NUMBER:
        printf("%lf\n", value->number);
        break;
    case JSON_STRING:
        printf("\"%s\"\n", value->string);
        break;
    case JSON_ARRAY:
        printf("[\n");
        for (size_t i = 0; i < value->array->size; i++)
        {
            print_json_value(&value->array->values[i], indent + 1);
        }
        for (int i = 0; i < indent; i++)
            printf("  ");
        printf("]\n");
        break;
    case JSON_OBJECT:
        printf("{\n");
        for (size_t i = 0; i < value->object->size; i++)
        {
            for (int j = 0; j < indent + 1; j++)
                printf("  ");
            printf("\"%s\": ", value->object->pairs[i].key);
            print_json_value(value->object->pairs[i].value, indent + 1);
        }
        for (int i = 0; i < indent; i++)
            printf("  ");
        printf("}\n");
        break;
    default:
        printf("Unknown type\n");
        break;
    }
}

int main()
{
    const char *json_string = "{ \"name\": \"John\", \"age\": 30, \"isStudent\": false }";

    JsonValue *parsed_json = json_parse(json_string);
    if (parsed_json)
    {
        printf("Parsed JSON successfully!\n");
        print_json_value(parsed_json, 0);
        json_free_value(parsed_json); // Clean up
    }
    else
    {
        printf("Failed to parse JSON.\n");
    }

    return 0;
}
