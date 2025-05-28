#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

static size_t sAmount = 0;

struct malloc_area {
    void *ptr;
    size_t size;
    bool using;
} records[1000];

void *__real_malloc(size_t);
void *__real_free(void *);
void *__real_realloc(void *p, size_t new_size);

void *__wrap_malloc(size_t size);
void __wrap_free(void *p);
void *__wrap_realloc(void *p, size_t new_size);

void *__wrap_malloc(size_t size) {
    void *p = __real_malloc(size);
    if (p) {
        for (int i = 0; i < 1000; i++) {
            if (!records[i].using) {
                records[i].ptr = p;
                records[i].size = size;
                records[i].using = true;
                break;
            }
        }
    }
    return p;
}

void __wrap_free(void *p) {
    __real_free(p);
    for (int i = 0; i < 1000; i++) {
        if (records[i].using && records[i].ptr == p) {
            records[i].using = false;
            break;
        }
    }
}

void *__wrap_realloc(void *p, size_t new_size) {
    void *pnew = __real_realloc(p, new_size);
    if (pnew) {
        for (int i = 0; i < 1000; i++) {
            if (records[i].using && records[i].ptr == p) {
                records[i].ptr = pnew; // Update the pointer
                records[i].size = new_size; // Update the size
                break;
            }
        }
    } else if (new_size == 0) {
        for (int i = 0; i < 1000; i++) {
            if (records[i].using && records[i].ptr == p) {
                records[i].using = false;
                break;
            }
        }
    }

    return pnew;
}

size_t MallocSizeOf(void *p) {
    if (!p) {
        return 0;
    } else {
        for (int i = 0; i < 1000; i++) {
            if (records[i].using && records[i].ptr == p) {
                return records[i].size;
            }
        }

        exit(1); // If the pointer is not found, exit with an error
    }
}

void* CountingAlloc(size_t size) {
    void* p = malloc(size);
    if (p) {
        sAmount += MallocSizeOf(p);
    }
    return p;
}

void CountingFree(void* p) {
    if (p) {
        sAmount -= MallocSizeOf(p);
        free(p);
    }
}

void* CountingRealloc(size_t new_size, void* p) {
// BEGIN SOLUTION
    sAmount -= MallocSizeOf(p);
    void* pnew = realloc(p, new_size);

    if (pnew) {
        sAmount += MallocSizeOf(pnew);
    } else if (new_size != 0) {
        sAmount += MallocSizeOf(p);
    }

    return pnew;
// END SOLUTION
}

int main() {
    void *p = CountingAlloc(10);
    if (sAmount != 10) {
        printf("Error: sAmount should be 10 after allocation.\n");
        return 1;
    }

    CountingFree(p);
    if (sAmount != 0) {
        printf("Error: sAmount should be 0 after freeing.\n");
        return 1;
    }

    p = CountingAlloc(10);
    if (sAmount != 10) {
        printf("Error: sAmount should be 10 after allocation.\n");
        return 1;
    }

    p = CountingRealloc(20, p);
    if (sAmount != 20) {
        printf("Error: sAmount should be 20 after reallocating.\n");
        return 1;
    }

    void *pnew = CountingRealloc((size_t)(-8), p);
    if (pnew != NULL || sAmount != 20) {
        printf("Error: sAmount should be 20 after reallocating with size -8.\n");
        return 1;
    }

    CountingFree(p);
    if (sAmount != 0) {
        printf("Error: sAmount should be 0 after freeing.\n");
        return 1;
    }
}

