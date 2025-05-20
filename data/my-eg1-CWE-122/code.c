#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/**
 * @brief Retrieves a JavaScript string value and writes it into a buffer as an ASCII-encoded string.
 * 
 * This function extracts a JavaScript string from a javascript value and writes it into a provided buffer
 * in ASCII encoding. If the buffer is NULL, the function returns the length of the string in bytes
 * (excluding the null terminator) through the `result` parameter.
 * 
 * @param js_env [in] The environment that the API is invoked under.
 * @param js_value [in] js_value representing the JavaScript string.
 * @param buf [in] Buffer to write the ASCII-encoded string into. If NULL is passed in, the length 
 *                 of the string (excluding the null terminator) is returned. Note that the string
 *                 written to the buffer is always null-terminated, even if the buffer size is insufficient.
 * @param buf_size [in] Size of the destination buffer. When this value is insufficient, the returned 
 *                      string is truncated and null-terminated.
 * @param result [out] Number of bytes copied into the buffer, excluding the null terminator.
 */
extern void js_get_value_string_ascii(void *js_env, void *js_value, char *buf, size_t buf_size, size_t *result);

/**
 * @struct string
 * @brief Represents a dynamically allocated string with a specified size.
 * 
 * This structure is used to store a string along with its size. The string is not null-terminated,
 * and its length is explicitly stored in the `size` field. The `str` field is a flexible array member
 * that holds the string data.
 * 
 * @var string::size
 * The size of the string in bytes. This does not include a null terminator.
 * 
 * @var string::str
 * A flexible array member that contains the string data. The string is stored as raw bytes and is not null-terminated.
 */
typedef struct string {
    int size;
    char str[];
} string;

/**
 * @brief Allocates memory for a string structure with a specified size.
 * 
 * This function allocates memory for a `string` structure, including space for the string data.
 * The size of the string is specified in bytes. The function checks for valid size and returns
 * NULL if the size is invalid or if memory allocation fails.
 * 
 * @param size The size of the string to allocate, in bytes.
 * @return string* A pointer to the allocated `string` structure. Returns NULL if allocation fails.
 */
string *alloc(int size) {
    if (size < 0 || size > INT_MAX - sizeof(string)) {
        return NULL;
    }
    string *s = malloc(sizeof(string) + size);
    if (s == NULL) {
        return NULL;
    }
    s->size = size;
    return s;
}

void free_string(string *s) {
    if (s != NULL) {
        free(s);
    }
}

/**
 * @brief Retrieves a JavaScript string from a js_value and stores it in a dynamically allocated string structure.
 * 
 * This function extracts a JavaScript string from a js_value, allocates memory for it using the `alloc` function,
 * and writes the string into the allocated buffer. The resulting string is stored in a `string` structure, which
 * includes the size of the string and the null-terminated ASCII-encoded string itself.
 * 
 * @param js_env [in] The environment that the API is invoked under.
 * @param js_value [in] js_value representing the JavaScript string.
 * 
 * @return string* A pointer to the dynamically allocated `string` structure containing the retrieved string.
 *                 Returns NULL if any error occurs.
 */
string *retrieve_string_from_js_value(void *js_env, void *js_value) {
// BEGIN SOLUTION
    size_t len;
    js_get_value_string_ascii(js_env, js_value, NULL, 0, &len);
    string *s = alloc(len + 1);
    if (s == NULL) {
        return NULL;
    }
    js_get_value_string_ascii(js_env, js_value, s->str, len + 1, &len);
    s->size -= 1;
    return s;
// END SOLUTION
}
