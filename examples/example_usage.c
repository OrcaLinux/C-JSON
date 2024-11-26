#include "json_parser.h"
#include "json_printer.h"
#include "json_accessor.h"
#include "json_types.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    const char *json_input = "{\n"
                             "    \"name\": \"John Doe\",\n"
                             "    \"age\": 30,\n"
                             "    \"isStudent\": false,\n"
                             "    \"scores\": [85.5, 90.0, 78.5],\n"
                             "    \"address\": {\n"
                             "        \"street\": \"123 Main St\",\n"
                             "        \"city\": \"Anytown\"\n"
                             "    },\n"
                             "    \"nickname\": null\n"
                             "}";

    /* Parse the JSON input */
    JsonValue *root = json_parse(json_input);

    if (root)
    {
        printf("Parsed JSON:\n");
        json_print(root, 0);
        printf("\n");

        /* Accessing specific data using accessor functions */
        const char *name = json_get_string(root, "name");
        double age = json_get_number(root, "age");
        bool is_student = json_get_bool(root, "isStudent");
        JsonValue *scores = json_get_array(root, "scores");
        JsonValue *address = json_get_object(root, "address");
        bool has_nickname = json_is_null(root, "nickname");

        if (name)
            printf("Name: %s\n", name);
        printf("Age: %.0lf\n", age);
        printf("Is Student: %s\n", is_student ? "true" : "false");

        if (scores && scores->type == JSON_ARRAY)
        {
            printf("Scores: ");
            for (size_t i = 0; i < scores->value.array->count; i++)
            {
                printf("%.1lf ", scores->value.array->items[i]->value.number);
            }
            printf("\n");
        }

        if (address && address->type == JSON_OBJECT)
        {
            const char *city = json_get_string(address, "city");
            if (city)
                printf("City: %s\n", city);
        }

        printf("Nickname: %s\n", has_nickname ? "null" : "Exists");

        /* Free the parse tree */
        json_free_value(root);
    }
    else
    {
        printf("Failed to parse JSON.\n");
    }

    return 0;
}
