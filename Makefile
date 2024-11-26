# Compiler and flags
CC = gcc

# Default build mode (Release)
BUILD_MODE ?= release

# Flags for Release and Debug
CFLAGS_DEBUG = -Wall -Wextra -Iinclude -g -fPIC -std=c99
CFLAGS_RELEASE = -Wall -Wextra -Iinclude -O3 -fPIC -std=c99

# Use the appropriate flags based on BUILD_MODE
ifeq ($(BUILD_MODE), debug)
    CFLAGS = $(CFLAGS_DEBUG)
else
    CFLAGS = $(CFLAGS_RELEASE)
endif

# Default library type (static)
LIB_TYPE ?= static

# Directories
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = tests
EXAMPLES_DIR = examples
BUILD_DIR = build
BIN_DIR = bin

# Source files
# Updated to include all .c files in src/
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
TEST_FILES = $(TEST_DIR)/test_parser.c
EXAMPLE_FILES = $(EXAMPLES_DIR)/example_usage.c

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_FILES))
EXAMPLE_OBJ_FILES = $(patsubst $(EXAMPLES_DIR)/%.c, $(BUILD_DIR)/%.o, $(EXAMPLE_FILES))

# Targets for static and shared libraries
STATIC_LIB = $(BUILD_DIR)/libjsonparser.a
SHARED_LIB = $(BUILD_DIR)/libjsonparser.so
TEST_TARGET = $(BUILD_DIR)/test_parser
EXAMPLE_TARGET = $(BUILD_DIR)/example_usage

# New Variables for Dynamic Test Selection
# ----------------------------------------------------
# TEST_FILE: Name of the test file without the .c extension
# Default is 'test_parser', but can be overridden via command line
TEST_FILE ?= test_parser

# TEST_SOURCE: Path to the test source file
TEST_SOURCE = $(TEST_DIR)/$(TEST_FILE).c

# TEST_OBJ: Path to the compiled test object file
TEST_OBJ = $(BUILD_DIR)/$(TEST_FILE).o

# TEST_EXEC: Path to the compiled test executable
TEST_EXEC = $(BUILD_DIR)/$(TEST_FILE)
# ----------------------------------------------------

# Default target: build static or shared library based on LIB_TYPE
all: $(LIB_TYPE)

# Build static library
static: $(STATIC_LIB)

$(STATIC_LIB): $(OBJ_FILES)
	mkdir -p $(BUILD_DIR)
	ar rcs $@ $^

# Build shared library
shared: $(SHARED_LIB)

$(SHARED_LIB): $(OBJ_FILES)
	mkdir -p $(BUILD_DIR)
	$(CC) -shared -o $@ $^

# Build object files for source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build and run tests dynamically based on TEST_FILE
test: $(STATIC_LIB) $(TEST_OBJ)
	$(CC) $(CFLAGS) $(TEST_OBJ) $(STATIC_LIB) -o $(TEST_EXEC)
	./$(TEST_EXEC)

# Build object files for test files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build and run example
example: $(STATIC_LIB) $(EXAMPLE_OBJ_FILES)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(EXAMPLE_OBJ_FILES) $(STATIC_LIB) -o $(EXAMPLE_TARGET)
	./$(EXAMPLE_TARGET)

$(BUILD_DIR)/%.o: $(EXAMPLES_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Help target to display usage information
help:
	@echo "Available Targets:"
	@echo "  make all                   Build static or shared library based on LIB_TYPE"
	@echo "  make static                Build static library"
	@echo "  make shared                Build shared library"
	@echo "  make test                  Run the default test (test_parser)"
	@echo "  make test TEST_FILE=<name> Run a specific test (e.g., test_tokenizer)"
	@echo "  make example               Build and run the example usage"
	@echo "  make clean                 Clean all build artifacts"

# Phony targets
.PHONY: all static shared test example clean help
