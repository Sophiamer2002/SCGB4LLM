#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @brief A global variable for tracking total memory usage.
 * 
 * This variable is used to account for the total memory allocated via
 * custom memory management functions.
 */
static size_t sAmount = 0;

/**
 * @brief Memory reporting functions for tracking memory usage.
 * 
 * These functions calculate the size of allocated and freed memory blocks.
 */
extern size_t MallocSizeOf(void *p);

/**
 * This function allocates memory and updates the global memory usage tracker,
 * serving as a wrapper for the standard malloc function.
 * 
 * @param size The size of the memory block to allocate.
 * @return A pointer to the newly allocated memory block.
 */
void* CountingAlloc(size_t size) {
    void* p = malloc(size);
    if (p) {
        sAmount += MallocSizeOf(p);
    }
    return p;
}

/**
 * This function frees memory and updates the global memory usage tracker.
 * It serves as a wrapper for the standard free function.
 * 
 * @param p Pointer to the memory block to free.
 */
void CountingFree(void* p) {
    if (p) {
        sAmount -= MallocSizeOf(p);
        free(p);
    }
}

/**
 * This function reallocates memory while updating the global memory usage
 * tracker, serving as a wrapper for the standard realloc function.
 * 
 * @param cur_size The current size of the memory block.
 * @param new_size The new size of the memory block.
 * @param p Pointer to the current memory block.
 * @return A pointer to the newly allocated memory, same as the returned value of realloc.
 */
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
