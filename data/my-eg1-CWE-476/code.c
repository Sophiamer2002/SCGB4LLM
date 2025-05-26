#include <stddef.h>

typedef struct Node {
    int data;
    struct Node* next;
    struct Node* prev;
} Node;

// Function to delete a node from a doubly linked list
// Free the memory of the node and update the pointers of the previous and next nodes
void delete_node(Node* node) {
// BEGIN SOLUTION
    if (node == NULL) return;

    if (node->prev != NULL)
        node->prev->next = node->next;

    if (node->next != NULL)
        node->next->prev = node->prev;

    free(node);
// END SOLUTION
}
