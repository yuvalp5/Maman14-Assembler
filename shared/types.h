/**
 * @brief Basic + custom types for use in project
 */

#ifndef DEFINITIONS
#define DEFINITIONS
#include "../wrapper.h"
#endif

int is_empty();

int peek();

int push(int value);

int pop();

/* Destroy stack and free all memory */
int destroy_stack();

/*
// macro struct - suggestion- TODO
typedef struct {
    char name[MAX_MACRO_NAME_LEN];
    char content[MAX_MACRO_CONTENT_LEN];
} Macro;

// stack DB implementation - TODO
typedef struct Stack {
    int *collection;
    int capacity;
    int size;
} Stack;

// Utility commands for the stack
Stack *create_stack(int capacity);

void destroy_stack(Stack *stack);

int is_full(Stack *stack);

int is_empty(Stack *stack);

int pop(Stack *stack, int *item);

int push(Stack *stack, int item);

int peek(Stack *stack, int *item);
*/