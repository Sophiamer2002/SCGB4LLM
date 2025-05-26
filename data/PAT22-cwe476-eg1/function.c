#include <stdio.h>
#include <assert.h>
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

int main() {
    // Test case 1: Valid length
    long len = 5;
    struct vec_int *v = new_vec_int(len);
    assert(v != NULL); // Ensure the vector is created
    assert(v->len == len); // Ensure the length is correctly set
    free(v); // Free the allocated memory

    // Test case 2: Negative length
    len = -10;
    v = new_vec_int(len);
    assert(v == NULL); // Ensure NULL is returned for invalid length

    // Test case 3: Large length
    len = 1000000;
    v = new_vec_int(len);
    assert(v != NULL); // Ensure the vector is created for large length
    assert(v->len == len); // Ensure the length is correctly set
    free(v); // Free the allocated memory

    printf("All tests passed!\n");
    return 0;
}
