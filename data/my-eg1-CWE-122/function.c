#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

char *js_string = NULL;
void js_get_value_string_ascii(void *js_env, void *js_value, char *buf, size_t buf_size, size_t *result) {
    if (buf == NULL) {
        if (result != NULL)
            *result = strlen(js_string);
    } else if (buf != NULL && buf_size > 0) {
        size_t len = strlen(js_string);
        if (len > buf_size - 1) {
            len = buf_size - 1;
        }
        memcpy(buf, js_string, len);
        buf[len] = '\0';
        if (result != NULL)
            *result = len;
    } else {
        if (result != NULL)
            *result = 0;
    }
}

typedef struct string {
    int size;
    char str[];
} string;

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

int main() {
    char *tests[] = {"hello", "world", "this is a test", "another test", "test string"};

    const int TEST = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < TEST; i++) {
        js_string = tests[i];

        size_t len = strlen(js_string);
        string *s = retrieve_string_from_js_value((void *)(100), (void *)(100LL));

        if (s == NULL) {
            return 1;
        }

        if (s->size != len) {
            return 1;
        }

        if (strncmp(s->str, js_string, len) != 0) {
            return 1;
        }

        free_string(s);
    }
}
