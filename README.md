# **C-JSON**

_A lightweight and efficient JSON parsing library for C applications._

---

## **Overview**

C-JSON is a minimal yet robust JSON parsing library designed for developers who need fast and efficient JSON handling in C. Whether you're building embedded systems, working on performance-critical applications, or integrating JSON into your C projects, C-JSON provides the tools you need with minimal overhead.

With features like handling deeply nested structures, simple API integration, and comprehensive error handling, C-JSON ensures a smooth experience while processing JSON data efficiently.

---

## **Features**

- **Lightweight and Fast**: Built for performance and low memory usage.
- **Comprehensive Parsing**:
  - JSON Objects
  - Arrays
  - Strings
  - Numbers (integers and floating-point)
  - Booleans
  - Null values
- **Supports Nested Structures**: Handles deeply nested objects and arrays effortlessly.
- **Intuitive API**: Easy-to-use functions for seamless integration.
- **Error Handling**: Identifies and reports malformed JSON with helpful messages.
- **Cross-Platform**: Compatible with Linux, macOS, and Windows.

---

## **Quick Start**

### **Installation**

1. **Clone the Repository**  
   Get the source code from GitHub:

   ```bash
   git clone https://github.com/OrcaLinux/c-json.git
   cd c-json
   ```

2. **Build the Library**  
   Compile the library using the provided Makefile:

   ```bash
   make
   ```

3. **Run Tests**  
   Verify that the library is functioning as expected:

   ```bash
   make test
   ```

---

### **Integration**

1. Include the header file in your source code:

   ```c
   #include "json_parser.h"
   ```

2. Link the library during compilation:

   ```bash
   gcc -o my_program my_program.c -Lpath/to/json-parser/build -ljsonparser
   ```

---

## **Usage**

### **Example: Parsing a Simple JSON Object**

```c
#include "json_parser.h"
#include <stdio.h>

int main() {
    const char* json_str = "{ \"name\": \"C-JSON\", \"version\": 1.0, \"active\": true }";

    JsonValue* root = json_parse(json_str);
    if (!root) {
        printf("Failed to parse JSON.\n");
        return 1;
    }

    // Access object values
    JsonValue* name = json_object_get(root, "name");
    JsonValue* version = json_object_get(root, "version");
    JsonValue* active = json_object_get(root, "active");

    if (name && name->type == JSON_STRING)
        printf("Name: %s\n", name->value.string);
    if (version && version->type == JSON_NUMBER)
        printf("Version: %.1f\n", version->value.number);
    if (active && active->type == JSON_BOOL)
        printf("Active: %s\n", active->value.boolean ? "true" : "false");

    json_free_value(root);
    return 0;
}
```

---

### **Example: Handling Nested Structures**

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
            printf("Name: %s\n", name->value.string);
        if (age && age->type == JSON_NUMBER)
            printf("Age: %.0f\n", age->value.number);
    }

    // Access arrays
    JsonValue* hobbies = json_object_get(root, "hobbies");
    if (hobbies && hobbies->type == JSON_ARRAY) {
        printf("Hobbies:\n");
        for (size_t i = 0; i < hobbies->value.array->count; i++) {
            JsonValue* hobby = hobbies->value.array->items[i];
            if (hobby && hobby->type == JSON_STRING)
                printf("- %s\n", hobby->value.string);
        }
    }

    json_free_value(root);
    return 0;
}
```

---

## **Directory Structure**

```
.
├── examples/
│   └── example_usage.c      # Example application showcasing library usage
├── include/
│   ├── json_accessor.h      # JSON accessor API header
│   ├── json_parser.h        # Main parser API header
│   ├── json_printer.h       # JSON pretty-printing API header
│   ├── json_tokenizer.h     # Tokenizer API header
│   ├── json_types.h         # JSON type definitions
│   └── json_utils.h         # Utility functions header
├── LICENSE                  # Project license
├── Makefile                 # Build instructions
├── README.md                # Project documentation
├── src/
│   ├── json_accessor.c      # Implementation of accessor functions
│   ├── json_parser.c        # Implementation of the JSON parser
│   ├── json_printer.c       # Implementation of the JSON printer
│   ├── json_tokenizer.c     # Implementation of the tokenizer
│   └── json_utils.c         # Implementation of utility functions
└── tests/
    ├── test_parser.c        # Unit tests for the JSON parser
    └── test_tokenizer.c     # Unit tests for the tokenizer
```

---

## **Contributing**

Contributions are welcome! To contribute:

1. Fork the repository.
2. Create a feature branch:
   ```bash
   git checkout -b feature/AmazingFeature
   ```
3. Commit your changes:
   ```bash
   git commit -m "Add AmazingFeature"
   ```
4. Push your branch:
   ```bash
   git push origin feature/AmazingFeature
   ```
5. Open a pull request.

---

## **Roadmap**

- [ ] Add JSON serialization support.
- [ ] Optimize parsing for large JSON files.
- [ ] Add detailed error reporting.
- [ ] Include performance benchmarks.

---

## **License**

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

## **Acknowledgments**

- Inspired by the simplicity and versatility of JSON.
- Thanks to all contributors and open-source advocates.
