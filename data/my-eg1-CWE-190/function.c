#include <string.h>
#include <stddef.h>

void js_get_value_string_ascii(const char *js_value, char *buf, size_t buf_size, size_t *result) {
// BEGIN SOLUTION
    size_t len = strlen(js_value);
    if (buf == NULL) {
        if (result != NULL) {
            *result = len;
        }
    } else if (buf_size != 0) {
        size_t copy_size = len < buf_size - 1 ? len : buf_size - 1;
        memcpy(buf, js_value, copy_size);
        buf[copy_size] = '\0';

        if (result != NULL) {
            *result = copy_size;
        }
    }
// END SOLUTION
}

int main() {
    const char *tests[] = {
        "Hello, World!",
        "This is a test string.",
        "Another test string with more characters.",
        "Short"
    };

    const int TEST = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < TEST; i++) {
        const char *js_value = tests[i];
        size_t len = strlen(js_value);

        char buffer[100];
        size_t result;
        js_get_value_string_ascii(js_value, buffer, sizeof(buffer), &result);

        if (result != len || strcmp(buffer, js_value) != 0) {
            return 1;
        }

        js_get_value_string_ascii(js_value, buffer, (len + 1) / 2, &result);
        if (result != (len - 1) / 2 || strncmp(buffer, js_value, result) != 0 || buffer[result] != '\0') {
            return 1;
        }

        js_get_value_string_ascii(js_value, NULL, sizeof(buffer), &result);

        if (result != len) {
            return 1;
        }
    }

    return 0;
}
