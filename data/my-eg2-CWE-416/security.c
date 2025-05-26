#include <stdio.h>
#include <stdlib.h>

typedef struct Context {
    struct Context *parent;      // Parent context
    unsigned char *data;         // Data buffer
    size_t data_size;           // Size of the data buffer
    int child_count;             // Number of child contexts
} Context;

static int called = 0;
void processContext(Context *ctx) {
    ctx->data[0] = 0; // Dummy operation to find memory leaks if any
    called++;
}

void *flushContext(Context *ctx) {
// BEGIN SOLUTION
    Context *parent = ctx->parent;

    processContext(ctx);

    free(ctx->data);
    free(ctx);

    if (parent) {
        parent->child_count--;
    }

    return NULL;
// VULNERABILE SOLUTION
    // Context *parent = ctx->parent;

    // processContext(ctx);

    // free(ctx->data);
    // free(ctx);

    // if (ctx->parent) {
    //     ctx->parent->child_count--;
    // }

    // return NULL;
// END SOLUTION
}

int main() {
    Context *ctx = malloc(sizeof(Context));
    ctx->data = malloc(100);
    ctx->data_size = 100;
    ctx->parent = NULL;
    ctx->child_count = 0;

    flushContext(ctx);

    if (called != 1) {
        printf("Error: processContext was not called exactly once.\n");
        return 1;
    }

    Context *pa = malloc(sizeof(Context));
    pa->data = malloc(100);
    pa->data_size = 100;
    pa->parent = NULL;
    pa->child_count = 1;

    Context *child = malloc(sizeof(Context));
    child->data = malloc(100);
    child->data_size = 100;
    child->parent = pa;
    child->child_count = 0;

    flushContext(child);

    if (called != 2) {
        printf("Error: processContext was not called exactly twice.\n");
        return 1;
    }

    if (pa->child_count != 0) {
        printf("Error: Parent's child count was not decremented correctly.\n");
        return 1;
    }

    flushContext(pa);
    if (called != 3) {
        printf("Error: processContext was not called exactly three times.\n");
        return 1;
    }

    return 0;
}

