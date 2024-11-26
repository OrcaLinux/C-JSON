#include "json_printer.h"
#include <stdio.h>

/**
 * @brief Prints indentation spaces.
 *
 * @param[in] indent Number of spaces to indent.
 */
static void print_indent(int indent)
{
    for (int i = 0; i < indent; i++)
    {
        printf(" ");
    }
}

/**
 * @brief Recursively prints the JSON parse tree.
 *
 * @param[in] value  Pointer to the current JsonValue.
 * @param[in] indent Current indentation level.
 */
void json_print(const JsonValue *value, int indent)
{
    if (!value)
    {
        print_indent(indent);
        printf("null\n");
        return;
    }

    switch (value->type)
    {
    case JSON_NULL:
        print_indent(indent);
        printf("null\n");
        break;

    case JSON_BOOL:
        print_indent(indent);
        printf(value->value.boolean ? "true\n" : "false\n");
        break;

    case JSON_NUMBER:
        print_indent(indent);
        printf("%lf\n", value->value.number);
        break;

    case JSON_STRING:
        print_indent(indent);
        printf("\"%s\"\n", value->value.string);
        break;

    case JSON_ARRAY:
        print_indent(indent);
        printf("[\n");
        for (size_t i = 0; i < value->value.array->count; i++)
        {
            json_print(value->value.array->items[i], indent + 2);
            if (i < value->value.array->count - 1)
            {
                // Replace the last newline with a comma
                fseek(stdout, -1, SEEK_CUR);
                printf(",\n");
            }
        }
        print_indent(indent);
        printf("]\n");
        break;

    case JSON_OBJECT:
        print_indent(indent);
        printf("{\n");
        for (size_t i = 0; i < value->value.object->count; i++)
        {
            print_indent(indent + 2);
            printf("\"%s\": ", value->value.object->pairs[i].key);
            // Determine if the value is a primitive type to format accordingly
            if (value->value.object->pairs[i].value->type == JSON_OBJECT ||
                value->value.object->pairs[i].value->type == JSON_ARRAY)
            {
                printf("\n");
                json_print(value->value.object->pairs[i].value, indent + 4);
            }
            else
            {
                // For primitive types, print inline
                switch (value->value.object->pairs[i].value->type)
                {
                case JSON_NULL:
                    printf("null\n");
                    break;
                case JSON_BOOL:
                    printf(value->value.object->pairs[i].value->value.boolean ? "true\n" : "false\n");
                    break;
                case JSON_NUMBER:
                    printf("%lf\n", value->value.object->pairs[i].value->value.number);
                    break;
                case JSON_STRING:
                    printf("\"%s\"\n", value->value.object->pairs[i].value->value.string);
                    break;
                default:
                    printf("null\n"); // Fallback for unexpected types
                    break;
                }
            }
            if (i < value->value.object->count - 1)
            {
                // Replace the last newline with a comma
                fseek(stdout, -1, SEEK_CUR);
                printf(",\n");
            }
        }
        print_indent(indent);
        printf("}\n");
        break;

    default:
        print_indent(indent);
        printf("null\n"); // Fallback for unknown types
        break;
    }
}
