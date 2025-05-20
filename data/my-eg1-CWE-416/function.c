#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

static size_t sAmount = 0;

size_t MallocSizeOf(void *p) {
    if (!p) {
        return 0;
    } else {
        return 1;
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
    if (sAmount != 1) {
        printf("Error: sAmount should be 1 after allocation.\n");
        return 1;
    }

    CountingFree(p);
    if (sAmount != 0) {
        printf("Error: sAmount should be 0 after freeing.\n");
        return 1;
    }

    p = CountingAlloc(10);
    if (sAmount != 1) {
        printf("Error: sAmount should be 1 after allocation.\n");
        return 1;
    }

    p = CountingRealloc(20, p);
    if (sAmount != 1) {
        printf("Error: sAmount should be 1 after reallocating.\n");
        return 1;
    }

    void *pnew = CountingRealloc((size_t)(-8), p);
    if (pnew != NULL || sAmount != 1) {
        printf("Error: sAmount should be 1 after reallocating with size -8.\n");
        return 1;
    }

    CountingFree(p);
    if (sAmount != 0) {
        printf("Error: sAmount should be 0 after freeing.\n");
        return 1;
    }
}

