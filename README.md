# **LightJSON**

_A lightweight and efficient JSON parsing library for C applications._

---

### **Overview**

LightJSON is a minimalist yet powerful JSON parsing library designed for C developers who value performance and simplicity. Whether you are working on embedded systems, performance-critical applications, or lightweight utilities, LightJSON is your go-to solution for handling JSON data with minimal overhead.

Key features include robust parsing of JSON objects, arrays, strings, numbers, booleans, and null values. With support for nested structures and a small memory footprint, LightJSON ensures your applications stay efficient while offering full JSON compatibility.

---

## **Features**

- **Lightweight & Fast**: Designed with performance and resource efficiency in mind.
- **Robust Parsing**: Supports:
  - JSON Objects
  - Arrays
  - Strings
  - Numbers (including integers and floats)
  - Boolean values
  - Null values
- **Handles Nesting**: Parse and access deeply nested JSON structures with ease.
- **Simple API**: Intuitive and easy-to-use interface for seamless integration.
- **Error Handling**: Provides clear error feedback for malformed JSON strings.
- **Cross-Platform**: Compatible with any C compiler on Linux, macOS, and Windows.

---

## **Quick Start**

### **Installation**

1. **Clone the Repository**  
   Download the source code from GitHub:

   ```bash
   git clone https://github.com/OrcaLinux/json-parser.git
   cd lightjson
   ```

2. **Build the Library**  
   Compile the library using `make`:

   ```bash
   make
   ```

3. **Run Tests**  
   Ensure everything is working as expected:

   ```bash
   make test
   ```

### **Adding LightJSON to Your Project**

1. Include the header file in your source code:

   ```c
   #include "json_parser.h"
   ```

2. Link the compiled library (`libjsonparser.a`) to your project during compilation:

   ```bash
   gcc -o my_program my_program.c -Lpath/to/lightjson/build -ljsonparser
   ```

---

## **Usage**

LightJSON simplifies JSON parsing into just a few function calls. Here's a practical example:

### **Example: Parsing a Simple JSON Object**

```c
#include "json_parser.h"
#include <stdio.h>

int main() {
    const char* json_str = "{ \"name\": \"LightJSON\", \"version\": 1.0, \"active\": true }";

    JsonValue* root = json_parse(json_str);
    if (root == NULL) {
        printf("Failed to parse JSON.\n");
        return 1;
    }

    // Access object values
    JsonValue* name = json_object_get(root, "name");
    JsonValue* version = json_object_get(root, "version");
    JsonValue* active = json_object_get(root, "active");

    if (name != NULL && name->type == JSON_STRING)
        printf("Name: %s\n", name->string);
    if (version != NULL && version->type == JSON_NUMBER)
        printf("Version: %.1f\n", version->number);
    if (active != NULL && active->type == JSON_BOOL)
        printf("Active: %s\n", active->boolean ? "true" : "false");

    json_free_value(root);
    return 0;
}
```

---

## **Advanced Usage**

### **Example: Parsing Nested Structures**

LightJSON handles complex and deeply nested JSON effortlessly:

```c
#include "json_parser.h"
#include <stdio.h>

int main() {
    const char* json_str = "{ \"user\": { \"name\": \"Alice\", \"age\": 25 }, \"hobbies\": [\"reading\", \"swimming\"] }";

    JsonValue* root = json_parse(json_str);
    if (!root) {
        printf("Failed to parse JSON.\n");
        return 1;
    }

    // Access nested objects
    JsonValue* user = json_object_get(root, "user");
    if (user && user->type == JSON_OBJECT) {
        JsonValue* name = json_object_get(user, "name");
        JsonValue* age = json_object_get(user, "age");

        if (name && name->type == JSON_STRING)
            printf("Name: %s\n", name->string);
        if (age && age->type == JSON_NUMBER)
            printf("Age: %.0f\n", age->number);
    }

    // Access arrays
    JsonValue* hobbies = json_object_get(root, "hobbies");
    if (hobbies && hobbies->type == JSON_ARRAY) {
        printf("Hobbies:\n");
        for (size_t i = 0; i < hobbies->array->size; i++) {
            JsonValue* hobby = hobbies->array->values[i];
            if (hobby && hobby->type == JSON_STRING)
                printf("- %s\n", hobby->string);
        }
    }

    json_free_value(root);
    return 0;
}
```

---

## **Directory Structure**

```
lightjson/
├── include/
│   └── json_parser.h      # Public header file
├── src/
│   ├── json_parser.c      # Main parser implementation
│   ├── json_tokenizer.c   # Tokenizer for JSON strings
│   ├── json_utils.c       # Utility functions
├── tests/
│   ├── test_parser.c      # Unit tests for parser
│   ├── test_tokenizer.c   # Unit tests for tokenizer
├── build/                 # Compiled library and object files (generated by make)
├── Makefile               # Build system configuration
└── README.md              # Project documentation
```

---

## **Contributing**

Contributions are welcome and encouraged! Here's how you can contribute:

1. Fork the repository.
2. Create a branch for your feature or bug fix:
   ```bash
   git checkout -b feature/AmazingFeature
   ```
3. Commit your changes with a descriptive message:
   ```bash
   git commit -m "Add AmazingFeature to improve performance"
   ```
4. Push your branch to GitHub:
   ```bash
   git push origin feature/AmazingFeature
   ```
5. Open a pull request and describe your changes.

---

## **Roadmap**

- [ ] Add support for JSON serialization.
- [ ] Optimize memory usage for large JSON files.
- [ ] Provide benchmarks comparing parsing speed with other libraries.
- [ ] Expand error handling with detailed error messages.

---

## **License**

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

## **Acknowledgments**

- Inspired by the simplicity of JSON and the need for lightweight tools in C.
- Special thanks to contributors and the open-source community.
