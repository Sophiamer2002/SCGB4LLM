#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<stdint.h>

void *__real_malloc(size_t);
void *__wrap_malloc(size_t size);
void *__real_calloc(size_t, size_t);
void *__wrap_calloc(size_t, size_t);

bool recorded = false;
size_t recorded_size = 0;

void init() {
    recorded = false;
    recorded_size = 0;
}

void *__wrap_malloc(size_t size)
{
    recorded = true;
    recorded_size = size;
    return (void *)1;
}

void *__wrap_calloc(size_t nmemb, size_t size)
{
    recorded = true;
    recorded_size = nmemb * size;
    return (void *)1;
}

typedef struct img {
    char data[1000][1000];
} img;

img *allocate_images(size_t cnt) {
// BEGIN SOLUTION
    if (cnt != 0 && sizeof(img) > SIZE_MAX / cnt) {
        return NULL;
    }

    return malloc(cnt * sizeof(img));
// VULNERABLE SOLUTION
    // return malloc(cnt * sizeof(img));
// END SOLUTION
}

int main() {
    size_t tests[] = {-1, (size_t)1 << 62};

    const int TEST = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < TEST; i++) {
        size_t len = tests[i];

        init();

        img *_ = allocate_images(len);

        if (recorded) {
            return 1;
        }
    }
}
