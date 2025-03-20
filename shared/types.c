#include "types.h"

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif
#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif
#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

/* Basic DB item structure */
typedef struct Item {
    void *name;
    void *value;
    struct Item *next; // Pointer to the next item
} Item;

/* Stack structure */
typedef struct {
    int size;
    Item *top; // Top of the stack (LIFO)
} Stack;

/* Initialize a new stack */
int initStack(Stack *stack) {
    stack->size = 0;
    stack->top = NULL;
    return 0;
}

/* Push an item onto the stack */
int push(Stack *stack, void *name, void *value) {
    Item *newItem = (Item *)malloc(sizeof(Item));
    if (!newItem) {
        perror("Failed to allocate memory");
        return;
    }
    newItem->name = name;
    newItem->value = value;
    newItem->next = stack->top; // Link new item to the old top
    stack->top = newItem;       // Update stack top
    stack->size++;
    return 0;
}

/* Pop an item from the stack */
Item *pop(Stack *stack) {
    if (stack->size == 0) {
        return NULL; // Stack is empty
    }
    Item *poppedItem = stack->top;
    stack->top = poppedItem->next; // Update top to the next item
    stack->size--;
    return poppedItem;
}

/* Free the stack */
int destroy_stack(Stack *stack) {
    while (stack->top) {
        Item *temp = pop(stack);
        free(temp);
    }
    return 0;
}

int is_empty(Stack *stack) { return stack->size == 0; }

int pop(Stack *stack, int *item) { // int item will not be int of course
    if (is_empty(stack)) {
        return;
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
