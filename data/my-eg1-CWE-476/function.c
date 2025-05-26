#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

typedef struct Node {
    int data;
    struct Node* next;
    struct Node* prev;
} Node;

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

Node* create_node(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node != NULL) {
        node->data = data;
        node->next = NULL;
        node->prev = NULL;
    }
    return node;
}

int main() {
    // Test case 1: Delete a middle node
    Node* node1 = create_node(1);
    Node* node2 = create_node(2);
    Node* node3 = create_node(3);
    node1->next = node2;
    node2->prev = node1;
    node2->next = node3;
    node3->prev = node2;

    delete_node(node2);

    assert(node1->next == node3); // Ensure node1 points to node3
    assert(node3->prev == node1); // Ensure node3 points back to node1

    free(node1);
    free(node3);

    printf("All tests passed!\n");
    return 0;
}
