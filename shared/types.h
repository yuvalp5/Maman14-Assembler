/**
 * @brief Basic + custom types for use in project
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"

//macro struct - suggestion
typedef struct {
 char name[MAX_MACRO_NAME_LEN];
 char content[MAX_MACRO_CONTENT_LEN];
} Macro;


// stack DB implementation
typedef struct{
 int *collection;
 int capacity;
 int size;
} Stack;

// Utility commands for the stack
Stack *create_stack(int capacity);

void destroy_stack(Stack *stack);

bool is_full(Stack *stack);

bool is_empty(Stack *stack);

bool pop(Stack *stack, int *item);

bool push(Stack *stack, int item);

bool peek(Stack *stack, int *item);
