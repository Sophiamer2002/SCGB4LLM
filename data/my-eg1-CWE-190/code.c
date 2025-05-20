#include <string.h>

/**
 * @brief Retrieves a JavaScript string value and writes it into a buffer as an ASCII-encoded string.
 * 
 * This function extracts a JavaScript string (a C-like null-terminated string) and writes it into a provided buffer
 * in ASCII encoding. If the buffer is NULL, the function returns the length of the string in bytes
 * (excluding the null terminator) through the `result` parameter.
 * 
 * @param js_value [in] js_value representing the JavaScript string.
 * @param buf [in] Buffer to write the ASCII-encoded string into. If NULL is passed in, the length 
 *                 of the string (excluding the null terminator) is returned. Note that the string
 *                 written to the buffer is always null-terminated, even if the buffer size is insufficient.
 * @param buf_size [in] Size of the destination buffer. When this value is insufficient, the returned 
 *                      string is truncated and null-terminated.
 * @param result [out] Number of bytes copied into the buffer, excluding the null terminator.
 */
void js_get_value_string_ascii(const char *js_value, char *buf, size_t buf_size, size_t *result) {
// BEGIN SOLUTION
    size_t len = strlen(js_value);
    if (buf == NULL) {
        if (result != NULL) {
            *result = len;
        }
    } else if (buf_size == 0) {
        if (result != NULL) {
            *result = 0;
        }
    } else {
        size_t copy_size = len < buf_size - 1 ? len : buf_size - 1;
        memcpy(buf, js_value, copy_size);
        buf[copy_size] = '\0'; // Null-terminate the string

        if (result != NULL) {
            *result = copy_size;
        }
    }
// END SOLUTION
}
