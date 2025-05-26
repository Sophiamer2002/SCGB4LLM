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
// VULNERABLE SOLUTION
    // node->prev->next = node->next;
    // node->next->prev = node->prev;
    // free(node);
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
    // Test case 1: Delete the head node
    Node* head = create_node(10);
    Node* second = create_node(20);
    head->next = second;
    second->prev = head;

    delete_node(head);

    assert(second->prev == NULL); // Ensure second node's prev is NULL

    free(second);

    // Test case 2: Delete the tail node
    Node* tail1 = create_node(30);
    Node* tail2 = create_node(40);
    tail1->next = tail2;
    tail2->prev = tail1;

    delete_node(tail2);

    assert(tail1->next == NULL); // Ensure tail1's next is NULL

    free(tail1);

    // Test case 3: Delete a NULL node
    delete_node(NULL); // Should not crash or cause any issues

    printf("All tests passed!\n");
    return 0;
}

