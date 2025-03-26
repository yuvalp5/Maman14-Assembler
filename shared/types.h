/**
 * @brief Basic + custom types for use in project
 */

#ifndef TYPES_H
#define TYPES_H 1

#include "definitions.h"

/* Basic Item structure */
typedef struct Item {
    char *name;
    char *value;
    struct Item *next;
} Item;

/* Table structure */
typedef struct {
    Item *content;
    int size;
} Table;

/* Global tables */
extern Table label_table;
extern Table string_table;
extern Table macro_table;

/* Stack functions */
int is_empty(void);
int get_stack_size(void);
int peek(void);
int push(int value);
int pop(void);
int destroy_stack(void);

/* Label table functions */
int get_label_table_size(void);
int add_label(char *name, int value);
int get_label(char *name);
void destroy_label_table(void);

/* String table functions */
int get_string_table_size(void);
int add_string(char *name, char *value);
char *get_string(char *name);
void destroy_string_table(void);

/* Custom string functions */
char *my_strdup(const char *str);

#endif
