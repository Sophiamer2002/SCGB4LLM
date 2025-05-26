#include <stdio.h>
#include <stdlib.h>

typedef struct Context {
    struct Context *parent;      // Parent context
    unsigned char *data;         // Data buffer
    size_t data_size;           // Size of the data buffer
    int child_count;             // Number of child contexts
} Context;

extern Context* initContext();
extern void connectParentChild(Context *parent, Context *child);
extern void processContext(Context *ctx);

// flush the context, in three steps:
// 1. process the context
// 2. free the allocated memory
// 3. update its parent(decrement the parent's child count)
void *flushContext(Context *ctx) {
// BEGIN SOLUTION
    Context *parent = ctx->parent;

    // Process the context
    processContext(ctx);

    // Free the allocated memory
    free(ctx->data);
    free(ctx);

    // Decrement the parent's child count
    if (parent) {
        parent->child_count--;
    }

    return NULL;
// END SOLUTION
}
