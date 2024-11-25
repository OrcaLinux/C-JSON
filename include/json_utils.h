#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <stddef.h>
#include <stdlib.h>

/**
 * @file json_utils.h
 * @brief Utility functions for JSON parser.
 *
 * This header file declares utility functions used by the JSON parser,
 * including memory management and string handling functions.
 */

/**
 * @brief Allocates memory of the specified size.
 *
 * This function wraps the standard `malloc` function, providing a centralized
 * location for memory allocation, which can be modified or optimized as needed.
 *
 * @param[in] size The size in bytes to allocate.
 * @return Pointer to the allocated memory on success, or NULL if allocation fails.
 *
 * @note It is the caller's responsibility to free the allocated memory using `json_free`.
 */
void* json_alloc(size_t size);

/**
 * @brief Reallocates memory to a new size.
 *
 * This function wraps the standard `realloc` function, allowing dynamic resizing
 * of previously allocated memory blocks.
 *
 * @param[in,out] ptr  Pointer to the memory block previously allocated with `json_alloc` or `json_realloc`.
 * @param[in]     size The new size in bytes for the memory block.
 * @return Pointer to the reallocated memory on success, or NULL if reallocation fails.
 *
 * @note If reallocation fails, the original memory block remains unchanged.
 *       It is the caller's responsibility to handle the NULL return value appropriately.
 */
void* json_realloc(void *ptr, size_t size);

/**
 * @brief Frees the allocated memory.
 *
 * This function wraps the standard `free` function, providing a centralized
 * location for memory deallocation, which can be modified or optimized as needed.
 *
 * @param[in,out] ptr Pointer to the memory block to free.
 *
 * @note After calling this function, the pointer should not be used unless it is reassigned.
 */
void json_free(void *ptr);

/**
 * @brief Duplicates a null-terminated string.
 *
 * This function creates a duplicate of the given string by allocating sufficient
 * memory and copying the string content. It is equivalent to the standard `strdup`
 * function but utilizes `json_alloc` for memory allocation.
 *
 * @param[in] s The null-terminated string to duplicate.
 * @return Pointer to the duplicated string on success, or NULL if allocation fails.
 *
 * @note The caller is responsible for freeing the duplicated string using `json_free`.
 */
char* json_strdup(const char *s);

/**
 * @brief Duplicates a range of characters from a string.
 *
 * This function creates a duplicate of a specified range within a string by allocating
 * sufficient memory and copying the characters. It ensures that the duplicated string
 * is null-terminated.
 *
 * @param[in] s   The source string from which to duplicate characters.
 * @param[in] len The number of characters to duplicate.
 * @return Pointer to the duplicated string on success, or NULL if allocation fails.
 *
 * @note The duplicated string will contain `len` characters followed by a null terminator.
 *       The caller is responsible for freeing the duplicated string using `json_free`.
 */
char* json_strdup_range(const char *s, size_t len);

/**
 * @brief Checks if a character is considered whitespace in JSON.
 *
 * This function determines whether a given character is classified as whitespace
 * according to JSON specifications. Whitespace characters are typically ignored
 * by the JSON parser outside of string literals.
 *
 * @param[in] c The character to check.
 * @return Non-zero if the character is whitespace (` `, `\t`, `\n`, `\r`), zero otherwise.
 */
int json_is_whitespace(char c);

#endif // JSON_UTILS_H
