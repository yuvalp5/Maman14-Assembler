#include "types.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Labels table instance */
Table label_table = {0, NULL};
/* Macro table instance */
Table macro_table = {0, NULL};
Symbol symbol_table[MAX_SYMBOLS];

/* Global variables */
int DC = 0;           /* Data counter initialized to 0 */
int IC = 100;         /* Instruction counter initialized to 100 */
int symbol_count = 0; /* Number of symbols in the symbol table */
int error_count = 0;  /* Number of errors */
int line_number = 0;  /* Line number */
int ICF, DCF;         /* Final IC and DC values */
int L;                /* Number of words in the instruction */

/* Label table functions */
int get_label_table_size() { return label_table.size; }

int add_label(char *name, int value) {
    /* Allocate memory for the value */
    int *value_ptr = (int *)malloc(sizeof(int));
    if (!value_ptr) {
        printf("[TYPES:] Memory allocation failed for label value\n");
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
            printf("[TYPES:] Updated label %s with value %d.\n", name, value);
            return 0;
        }
        current = current->next;
    }

    /* Create new item */
    Item *new_item = (Item *)malloc(sizeof(Item));
    if (!new_item) {
        free(value_ptr);
        printf("[TYPES:] Memory allocation failed for label item\n");
        return 1;
    }

    new_item->name = strdup(name);
    if (!new_item->name) {
        free(value_ptr);
        free(new_item);
        printf("[TYPES:] Memory allocation failed for label name\n");
        return 1;
    }

    new_item->value = value_ptr;
    new_item->next = label_table.content;
    label_table.content = new_item;
    label_table.size++;

    printf("[TYPES:] Added label %s with value %d.\n", name, value);
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
                printf("[TYPES:] Memory allocation failed for string value\n");
                return 1;
            }
            printf("[TYPES:] Updated string mapping %s to %s.\n", name, value);
            return 0;
        }
        current = current->next;
    }

    /* Create new item */
    Item *new_item = (Item *)malloc(sizeof(Item));
    if (!new_item) {
        printf("[TYPES:] Memory allocation failed for string item\n");
        return 1;
    }

    new_item->name = strdup(name);
    if (!new_item->name) {
        free(new_item);
        printf("[TYPES:] Memory allocation failed for string name\n");
        return 1;
    }

    new_item->value = strdup(value);
    if (!new_item->value) {
        free(new_item->name);
        free(new_item);
        printf("[TYPES:] Memory allocation failed for string value\n");
        return 1;
    }

    new_item->next = macro_table.content;
    macro_table.content = new_item;
    macro_table.size++;

    printf("[TYPES:] Added string mapping %s to %s.\n", name, value);
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
    printf("[TYPES:] Label table destroyed.\n");
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
    printf("[TYPES:] String table destroyed.\n");
}