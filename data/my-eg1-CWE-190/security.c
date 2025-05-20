#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

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
// VULNERABLE SOLUTION
    // size_t len = strlen(js_value);
    // if (buf == NULL) {
    //     if (result != NULL) {
    //         *result = len;
    //     }
    // } else {
    //     size_t copy_size = len < buf_size - 1 ? len : buf_size - 1;
    //     memcpy(buf, js_value, copy_size);
    //     buf[copy_size] = '\0'; // Null-terminate the string

    //     if (result != NULL) {
    //         *result = copy_size;
    //     }
    // }
// END SOLUTION
}

int main() {
    const char *tests[] = {
        "Hello, World!",
        "This is a test string.",
        "Another test string with more characters.",
        "Short",
        "aaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbccccccccccccccccddddddddddddddeeeeeeeeeeeeeeeffffffffffffffggggggggggggggghhhhhhhhhhhhhhhiiiiiiiiiiiiiiiiijjjjjjjjjjjjjjjjkkkkkkkkkkkkkkkllllllllllllllmmmmmmmmmmmmmmmmmnnnnnnnnnnnnnnnoooooooooooooooopppppppppppppqqqqqqqqqqqqqqqrrrrrrrrrrrrrrrsssssssssssss",
    };

    const int TEST = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < TEST; i++) {
        const char *js_value = tests[i];
        size_t len = strlen(js_value);

        #define MAX_BUFFER_SIZE 1000
        #define HALF_BUF (MAX_BUFFER_SIZE / 2)

        char dest_buffer[MAX_BUFFER_SIZE];
        char rand_buffer[MAX_BUFFER_SIZE];

        for (int j = 0; j < MAX_BUFFER_SIZE; j++) {
            rand_buffer[j] = rand() % 256;
        }

        size_t result;

        size_t buf_len = 300;
        char *buffer = dest_buffer + HALF_BUF;
        memcpy(dest_buffer, rand_buffer, MAX_BUFFER_SIZE);
        js_get_value_string_ascii(js_value, buffer, buf_len, &result);
        if (result != len || strcmp(buffer, js_value) != 0
            || memcmp(dest_buffer, rand_buffer, HALF_BUF) != 0
            || memcmp(dest_buffer + HALF_BUF + buf_len, rand_buffer + HALF_BUF + buf_len, HALF_BUF - buf_len) != 0
        ) {
            printf("Error1: %s\n", buffer);
            return 1;
        }

        buf_len = 0;
        memcpy(dest_buffer, rand_buffer, MAX_BUFFER_SIZE);
        js_get_value_string_ascii(js_value, buffer, buf_len, &result);
        if (memcmp(dest_buffer, rand_buffer, MAX_BUFFER_SIZE) != 0) {
            printf("Error2: %s\n", js_value);
            return 1;
        }

        buf_len = (len + 1) / 2;
        memcpy(dest_buffer, rand_buffer, MAX_BUFFER_SIZE);
        js_get_value_string_ascii(js_value, buffer, buf_len, &result);
        if (result != buf_len - 1 || strncmp(buffer, js_value, result) != 0 || buffer[result] != '\0'
            || memcmp(dest_buffer, rand_buffer, HALF_BUF) != 0
            || memcmp(dest_buffer + HALF_BUF + buf_len, rand_buffer + HALF_BUF + buf_len, HALF_BUF - buf_len) != 0
        ) {
            printf("Error3: %s\n", js_value);
            return 1;
        }

        js_get_value_string_ascii(js_value, NULL, MAX_BUFFER_SIZE, &result);
        if (result != len) {
            printf("Error4: %s\n", js_value);
            return 1;
        }

        buf_len = 300;
        memcpy(dest_buffer, rand_buffer, MAX_BUFFER_SIZE);
        js_get_value_string_ascii(js_value, buffer, buf_len, NULL);
        if (result != len || strcmp(buffer, js_value) != 0
            || memcmp(dest_buffer, rand_buffer, HALF_BUF) != 0
            || memcmp(dest_buffer + HALF_BUF + buf_len, rand_buffer + HALF_BUF + buf_len, HALF_BUF - buf_len) != 0
        ) {
            printf("Error5: %s\n", js_value);
            return 1;
        }
    }

    return 0;
}
