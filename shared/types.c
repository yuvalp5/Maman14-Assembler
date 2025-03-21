#include "types.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Basic DB item structure as K-V pair */
typedef struct Item {
    void *name;
    void *value;
    struct Item *next;
} Item;

/* Basic Node structure */
typedef struct Node {
    int data;
    struct Node *next;
} Node;

/* Stack structure */
typedef struct Stack {
    int size;
    Node *head;
} Stack;

/* Stack instance for use */
Stack stack = {0, NULL};

int is_empty() { return stack.size == 0; }

int peek() {
    if (is_empty()) {
        printf("Stack is empty! Cannot peek.\n");
        return -1;
    }
    return stack.head->data;
}

int push(int value) { // TODO remember this return val at error
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) {
        print_and_log("[TYPES:] Memory allocation failed\n");
        return 1;
    }
    new_node->data = value;
    new_node->next = stack.head;
    stack.head = new_node;
    stack.size++;
    print_and_log(("[TYPES:] Pushed %d onto the stack.\n", value));
    return 0;
}

int pop() { // TODO remember this return val at error
    if (is_empty()) {
        print_and_log("[TYPES:] Stack is empty! Cannot pop.\n");
        return NULL;
    }
    Node *top_node = stack.head;
    int value = top_node->data;
    stack.head = top_node->next;
    free(top_node);
    stack.size--;
    return value;
}

/* Destroy stack and free all memory */
int destroy_stack() {
    while (!is_empty()) {
        pop();
    }
    print_and_log("[TYPES:] Stack destroyed.\n");
    return 0;
}

// old ver.
/*
int pop(Stack *stack, int *item) { // int item will not be int of course
    if (is_empty(stack)) {
        return NULL;
    } else {
        *item = stack->collection[stack->size - 1];
        stack->size--;
        return 1;
    }
}

int push(Stack *stack, int item) {
    if (is_full(stack)) {
        return 0;
    }
    stack->collection[stack->size] = item;
    stack->size++;
    return 1;
}

int peek(Stack *stack, int *item) {
    if (is_empty(stack)) {
        return 0;
    }
    *item = stack->collection[stack->size - 1];
}
*/