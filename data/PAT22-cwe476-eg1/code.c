#include <stdlib.h>

struct vec_int {
    long len;
    int arr[];
};

// Function to create a new vector of integers
// Allocates memory for the vector and initializes its length
// Returns a pointer to the new vector or NULL if allocation fails
struct vec_int *new_vec_int(long len) {
// BEGIN SOLUTION
    if (len < 0 || sizeof(struct vec_int) > __LONG_MAX__ - len) {
        return NULL;
    }

    struct vec_int *v = malloc(sizeof(struct vec_int) + sizeof(int) * len);
    if (v) {
        v->len = len;
    }
    return v;
// END SOLUTION
}
