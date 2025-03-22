#include "types.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* Basic DB item structure as K-V pair */
typedef struct Item {
    void *name;
    void *value;
    struct Item *next;
} Item;

/* Basic table structure as array of items */
typedef struct Table {
    int size;
    Item *content;
} Table;

/* Stack instance for use - internal variable */
Stack stack = {0, NULL};
/* Labels table instance */
Table label_table = {0, NULL};
/* Macro table instance */
Table macro_table = {0, NULL};

int get_stack_size() { return stack.size; }

int peek() {
    if (is_empty()) {
        print_and_log("[TYPES:] Stack is empty! Cannot peek.\n");
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

Table label_table = {0, NULL};
Table macro_table = {0, NULL};

/* Label table functions */
int get_label_table_size() { return label_table.size; }

int add_label(char *name, int value) {
    /* Allocate memory for the value */
    int *value_ptr = (int *)malloc(sizeof(int));
    if (!value_ptr) {
        print_and_log("[TYPES:] Memory allocation failed for label value\n");
        return 1;
    }
    *value_ptr = value;

    /* Check if the label already exists */
    Item *current = label_table.content;
    while (current) {
        if (strcmp((char *)current->name, name) == 0) {
            /* Free the old value and update */
            free(current->value);
            current->value = value_ptr;
            print_and_log(
                ("[TYPES:] Updated label %s with value %d.\n", name, value));
            return 0;
        }
        current = current->next;
    }

    /* Create new item */
    Item *new_item = (Item *)malloc(sizeof(Item));
    if (!new_item) {
        free(value_ptr);
        print_and_log("[TYPES:] Memory allocation failed for label item\n");
        return 1;
    }

    new_item->name = strdup(name);
    if (!new_item->name) {
        free(value_ptr);
        free(new_item);
        print_and_log("[TYPES:] Memory allocation failed for label name\n");
        return 1;
    }

    new_item->value = value_ptr;
    new_item->next = label_table.content;
    label_table.content = new_item;
    label_table.size++;

    print_and_log(("[TYPES:] Added label %s with value %d.\n", name, value));
    return 0;
}

int get_label(char *name) {
    Item *current = label_table.content;

    while (current) {
        if (strcmp((char *)current->name, name) == 0) {
            return *((int *)current->value);
        }
        current = current->next;
    }

    return -1; /* Return -1 if label not found */
}

/* String table functions */
int get_string_table_size() { return macro_table.size; }

int add_string(char *name, char *value) {
    /* Check if the string already exists */
    Item *current = macro_table.content;
    while (current) {
        if (strcmp((char *)current->name, name) == 0) {
            /* Free the old value and update */
            free(current->value);
            current->value = strdup(value);
            if (!current->value) {
                print_and_log(
                    "[TYPES:] Memory allocation failed for string value\n");
                return 1;
            }
            print_and_log(
                ("[TYPES:] Updated string mapping %s to %s.\n", name, value));
            return 0;
        }
        current = current->next;
    }

    /* Create new item */
    Item *new_item = (Item *)malloc(sizeof(Item));
    if (!new_item) {
        print_and_log("[TYPES:] Memory allocation failed for string item\n");
        return 1;
    }

    new_item->name = strdup(name);
    if (!new_item->name) {
        free(new_item);
        print_and_log("[TYPES:] Memory allocation failed for string name\n");
        return 1;
    }

    new_item->value = strdup(value);
    if (!new_item->value) {
        free(new_item->name);
        free(new_item);
        print_and_log("[TYPES:] Memory allocation failed for string value\n");
        return 1;
    }

    new_item->next = macro_table.content;
    macro_table.content = new_item;
    macro_table.size++;

    print_and_log(("[TYPES:] Added string mapping %s to %s.\n", name, value));
    return 0;
}

char *get_string(char *name) {
    Item *current = macro_table.content;

    while (current) {
        if (strcmp((char *)current->name, name) == 0) {
            return (char *)current->value;
        }
        current = current->next;
    }

    return NULL; /* Return NULL if string not found */
}

/* Free memory allocated for tables */
void destroy_label_table() {
    Item *current = label_table.content;
    Item *next;

    while (current) {
        next = current->next;
        free(current->name);
        free(current->value);
        free(current);
        current = next;
    }

    label_table.content = NULL;
    label_table.size = 0;
    print_and_log("[TYPES:] Label table destroyed.\n");
}

void destroy_string_table() {
    Item *current = macro_table.content;
    Item *next;

    while (current) {
        next = current->next;
        free(current->name);
        free(current->value);
        free(current);
        current = next;
    }

    macro_table.content = NULL;
    macro_table.size = 0;
    print_and_log("[TYPES:] String table destroyed.\n");
}